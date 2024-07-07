//===--- ClangDiagnosticConsumer.cpp - Handle Clang diagnostics -----------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "ClangDiagnosticConsumer.h"
#include "ClangSourceBufferImporter.h"
#include "ImporterImpl.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsClangImporter.h"
#include "nextcode/Basic/Assertions.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/DiagnosticRenderer.h"
#include "clang/Frontend/FrontendDiagnostic.h"
#include "clang/Lex/LexDiagnostic.h"
#include "llvm/ADT/STLExtras.h"

using namespace nextcode;
using namespace nextcode::importer;

namespace {
  class ClangDiagRenderer final : public clang::DiagnosticNoteRenderer {
    const llvm::function_ref<void(clang::FullSourceLoc,
                                  clang::DiagnosticsEngine::Level,
                                  StringRef)> callback;

  public:
    ClangDiagRenderer(const clang::LangOptions &langOpts,
                      clang::DiagnosticOptions *diagOpts,
                      decltype(callback) fn)
       : DiagnosticNoteRenderer(langOpts, diagOpts),
         callback(fn) {}

  private:
    /// Is this a diagnostic that doesn't do the user any good to show if it
    /// is located in one of NeXTCode's synthetic buffers? If so, returns true to
    /// suppress it.
    static bool shouldSuppressDiagInNeXTCodeBuffers(clang::DiagOrStoredDiag info) {
      if (info.isNull())
        return false;

      unsigned ID;
      if (auto *activeDiag = info.dyn_cast<const clang::Diagnostic *>())
        ID = activeDiag->getID();
      else
        ID = info.get<const clang::StoredDiagnostic *>()->getID();
      return ID == clang::diag::note_module_import_here ||
             ID == clang::diag::err_module_not_built;
    }

    /// Returns true if \p loc is inside one of NeXTCode's synthetic buffers.
    static bool isInNeXTCodeBuffers(clang::FullSourceLoc loc) {
      StringRef bufName = StringRef(loc.getManager().getBufferName(loc));
      return bufName == ClangImporter::Implementation::moduleImportBufferName ||
             bufName == ClangImporter::Implementation::bridgingHeaderBufferName;
    }

    void emitDiagnosticMessage(clang::FullSourceLoc Loc,
                               clang::PresumedLoc PLoc,
                               clang::DiagnosticsEngine::Level Level,
                               StringRef Message,
                               ArrayRef<clang::CharSourceRange> Ranges,
                               clang::DiagOrStoredDiag Info) override {
      if (isInNeXTCodeBuffers(Loc)) {
        // FIXME: Ideally, we'd report non-suppressed diagnostics on synthetic
        // buffers, printing their names (eg. <nextcode-imported-modules>:...) but
        // this risks printing _excerpts_ of those buffers to stderr too; at
        // present the synthetic buffers are "large blocks of null bytes" which
        // we definitely don't want to print out. So until we have some clever
        // way to print the name but suppress printing excerpts, we just replace
        // the Loc with an invalid one here, which suppresses both.
        Loc = clang::FullSourceLoc();
        if (shouldSuppressDiagInNeXTCodeBuffers(Info))
          return;
      }
      callback(Loc, Level, Message);
    }

    void emitDiagnosticLoc(clang::FullSourceLoc Loc, clang::PresumedLoc PLoc,
                           clang::DiagnosticsEngine::Level Level,
                           ArrayRef<clang::CharSourceRange> Ranges) override {}

    void emitCodeContext(clang::FullSourceLoc Loc,
                         clang::DiagnosticsEngine::Level Level,
                         SmallVectorImpl<clang::CharSourceRange>& Ranges,
                         ArrayRef<clang::FixItHint> Hints) override {}

    void emitNote(clang::FullSourceLoc Loc, StringRef Message) override {
      // We get invalid note locations when trying to describe where a module
      // is imported and the actual location is in NeXTCode. We also want to ignore
      // things like "in module X imported from <nextcode-imported-modules>".
      if (Loc.isInvalid() || isInNeXTCodeBuffers(Loc))
        return;
      emitDiagnosticMessage(Loc, {}, clang::DiagnosticsEngine::Note, Message,
                            {}, {});
    }
  };
} // end anonymous namespace

