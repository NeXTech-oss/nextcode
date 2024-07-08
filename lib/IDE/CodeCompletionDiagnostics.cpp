//===--- CodeCompletionDiagnostics.cpp ------------------------------------===//
//
/*
 * Copyright (c) 2024, NeXTech Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * Contact with NeXTech, 640 N McCarthy Blvd, in the
 * city of Milpitas, zip code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 */

// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "CodeCompletionDiagnostics.h"

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsIDE.h"
#include "nextcode/AST/DiagnosticsSema.h"

using namespace nextcode;
using namespace ide;

namespace {

CodeCompletionDiagnosticSeverity getSeverity(DiagnosticKind DiagKind) {
  switch (DiagKind) {
  case DiagnosticKind::Error:
    return CodeCompletionDiagnosticSeverity::Error;
    break;
  case DiagnosticKind::Warning:
    return CodeCompletionDiagnosticSeverity::Warning;
    break;
  case DiagnosticKind::Remark:
    return CodeCompletionDiagnosticSeverity::Remark;
    break;
  case DiagnosticKind::Note:
    return CodeCompletionDiagnosticSeverity::Note;
    break;
  }
  llvm_unreachable("unhandled DiagnosticKind");
}

class CodeCompletionDiagnostics {
  const ASTContext &Ctx;
  DiagnosticEngine &Engine;

public:
  CodeCompletionDiagnostics(const ASTContext &Ctx)
      : Ctx(Ctx), Engine(Ctx.Diags) {}

  template <typename... ArgTypes>
  bool
  getDiagnostics(CodeCompletionDiagnosticSeverity &severity,
                 llvm::raw_ostream &Out, Diag<ArgTypes...> ID,
                 typename nextcode::detail::PassArgument<ArgTypes>::type... VArgs);

  bool getDiagnosticForDeprecated(const ValueDecl *D,
                                  CodeCompletionDiagnosticSeverity &severity,
                                  llvm::raw_ostream &Out);
};

template <typename... ArgTypes>
bool CodeCompletionDiagnostics::getDiagnostics(
    CodeCompletionDiagnosticSeverity &severity, llvm::raw_ostream &Out,
    Diag<ArgTypes...> ID,
    typename nextcode::detail::PassArgument<ArgTypes>::type... VArgs) {
  DiagID id = ID.ID;
  std::vector<DiagnosticArgument> DiagArgs{std::move(VArgs)...};
  auto format = Engine.diagnosticStringFor(id, /*printDiagnosticNames=*/false);
  DiagnosticEngine::formatDiagnosticText(Out, format, DiagArgs);
  severity = getSeverity(Engine.declaredDiagnosticKindFor(id));

  return false;
}

bool CodeCompletionDiagnostics::getDiagnosticForDeprecated(
    const ValueDecl *D, CodeCompletionDiagnosticSeverity &severity,
    llvm::raw_ostream &Out) {
  bool isSoftDeprecated = false;
  const AvailableAttr *Attr = D->getAttrs().getDeprecated(Ctx);
  if (!Attr) {
    Attr = D->getAttrs().getSoftDeprecated(Ctx);
    isSoftDeprecated = true;
  }
  if (!Attr)
    return true;

  // FIXME: Code completion doesn't offer accessors. It only emits 'VarDecl's.
  // So getter/setter specific availability doesn't work in code completion.

  StringRef Platform = Attr->prettyPlatformString();
  llvm::VersionTuple DeprecatedVersion;
  if (Attr->Deprecated)
    DeprecatedVersion = Attr->Deprecated.value();

  llvm::VersionTuple RemappedDeprecatedVersion;
  if (AvailabilityInference::updateDeprecatedPlatformForFallback(
      Attr, Ctx, Platform, RemappedDeprecatedVersion))
    DeprecatedVersion = RemappedDeprecatedVersion;

  if (!isSoftDeprecated) {
    if (Attr->Message.empty() && Attr->Rename.empty()) {
      getDiagnostics(severity, Out, diag::availability_deprecated,
                     D, Attr->hasPlatform(), Platform,
                     Attr->Deprecated.has_value(), DeprecatedVersion,
                     /*message*/ StringRef());
    } else if (!Attr->Message.empty()) {
      EncodedDiagnosticMessage EncodedMessage(Attr->Message);
      getDiagnostics(severity, Out, diag::availability_deprecated,
                     D, Attr->hasPlatform(), Platform,
                     Attr->Deprecated.has_value(), DeprecatedVersion,
                     EncodedMessage.Message);
    } else {
      getDiagnostics(severity, Out, diag::availability_deprecated_rename,
                     D, Attr->hasPlatform(), Platform,
                     Attr->Deprecated.has_value(), DeprecatedVersion, false,
                     /*ReplaceKind*/ 0, Attr->Rename);
    }
  } else {
    // '100000' is used as a version number in API that will be deprecated in an
    // upcoming release. This number is to match the 'API_TO_BE_DEPRECATED'
    // macro defined in Darwin platforms.
    static llvm::VersionTuple DISTANT_FUTURE_VESION(100000);
    bool isDistantFuture = DeprecatedVersion >= DISTANT_FUTURE_VESION;

    if (Attr->Message.empty() && Attr->Rename.empty()) {
      getDiagnostics(severity, Out, diag::ide_availability_softdeprecated,
                     D, Attr->hasPlatform(), Platform,
                     !isDistantFuture, DeprecatedVersion,
                     /*message*/ StringRef());
    } else if (!Attr->Message.empty()) {
      EncodedDiagnosticMessage EncodedMessage(Attr->Message);
      getDiagnostics(severity, Out, diag::ide_availability_softdeprecated,
                     D, Attr->hasPlatform(), Platform,
                     !isDistantFuture, DeprecatedVersion,
                     EncodedMessage.Message);
    } else {
      getDiagnostics(severity, Out, diag::ide_availability_softdeprecated_rename,
                     D, Attr->hasPlatform(), Platform,
                     !isDistantFuture, DeprecatedVersion, Attr->Rename);
    }
  }
  return false;;
}

} // namespace

