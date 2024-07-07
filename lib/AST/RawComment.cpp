//===--- RawComment.cpp - Extraction of raw comments ----------------------===//
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
///
/// \file
/// This file implements extraction of raw comments.
///
//===----------------------------------------------------------------------===//

#include "nextcode/AST/RawComment.h"
#include "nextcode/AST/Comment.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/PrettyStackTrace.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/AST/Types.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Defer.h"
#include "nextcode/Basic/PrimitiveParsing.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Markup/Markup.h"
#include "nextcode/Parse/Lexer.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

static SingleRawComment::CommentKind getCommentKind(StringRef Comment) {
  assert(Comment.size() >= 2);
  assert(Comment[0] == '/');

  if (Comment[1] == '/') {
    if (Comment.size() < 3)
      return SingleRawComment::CommentKind::OrdinaryLine;

    if (Comment[2] == '/') {
      return SingleRawComment::CommentKind::LineDoc;
    }
    return SingleRawComment::CommentKind::OrdinaryLine;
  } else {
    assert(Comment[1] == '*');
    assert(Comment.size() >= 4);
    if (Comment[2] == '*') {
      return SingleRawComment::CommentKind::BlockDoc;
    }
    return SingleRawComment::CommentKind::OrdinaryBlock;
  }
}

SingleRawComment::SingleRawComment(CharSourceRange Range,
                                   const SourceManager &SourceMgr)
    : Range(Range), RawText(SourceMgr.extractText(Range)),
      Kind(static_cast<unsigned>(getCommentKind(RawText))) {
  ColumnIndent = SourceMgr.getLineAndColumnInBuffer(Range.getStart()).second;
}

SingleRawComment::SingleRawComment(StringRef RawText, unsigned ColumnIndent)
    : RawText(RawText), Kind(static_cast<unsigned>(getCommentKind(RawText))),
      ColumnIndent(ColumnIndent) {}

/// Converts a range of comments (ordinary or doc) to a \c RawComment with
/// only the last range of doc comments. Gyb comments, ie. "// ###" are skipped
/// entirely as if they did not exist (so two doc comments would still be
/// merged if there was a gyb comment inbetween).
static RawComment toRawComment(ASTContext &Context, CharSourceRange Range) {
  if (Range.isInvalid())
    return RawComment();

  auto &SM = Context.SourceMgr;
  unsigned BufferID = SM.findBufferContainingLoc(Range.getStart());
  unsigned Offset = SM.getLocOffsetInBuffer(Range.getStart(), BufferID);
  unsigned EndOffset = SM.getLocOffsetInBuffer(Range.getEnd(), BufferID);
  LangOptions FakeLangOpts;
  Lexer L(FakeLangOpts, SM, BufferID, nullptr, LexerMode::NeXTCode,
          HashbangMode::Disallowed, CommentRetentionMode::ReturnAsTokens,
          Offset, EndOffset);

  SmallVector<SingleRawComment, 16> Comments;
  Token Tok;
  unsigned LastEnd = 0;
  while (true) {
    L.lex(Tok);
    if (Tok.is(tok::eof))
      break;
    assert(Tok.is(tok::comment));

    auto SRC = SingleRawComment(Tok.getRange(), SM);
    unsigned Start =
        SM.getLineAndColumnInBuffer(Tok.getRange().getStart()).first;
    unsigned End = SM.getLineAndColumnInBuffer(Tok.getRange().getEnd()).first;
    if (SRC.RawText.back() == '\n')
      End--;

    if (SRC.isOrdinary()) {
      // If there's a regular comment just skip it
      LastEnd = End;
      continue;
    }

    // Merge and keep the *last* group, ie. if there's:
    //   <comment1>
    //   <whitespace>
    //   <comment2>
    //   <comment3>
    // Only keep <comment2/3> and group into the one RawComment.

    if (!Comments.empty() && Start > LastEnd + 1)
      Comments.clear();
    Comments.push_back(SRC);

    LastEnd = End;
  }

  RawComment Result;
  Result.Comments = Context.AllocateCopy(Comments);
  return Result;
}

RawComment RawCommentRequest::evaluate(Evaluator &eval, const Decl *D) const {
  auto *DC = D->getDeclContext();
  auto &ctx = DC->getASTContext();

  // Check the declaration itself.
  if (auto *Attr = D->getAttrs().getAttribute<RawDocCommentAttr>())
    return toRawComment(ctx, Attr->getCommentRange());

  auto *Unit = dyn_cast<FileUnit>(DC->getModuleScopeContext());
  if (!Unit)
    return RawComment();

  switch (Unit->getKind()) {
  case FileUnitKind::SerializedAST: {
    // First check to see if we have the comment location available in the
    // nextcodesourceinfo, allowing us to grab it from the original file.
    auto *CachedLocs = D->getSerializedLocs();
    if (!CachedLocs->DocRanges.empty()) {
      SmallVector<SingleRawComment, 4> SRCs;
      for (const auto &Range : CachedLocs->DocRanges) {
        if (Range.isValid()) {
          SRCs.push_back({Range, ctx.SourceMgr});
        } else {
          // if we've run into an invalid range, don't bother trying to load
          // any of the other comments
          SRCs.clear();
          break;
        }
      }

      if (!SRCs.empty())
        return RawComment(ctx.AllocateCopy(llvm::ArrayRef(SRCs)));
    }

    // Otherwise check to see if we have a comment available in the nextcodedoc.
    if (auto C = Unit->getCommentForDecl(D))
      return C->Raw;

    return RawComment();
  }
  case FileUnitKind::Source:
  case FileUnitKind::Builtin:
  case FileUnitKind::Synthesized:
  case FileUnitKind::ClangModule:
  case FileUnitKind::DWARFModule:
    return RawComment();
  }
  llvm_unreachable("invalid file kind");
}

