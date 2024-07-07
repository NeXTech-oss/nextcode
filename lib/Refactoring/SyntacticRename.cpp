//===----------------------------------------------------------------------===//
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
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/DiagnosticsRefactoring.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Frontend/PrintingDiagnosticConsumer.h"
#include "nextcode/IDE/IDEBridging.h"
#include "nextcode/Parse/Lexer.h"
#include "nextcode/Refactoring/Refactoring.h"

using namespace nextcode;
using namespace nextcode::ide;

#if NEXTCODE_BUILD_NEXTCODE_SYNTAX
std::vector<ResolvedAndRenameLoc>
nextcode::ide::resolveRenameLocations(ArrayRef<RenameLoc> RenameLocs,
                                   StringRef NewName, SourceFile &SF,
                                   DiagnosticEngine &Diags) {
  SourceManager &SM = SF.getASTContext().SourceMgr;
  unsigned BufferID = SF.getBufferID().value();

  std::vector<SourceLoc> UnresolvedLocs;
  for (const RenameLoc &RenameLoc : RenameLocs) {
    DeclNameViewer OldName(RenameLoc.OldName);
    SourceLoc Location =
        SM.getLocForLineCol(BufferID, RenameLoc.Line, RenameLoc.Column);

    if (!OldName.isValid()) {
      Diags.diagnose(Location, diag::invalid_name, RenameLoc.OldName);
      return {};
    }

    if (!NewName.empty()) {
      DeclNameViewer NewDeclName(NewName);
      ArrayRef<StringRef> ParamNames = NewDeclName.args();
      bool newOperator = Lexer::isOperator(NewDeclName.base());
      bool NewNameIsValid =
          NewDeclName.isValid() &&
          (Lexer::isIdentifier(NewDeclName.base()) || newOperator) &&
          std::all_of(ParamNames.begin(), ParamNames.end(),
                      [](StringRef Label) {
                        return Label.empty() || Lexer::isIdentifier(Label);
                      });

      if (!NewNameIsValid) {
        Diags.diagnose(Location, diag::invalid_name, NewName);
        return {};
      }

      if (NewDeclName.partsCount() != OldName.partsCount()) {
        Diags.diagnose(Location, diag::arity_mismatch, NewName,
                       RenameLoc.OldName);
        return {};
      }

      if (RenameLoc.Usage == RenameLocUsage::Call && !OldName.isFunction()) {
        Diags.diagnose(Location, diag::name_not_functionlike, NewName);
        return {};
      }
    }

    UnresolvedLocs.push_back({Location});
  }

  assert(UnresolvedLocs.size() == RenameLocs.size());

  std::vector<ResolvedLoc> resolvedLocsInSourceOrder =
      runNameMatcher(SF, UnresolvedLocs);

  // Callers need to corrolate the `ResolvedLoc` with the `RenameLoc` that they
  // originated from. Match them.
  std::vector<ResolvedAndRenameLoc> resolvedAndRenameLocs;
  for (auto [unresolvedLoc, renameLoc] :
       llvm::zip_equal(UnresolvedLocs, RenameLocs)) {
    auto found = llvm::find_if(
        resolvedLocsInSourceOrder,
        [unresolvedLoc = unresolvedLoc](const ResolvedLoc &resolved) {
          return resolved.range.getStart() == unresolvedLoc;
        });
    ResolvedLoc resolvedLoc;
    if (found == resolvedLocsInSourceOrder.end()) {
      resolvedLoc =
          ResolvedLoc(CharSourceRange(),
                      /*LabelRanges=*/{}, std::nullopt, LabelRangeType::None,
                      /*IsActive=*/true, ResolvedLocContext::Comment);
    } else {
      resolvedLoc = *found;
    }
    resolvedAndRenameLocs.push_back({renameLoc, resolvedLoc});
  }
  return resolvedAndRenameLocs;
}
#endif

CancellableResult<std::vector<SyntacticRenameRangeDetails>>
nextcode::ide::findSyntacticRenameRanges(SourceFile *SF,
                                      ArrayRef<RenameLoc> RenameLocs,
                                      StringRef NewName) {
  using ResultType =
      CancellableResult<std::vector<SyntacticRenameRangeDetails>>;
#if !NEXTCODE_BUILD_NEXTCODE_SYNTAX
  return ResultType::failure("find-syntactic-rename-ranges is not supported because sourcekitd was built without nextcode-syntax");
#else
  assert(SF && "null source file");

  SourceManager &SM = SF->getASTContext().SourceMgr;
  DiagnosticEngine DiagEngine(SM);
  std::string ErrBuffer;
  llvm::raw_string_ostream DiagOS(ErrBuffer);
  nextcode::PrintingDiagnosticConsumer DiagConsumer(DiagOS);
  DiagEngine.addConsumer(DiagConsumer);

  auto ResolvedAndRenameLocs =
      resolveRenameLocations(RenameLocs, NewName, *SF, DiagEngine);
  if (ResolvedAndRenameLocs.size() != RenameLocs.size() ||
      DiagConsumer.didErrorOccur())
    return ResultType::failure(ErrBuffer);

  std::vector<SyntacticRenameRangeDetails> Result;
  for (const ResolvedAndRenameLoc &Loc : ResolvedAndRenameLocs) {
    SyntacticRenameRangeDetails Details = getSyntacticRenameRangeDetails(
        SM, Loc.renameLoc.OldName, Loc.resolved, Loc.renameLoc);
    if (Details.Type == RegionType::Mismatch) {
      DiagEngine.diagnose(Loc.resolved.range.getStart(),
                          diag::mismatched_rename, NewName);
      Result.emplace_back(SyntacticRenameRangeDetails{Details.Type, {}});
    } else {
      Result.push_back(Details);
    }
  }

  if (DiagConsumer.didErrorOccur())
    return ResultType::failure(ErrBuffer);

  return ResultType::success(Result);
#endif
}