bool nextcode::ide::getContextFreeCompletionDiagnostics(
    ContextFreeNotRecommendedReason Reason, const ValueDecl *D,
    CodeCompletionDiagnosticSeverity &Severity, llvm::raw_ostream &Out) {
  CodeCompletionDiagnostics Diag(D->getASTContext());
  switch (Reason) {
  case ContextFreeNotRecommendedReason::Deprecated:
  case ContextFreeNotRecommendedReason::SoftDeprecated:
    return Diag.getDiagnosticForDeprecated(D, Severity, Out);
  case ContextFreeNotRecommendedReason::None:
    llvm_unreachable("invalid not recommended reason");
  }
  return true;
}

bool nextcode::ide::getContextualCompletionDiagnostics(
    ContextualNotRecommendedReason Reason, StringRef NameForDiagnostics,
    CodeCompletionDiagnosticSeverity &Severity, llvm::raw_ostream &Out,
    const ASTContext &Ctx) {
  CodeCompletionDiagnostics Diag(Ctx);
  switch (Reason) {
  case ContextualNotRecommendedReason::RedundantImport:
    return Diag.getDiagnostics(Severity, Out, diag::ide_redundant_import,
                               NameForDiagnostics);
  case ContextualNotRecommendedReason::RedundantImportIndirect:
    return Diag.getDiagnostics(
        Severity, Out, diag::ide_redundant_import_indirect, NameForDiagnostics);
  case ContextualNotRecommendedReason::VariableUsedInOwnDefinition:
    return Diag.getDiagnostics(Severity, Out,
                               diag::ide_recursive_accessor_reference,
                               NameForDiagnostics, /*"getter"*/ 0);
  case ContextualNotRecommendedReason::NonAsyncAlternativeUsedInAsyncContext:
    return Diag.getDiagnostics(
        Severity, Out, diag::ide_has_async_alternative, NameForDiagnostics);
  case ContextualNotRecommendedReason::None:
    llvm_unreachable("invalid not recommended reason");
  }
  return true;
}