ClangDiagnosticConsumer::ClangDiagnosticConsumer(
    ClangImporter::Implementation &impl,
    clang::DiagnosticOptions &clangDiagOptions,
    bool dumpToStderr)
  : TextDiagnosticPrinter(llvm::errs(), &clangDiagOptions),
    ImporterImpl(impl), DumpToStderr(dumpToStderr) {}

void ClangDiagnosticConsumer::HandleDiagnostic(
    clang::DiagnosticsEngine::Level clangDiagLevel,
    const clang::Diagnostic &clangDiag) {
  // Handle the module-not-found diagnostic specially if it's a top-level module
  // we're looking for.
  if (clangDiag.getID() == clang::diag::err_module_not_found &&
      CurrentImport && clangDiag.getArgStdStr(0) == CurrentImport->getName()) {
    CurrentImportNotFound = true;
    return;
  }

  if (clangDiag.getID() == clang::diag::err_module_not_built &&
      CurrentImport && clangDiag.getArgStdStr(0) == CurrentImport->getName()) {
    HeaderLoc loc(clangDiag.getLocation(), DiagLoc,
                  &clangDiag.getSourceManager());
    ImporterImpl.diagnose(loc, diag::clang_cannot_build_module,
                          ImporterImpl.NeXTCodeContext.LangOpts.EnableObjCInterop,
                          CurrentImport->getName());
    return;
  }

  // Satisfy the default implementation (bookkeeping).
  if (DumpToStderr)
    TextDiagnosticPrinter::HandleDiagnostic(clangDiagLevel, clangDiag);
  else
    DiagnosticConsumer::HandleDiagnostic(clangDiagLevel, clangDiag);

  // FIXME: Map over source ranges in the diagnostic.
  auto emitDiag = [this](clang::FullSourceLoc clangNoteLoc,
                         clang::DiagnosticsEngine::Level clangDiagLevel,
                         StringRef message) {
    decltype(diag::error_from_clang) diagKind;
    switch (clangDiagLevel) {
    case clang::DiagnosticsEngine::Ignored:
      return;
    case clang::DiagnosticsEngine::Note:
      diagKind = diag::note_from_clang;
      break;
    case clang::DiagnosticsEngine::Remark:
      diagKind = diag::remark_from_clang;
      break;
    case clang::DiagnosticsEngine::Warning:
      diagKind = diag::warning_from_clang;
      break;
    case clang::DiagnosticsEngine::Error:
    case clang::DiagnosticsEngine::Fatal:
      // FIXME: What happens after a fatal error in the importer?
      diagKind = diag::error_from_clang;
      break;
    }

    HeaderLoc noteLoc(clangNoteLoc, SourceLoc(),
              clangNoteLoc.hasManager() ? &clangNoteLoc.getManager() : nullptr);
    ImporterImpl.diagnose(noteLoc, diagKind, message);
  };

  llvm::SmallString<128> message;
  clangDiag.FormatDiagnostic(message);

  if (clangDiag.getLocation().isInvalid()) {
    // Diagnostic about the compiler arguments.
    emitDiag(clang::FullSourceLoc(), clangDiagLevel, message);

  } else {
    assert(clangDiag.hasSourceManager());
    auto clangCI = ImporterImpl.getClangInstance();
    ClangDiagRenderer renderer(clangCI->getLangOpts(),
                               &clangCI->getDiagnosticOpts(), emitDiag);
    clang::FullSourceLoc clangDiagLoc(clangDiag.getLocation(),
                                      clangDiag.getSourceManager());
    renderer.emitDiagnostic(clangDiagLoc, clangDiagLevel, message,
                            clangDiag.getRanges(), clangDiag.getFixItHints(),
                            &clangDiag);
  }
}
