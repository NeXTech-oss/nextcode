//===--- CodeCompletionStringPrinter.cpp ----------------------------------===//
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

#include "nextcode/IDE/CodeCompletionStringPrinter.h"
#include "CodeCompletionResultBuilder.h"
#include "nextcode/AST/Module.h"
#include "nextcode/Basic/Assertions.h"

using namespace nextcode;
using namespace nextcode::ide;

using ChunkKind = CodeCompletionString::Chunk::ChunkKind;

std::optional<ChunkKind>
CodeCompletionStringPrinter::getChunkKindForPrintNameContext(
    PrintNameContext context) const {
  switch (context) {
  case PrintNameContext::Keyword:
    if (isCurrentStructureKind(PrintStructureKind::EffectsSpecifiers)) {
      return ChunkKind::EffectsSpecifierKeyword;
    }
    return ChunkKind::Keyword;
  case PrintNameContext::IntroducerKeyword:
    return ChunkKind::DeclIntroducer;
  case PrintNameContext::Attribute:
    return ChunkKind::Attribute;
  case PrintNameContext::FunctionParameterExternal:
    if (isInType()) {
      return std::nullopt;
    }
    return ChunkKind::ParameterDeclExternalName;
  case PrintNameContext::FunctionParameterLocal:
    if (isInType()) {
      return std::nullopt;
    }
    return ChunkKind::ParameterDeclLocalName;
  default:
    return std::nullopt;
  }
}

std::optional<ChunkKind>
CodeCompletionStringPrinter::getChunkKindForStructureKind(
    PrintStructureKind Kind) const {
  switch (Kind) {
  case PrintStructureKind::FunctionParameter:
    if (isInType()) {
      return std::nullopt;
    }
    return ChunkKind::ParameterDeclBegin;
  case PrintStructureKind::DefaultArgumentClause:
    return ChunkKind::DefaultArgumentClauseBegin;
  case PrintStructureKind::DeclGenericParameterClause:
    return ChunkKind::GenericParameterClauseBegin;
  case PrintStructureKind::DeclGenericRequirementClause:
    return ChunkKind::GenericRequirementClauseBegin;
  case PrintStructureKind::EffectsSpecifiers:
    return ChunkKind::EffectsSpecifierClauseBegin;
  case PrintStructureKind::DeclResultTypeClause:
    return ChunkKind::DeclResultTypeClauseBegin;
  case PrintStructureKind::FunctionParameterType:
    return ChunkKind::ParameterDeclTypeBegin;
  default:
    return std::nullopt;
  }
}

void CodeCompletionStringPrinter::startNestedGroup(ChunkKind Kind) {
  flush();
  Builder.CurrentNestingLevel++;
  Builder.addSimpleChunk(Kind);
}

void CodeCompletionStringPrinter::endNestedGroup() {
  flush();
  Builder.CurrentNestingLevel--;
}

void CodeCompletionStringPrinter::flush() {
  if (Buffer.empty())
    return;
  Builder.addChunkWithText(CurrChunkKind, Buffer);
  Buffer.clear();
}

/// Start \c AttributeAndModifierListBegin group. This must be called before
/// any attributes/modifiers printed to the output when printing an override
/// compleion.
void CodeCompletionStringPrinter::startPreamble() {
  assert(!InPreamble);
  startNestedGroup(ChunkKind::AttributeAndModifierListBegin);
  InPreamble = true;
}

void CodeCompletionStringPrinter::endPremable() {
  if (!InPreamble)
    return;
  InPreamble = false;
  endNestedGroup();
}

void CodeCompletionStringPrinter::printText(StringRef Text) {
  // Detect ': ' and ', ' in parameter clauses.
  // FIXME: Is there a better way?
  if (isCurrentStructureKind(PrintStructureKind::FunctionParameter) &&
      Text == ": ") {
    setNextChunkKind(ChunkKind::ParameterDeclColon);
  } else if (isCurrentStructureKind(
                 PrintStructureKind::FunctionParameterList) &&
             Text == ", ") {
    setNextChunkKind(ChunkKind::Comma);
  }

  if (CurrChunkKind != NextChunkKind) {
    // If the next desired kind is different from the current buffer, flush
    // the current buffer.
    flush();
    CurrChunkKind = NextChunkKind;
  }
  Buffer.append(Text);
}

void CodeCompletionStringPrinter::printTypeRef(Type T, const TypeDecl *TD,
                                               Identifier Name,
                                               PrintNameContext NameContext) {

  NextChunkKind = TD->getModuleContext()->isNonUserModule()
                      ? ChunkKind::TypeIdSystem
                      : ChunkKind::TypeIdUser;

  ASTPrinter::printTypeRef(T, TD, Name, NameContext);
  NextChunkKind = ChunkKind::Text;
}

void CodeCompletionStringPrinter::printDeclLoc(const Decl *D) {
  endPremable();
  setNextChunkKind(ChunkKind::BaseName);
}

void CodeCompletionStringPrinter::printDeclNameEndLoc(const Decl *D) {
  setNextChunkKind(ChunkKind::Text);
}

void CodeCompletionStringPrinter::printNamePre(PrintNameContext context) {
  if (context == PrintNameContext::IntroducerKeyword)
    endPremable();
  if (auto Kind = getChunkKindForPrintNameContext(context))
    setNextChunkKind(*Kind);
}

void CodeCompletionStringPrinter::printNamePost(PrintNameContext context) {
  if (getChunkKindForPrintNameContext(context))
    setNextChunkKind(ChunkKind::Text);
}

void CodeCompletionStringPrinter::printTypePre(const TypeLoc &TL) {
  ++TypeDepth;
}

void CodeCompletionStringPrinter::printTypePost(const TypeLoc &TL) {
  assert(TypeDepth > 0);
  --TypeDepth;
}

void CodeCompletionStringPrinter::printStructurePre(PrintStructureKind Kind,
                                                    const Decl *D) {
  StructureStack.push_back(Kind);

  if (auto chunkKind = getChunkKindForStructureKind(Kind))
    startNestedGroup(*chunkKind);
}

void CodeCompletionStringPrinter::printStructurePost(PrintStructureKind Kind,
                                                     const Decl *D) {
  if (getChunkKindForStructureKind(Kind))
    endNestedGroup();

  assert(Kind == StructureStack.back());
  StructureStack.pop_back();
}