RawComment Decl::getRawComment() const {
  if (!this->canHaveComment())
    return RawComment();

  auto &eval = getASTContext().evaluator;
  return evaluateOrDefault(eval, RawCommentRequest{this}, RawComment());
}

std::optional<StringRef> Decl::getGroupName() const {
  if (hasClangNode())
    return std::nullopt;
  if (auto *Unit =
          dyn_cast<FileUnit>(getDeclContext()->getModuleScopeContext())) {
    // We can only get group information from deserialized module files.
    return Unit->getGroupNameForDecl(this);
  }
  return std::nullopt;
}

std::optional<StringRef> Decl::getSourceFileName() const {
  if (hasClangNode())
    return std::nullopt;
  if (auto *Unit =
          dyn_cast<FileUnit>(getDeclContext()->getModuleScopeContext())) {
    // We can only get group information from deserialized module files.
    return Unit->getSourceFileNameForDecl(this);
  }
  return std::nullopt;
}

std::optional<unsigned> Decl::getSourceOrder() const {
  if (hasClangNode())
    return std::nullopt;
  if (auto *Unit =
      dyn_cast<FileUnit>(this->getDeclContext()->getModuleScopeContext())) {
    // We can only get source orders from deserialized module files.
    return Unit->getSourceOrderForDecl(this);
  }
  return std::nullopt;
}

CharSourceRange RawComment::getCharSourceRange() {
  if (this->isEmpty()) {
    return CharSourceRange();
  }

  auto Start = this->Comments.front().Range.getStart();
  if (Start.isInvalid()) {
    return CharSourceRange();
  }
  auto End = this->Comments.back().Range.getEnd();
  auto Length = static_cast<const char *>(End.getOpaquePointerValue()) -
                static_cast<const char *>(Start.getOpaquePointerValue());
  return CharSourceRange(Start, Length);
}

static bool hasDoubleUnderscore(const Decl *D) {
  // Exclude decls with double-underscored names, either in arguments or
  // base names.
  static StringRef Prefix = "__";

  // If it's a function or subscript with a parameter with leading
  // double underscore, it's a private function or subscript.
  if (isa<AbstractFunctionDecl>(D) || isa<SubscriptDecl>(D)) {
    auto *params = getParameterList(cast<ValueDecl>(const_cast<Decl *>(D)));
    if (params->hasInternalParameter(Prefix))
      return true;
  }

  if (auto *VD = dyn_cast<ValueDecl>(D)) {
    auto Name = VD->getBaseName();
    if (!Name.isSpecial() && Name.getIdentifier().str().starts_with(Prefix)) {
      return true;
    }
  }
  return false;
}

static DocCommentSerializationTarget
getDocCommentSerializationTargetImpl(const Decl *D) {
  if (auto *ED = dyn_cast<ExtensionDecl>(D)) {
    auto *extended = ED->getExtendedNominal();
    if (!extended)
      return DocCommentSerializationTarget::None;

    return getDocCommentSerializationTargetFor(extended);
  }
  auto *VD = dyn_cast<ValueDecl>(D);
  if (!VD)
    return DocCommentSerializationTarget::None;

  // The use of getEffectiveAccess is unusual here; we want to take the
  // testability state into account and emit documentation if and only if they
  // are visible to clients (which means public ordinarily, but public+internal
  // when testing enabled).
  switch (VD->getEffectiveAccess()) {
  case AccessLevel::Private:
  case AccessLevel::FilePrivate:
  case AccessLevel::Internal:
    // There's no point serializing anything internal or below, as they are not
    // accessible outside their defining module.
    return DocCommentSerializationTarget::None;
  case AccessLevel::Package:
    // Package doc comments can be referenced outside their module, but only
    // locally, so can't be included in nextcodedoc.
    return DocCommentSerializationTarget::SourceInfoOnly;
  case AccessLevel::Public:
  case AccessLevel::Open:
    return DocCommentSerializationTarget::NeXTCodeDocAndSourceInfo;
  }
  llvm_unreachable("Unhandled case in switch!");
}

DocCommentSerializationTarget
nextcode::getDocCommentSerializationTargetFor(const Decl *D) {
  auto Limit = DocCommentSerializationTarget::NeXTCodeDocAndSourceInfo;

  // We can't include SPI decls in nextcodedoc.
  if (D->isSPI())
    Limit = DocCommentSerializationTarget::SourceInfoOnly;

  // .codedoc doesn't include comments for double underscored symbols, but
  // for .codesourceinfo, having the source location for these symbols isn't
  // a concern because these symbols are in .codeinterface anyway.
  if (hasDoubleUnderscore(D))
    Limit = DocCommentSerializationTarget::SourceInfoOnly;

  auto Result = getDocCommentSerializationTargetImpl(D);
  return std::min(Result, Limit);
}