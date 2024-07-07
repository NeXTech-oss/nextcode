//===--- DocComment.cpp - Extraction of doc comments ----------------------===//
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
/// This file implements extraction of documentation comments from a NeXTCode
/// Markup AST tree.
///
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Comment.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/AST/Types.h"
#include "nextcode/AST/PrettyStackTrace.h"
#include "nextcode/AST/RawComment.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Markup/Markup.h"
#include <queue>

using namespace nextcode;

void *DocComment::operator new(size_t Bytes, nextcode::markup::MarkupContext &MC,
                               unsigned Alignment) {
  return MC.allocate(Bytes, Alignment);
}

namespace {
std::optional<nextcode::markup::ParamField *>
extractParamOutlineItem(nextcode::markup::MarkupContext &MC,
                        nextcode::markup::MarkupASTNode *Node) {

  auto Item = dyn_cast<nextcode::markup::Item>(Node);
  if (!Item)
    return std::nullopt;

  auto Children = Item->getChildren();
  if (Children.empty())
    return std::nullopt;

  auto FirstChild = Children.front();
  auto FirstParagraph = dyn_cast<nextcode::markup::Paragraph>(FirstChild);
  if (!FirstParagraph)
    return std::nullopt;

  auto FirstParagraphChildren = FirstParagraph->getChildren();
  if (FirstParagraphChildren.empty())
    return std::nullopt;

  auto ParagraphText =
      dyn_cast<nextcode::markup::Text>(FirstParagraphChildren.front());
  if (!ParagraphText)
    return std::nullopt;

  StringRef Name;
  StringRef Remainder;
  std::tie(Name, Remainder) = ParagraphText->getLiteralContent().split(':');
  Name = Name.rtrim();

  if (Name.empty())
    return std::nullopt;

  ParagraphText->setLiteralContent(Remainder.ltrim());

  return nextcode::markup::ParamField::create(MC, Name, Children);
}

bool extractParameterOutline(
    nextcode::markup::MarkupContext &MC, nextcode::markup::List *L,
    SmallVectorImpl<nextcode::markup::ParamField *> &ParamFields) {
  SmallVector<nextcode::markup::MarkupASTNode *, 8> NormalItems;
  auto Children = L->getChildren();
  if (Children.empty())
    return false;

  for (auto Child : Children) {
    auto I = dyn_cast<nextcode::markup::Item>(Child);
    if (!I) {
      NormalItems.push_back(Child);
      continue;
    }

    auto ItemChildren = I->getChildren();
    if (ItemChildren.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    auto FirstChild = ItemChildren.front();
    auto FirstParagraph = dyn_cast<nextcode::markup::Paragraph>(FirstChild);
    if (!FirstParagraph) {
      NormalItems.push_back(Child);
      continue;
    }

    auto FirstParagraphChildren = FirstParagraph->getChildren();
    if (FirstParagraphChildren.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    auto HeadingText
        = dyn_cast<nextcode::markup::Text>(FirstParagraphChildren.front());
    if (!HeadingText) {
      NormalItems.push_back(Child);
      continue;
    }

    auto HeadingContent = HeadingText->getLiteralContent();
    if (!HeadingContent.rtrim().equals_insensitive("parameters:")) {
      NormalItems.push_back(Child);
      continue;
    }

    auto Rest = ArrayRef<nextcode::markup::MarkupASTNode *>(
        ItemChildren.begin() + 1, ItemChildren.end());
    if (Rest.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    for (auto Child : Rest) {
      auto SubList = dyn_cast<nextcode::markup::List>(Child);
      if (!SubList)
        continue;

      for (auto SubListChild : SubList->getChildren()) {
        auto Param = extractParamOutlineItem(MC, SubListChild);
        if (Param.has_value()) {
          ParamFields.push_back(Param.value());
        }
      }
    }
  }

  if (NormalItems.size() != Children.size()) {
    L->setChildren(NormalItems);
  }

  return NormalItems.empty();
}

bool extractSeparatedParams(
    nextcode::markup::MarkupContext &MC, nextcode::markup::List *L,
    SmallVectorImpl<nextcode::markup::ParamField *> &ParamFields) {
  SmallVector<nextcode::markup::MarkupASTNode *, 8> NormalItems;
  auto Children = L->getChildren();

  for (auto Child : Children) {
    auto I = dyn_cast<nextcode::markup::Item>(Child);
    if (!I) {
      NormalItems.push_back(Child);
      continue;
    }

    auto ItemChildren = I->getChildren();
    if (ItemChildren.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    auto FirstChild = ItemChildren.front();
    auto FirstParagraph = dyn_cast<nextcode::markup::Paragraph>(FirstChild);
    if (!FirstParagraph) {
      NormalItems.push_back(Child);
      continue;
    }

    auto FirstParagraphChildren = FirstParagraph->getChildren();
    if (FirstParagraphChildren.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    auto ParagraphText
        = dyn_cast<nextcode::markup::Text>(FirstParagraphChildren.front());
    if (!ParagraphText) {
      NormalItems.push_back(Child);
      continue;
    }

    StringRef ParameterPrefix("parameter ");
    auto ParagraphContent = ParagraphText->getLiteralContent();
    auto PotentialMatch = ParagraphContent.substr(0, ParameterPrefix.size());

    if (!PotentialMatch.starts_with_insensitive(ParameterPrefix)) {
      NormalItems.push_back(Child);
      continue;
    }

    ParagraphContent = ParagraphContent.substr(ParameterPrefix.size());
    ParagraphText->setLiteralContent(ParagraphContent.ltrim());

    auto ParamField = extractParamOutlineItem(MC, I);
    if (ParamField.has_value())
      ParamFields.push_back(ParamField.value());
    else
      NormalItems.push_back(Child);
  }

  if (NormalItems.size() != Children.size())
    L->setChildren(NormalItems);

  return NormalItems.empty();
}

bool extractSimpleField(
    nextcode::markup::MarkupContext &MC, nextcode::markup::List *L,
    nextcode::markup::CommentParts &Parts,
    SmallVectorImpl<const nextcode::markup::MarkupASTNode *> &BodyNodes) {
  auto Children = L->getChildren();
  SmallVector<nextcode::markup::MarkupASTNode *, 8> NormalItems;
  for (auto Child : Children) {
    auto I = dyn_cast<nextcode::markup::Item>(Child);
    if (!I) {
      NormalItems.push_back(Child);
      continue;
    }

    auto ItemChildren = I->getChildren();
    if (ItemChildren.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    auto FirstParagraph
        = dyn_cast<nextcode::markup::Paragraph>(ItemChildren.front());
    if (!FirstParagraph) {
      NormalItems.push_back(Child);
      continue;
    }

    auto ParagraphChildren = FirstParagraph->getChildren();
    if (ParagraphChildren.empty()) {
      NormalItems.push_back(Child);
      continue;
    }

    auto ParagraphText
        = dyn_cast<nextcode::markup::Text>(ParagraphChildren.front());
    if (!ParagraphText) {
      NormalItems.push_back(Child);
      continue;
    }

    StringRef Tag;
    StringRef Remainder;
    std::tie(Tag, Remainder) = ParagraphText->getLiteralContent().split(':');
    Tag = Tag.ltrim().rtrim();
    Remainder = Remainder.ltrim();

    if (!nextcode::markup::isAFieldTag(Tag)) {
      NormalItems.push_back(Child);
      continue;
    }

    ParagraphText->setLiteralContent(Remainder);
    auto Field = nextcode::markup::createSimpleField(MC, Tag, ItemChildren);

    if (auto RF = dyn_cast<nextcode::markup::ReturnsField>(Field)) {
      Parts.ReturnsField = RF;
    } else if (auto TF = dyn_cast<nextcode::markup::ThrowsField>(Field)) {
      Parts.ThrowsField = TF;
    } else if (auto TF = dyn_cast<nextcode::markup::TagField>(Field)) {
      llvm::SmallString<64> Scratch;
      llvm::raw_svector_ostream OS(Scratch);
      printInlinesUnder(TF, OS);
      Parts.Tags.insert(MC.allocateCopy(OS.str()));
    } else if (auto LKF = dyn_cast<markup::LocalizationKeyField>(Field)) {
      Parts.LocalizationKeyField = LKF;
    } else {
      BodyNodes.push_back(Field);
    }
  }

  if (NormalItems.size() != Children.size())
    L->setChildren(NormalItems);

  return NormalItems.empty();
}
} // namespace

void nextcode::printBriefComment(RawComment RC, llvm::raw_ostream &OS) {
  markup::MarkupContext MC;
  markup::LineList LL = MC.getLineList(RC);
  auto *markupDoc = markup::parseDocument(MC, LL);

  auto children = markupDoc->getChildren();
  if (children.empty())
    return;
  auto FirstParagraph = dyn_cast<nextcode::markup::Paragraph>(children.front());
  if (!FirstParagraph)
    return;
  nextcode::markup::printInlinesUnder(FirstParagraph, OS);
}

nextcode::markup::CommentParts
nextcode::extractCommentParts(nextcode::markup::MarkupContext &MC,
                    nextcode::markup::MarkupASTNode *Node) {

  nextcode::markup::CommentParts Parts;
  auto Children = Node->getChildren();
  if (Children.empty())
    return Parts;

  auto FirstParagraph
      = dyn_cast<nextcode::markup::Paragraph>(Node->getChildren().front());
  if (FirstParagraph)
    Parts.Brief = FirstParagraph;

  SmallVector<const nextcode::markup::MarkupASTNode *, 4> BodyNodes;
  SmallVector<nextcode::markup::ParamField *, 8> ParamFields;

  // Look for special top-level lists
  size_t StartOffset = FirstParagraph == nullptr ? 0 : 1;
  for (auto C = Children.begin() + StartOffset; C != Children.end(); ++C) {
    if (auto L = dyn_cast<nextcode::markup::List>(*C)) {
      // Could be one of the following:
      // 1. A parameter outline:
      //    - Parameters:
      //      - x: ...
      //      - y: ...
      // 2. An exploded parameter list:
      //    - parameter x: ...
      //    - parameter y: ...
      // 3. Some other simple field, including "returns" (see SimpleFields.def)
      auto ListNowEmpty = extractParameterOutline(MC, L, ParamFields);
      ListNowEmpty |= extractSeparatedParams(MC, L, ParamFields);
      ListNowEmpty |= extractSimpleField(MC, L, Parts, BodyNodes);
      if (ListNowEmpty)
        continue; // This drops the empty list from the doc comment body.
    }
    BodyNodes.push_back(*C);
  }

  // Copy BodyNodes and ParamFields into the MarkupContext.
  Parts.BodyNodes = MC.allocateCopy(llvm::ArrayRef(BodyNodes));
  Parts.ParamFields = MC.allocateCopy(llvm::ArrayRef(ParamFields));

  for (auto Param : Parts.ParamFields) {
    auto ParamParts = extractCommentParts(MC, Param);
    Param->setParts(ParamParts);
  }

  return Parts;
}

DocComment *DocComment::create(const Decl *D, markup::MarkupContext &MC,
                               RawComment RC) {
  assert(!RC.isEmpty());
  nextcode::markup::LineList LL = MC.getLineList(RC);
  auto *Doc = nextcode::markup::parseDocument(MC, LL);
  auto Parts = extractCommentParts(MC, Doc);
  return new (MC) DocComment(D, Doc, Parts);
}

void DocComment::addInheritanceNote(nextcode::markup::MarkupContext &MC,
                                    TypeDecl *base) {
  auto text = MC.allocateCopy("This documentation comment was inherited from ");
  auto name = MC.allocateCopy(base->getNameStr());
  auto period = MC.allocateCopy(".");
  auto paragraph = markup::Paragraph::create(MC, {
    markup::Text::create(MC, text),
    markup::Code::create(MC, name),
    markup::Text::create(MC, period)});

  auto note = markup::NoteField::create(MC, {paragraph});

  SmallVector<const markup::MarkupASTNode *, 8> BodyNodes{
    Parts.BodyNodes.begin(), Parts.BodyNodes.end()};
  BodyNodes.push_back(note);
  Parts.BodyNodes = MC.allocateCopy(llvm::ArrayRef(BodyNodes));
}

DocComment *nextcode::getSingleDocComment(nextcode::markup::MarkupContext &MC,
                                       const Decl *D) {
  PrettyStackTraceDecl StackTrace("parsing comment for", D);

  auto RC = D->getRawComment();
  if (RC.isEmpty())
    return nullptr;
  return DocComment::create(D, MC, RC);
}

namespace {
/// Helper class for finding the comment providing decl for either a brief or
/// raw comment.
template <typename Result>
class CommentProviderFinder final {
  using ResultWithDecl = std::pair<Result, const Decl *>;
  using VisitFnTy = std::optional<Result> (*)(const Decl *);

  VisitFnTy VisitFn;

public:
  CommentProviderFinder(VisitFnTy visitFn) : VisitFn(visitFn) {}

private:
  std::optional<ResultWithDecl> visit(const Decl *D) {
    // Adapt the provided visitor function to also return the decl.
    if (auto result = VisitFn(D))
      return {{*result, D}};
    return std::nullopt;
  }

  std::optional<ResultWithDecl> findOverriddenDecl(const ValueDecl *VD) {
    // Only applies to class member.
    if (!VD->getDeclContext()->getSelfClassDecl())
      return std::nullopt;

    while (auto *baseDecl = VD->getOverriddenDecl()) {
      if (auto result = visit(baseDecl))
        return result;

      VD = baseDecl;
    }
    return std::nullopt;
  }

  std::optional<ResultWithDecl> findDefaultProvidedDecl(const ValueDecl *VD) {
    // Only applies to protocol extension member.
    auto *protocol = VD->getDeclContext()->getExtendedProtocolDecl();
    if (!protocol)
      return std::nullopt;

    SmallVector<ValueDecl *, 2> members;
    protocol->lookupQualified(const_cast<ProtocolDecl *>(protocol),
                              DeclNameRef(VD->getName()),
                              VD->getLoc(), NLOptions::NL_ProtocolMembers,
                              members);

    std::optional<ResultWithDecl> result;
    for (auto *member : members) {
      if (!isa<ProtocolDecl>(member->getDeclContext()) ||
          !member->isProtocolRequirement())
        continue;

      auto newResult = visit(member);
      if (!newResult)
        continue;

      if (result) {
        // Found two or more decls with doc-comment.
        return std::nullopt;
      }
      result = newResult;
    }
    return result;
  }

  std::optional<ResultWithDecl> findRequirementDecl(const ValueDecl *VD) {
    std::queue<const ValueDecl *> requirements;
    while (true) {
      for (auto *req : VD->getSatisfiedProtocolRequirements()) {
        if (auto result = visit(req))
          return result;

        requirements.push(req);
      }
      if (requirements.empty())
        return std::nullopt;

      VD = requirements.front();
      requirements.pop();
    }
  }

public:
  std::optional<ResultWithDecl> findCommentProvider(const Decl *D) {
    if (auto result = visit(D))
      return result;

    auto *VD = dyn_cast<ValueDecl>(D);
    if (!VD)
      return std::nullopt;

    if (auto result = findOverriddenDecl(VD))
      return result;

    if (auto result = findDefaultProvidedDecl(VD))
      return result;

    if (auto result = findRequirementDecl(VD))
      return result;

    return std::nullopt;
  }
};
} // end anonymous namespace

const Decl *nextcode::getDocCommentProvidingDecl(const Decl *D) {
  // Search for the first decl we see with a non-empty raw comment.
  auto finder = CommentProviderFinder<RawComment>(
      [](const Decl *D) -> std::optional<RawComment> {
        auto comment = D->getRawComment();
        if (comment.isEmpty())
          return std::nullopt;
        return comment;
      });

  auto result = finder.findCommentProvider(D);
  return result ? result->second : nullptr;
}

DocComment *nextcode::getCascadingDocComment(nextcode::markup::MarkupContext &MC,
                                          const Decl *D) {
  auto *docD = getDocCommentProvidingDecl(D);
  if (!docD)
    return nullptr;

  auto *doc = getSingleDocComment(MC, docD);
  assert(doc && "getDocCommentProvidingDecl() returned decl with no comment");

  // If the doc-comment is inherited from other decl, add a note about it.
  if (docD != D) {
    doc->setDecl(D);
    if (auto baseD = docD->getDeclContext()->getSelfNominalTypeDecl()) {
      doc->addInheritanceNote(MC, baseD);

      // If the doc is inherited from protocol requirement, associate the
      // requirement with the doc-comment.
      // FIXME: This is to keep the old behavior.
      if (isa<ProtocolDecl>(baseD))
        doc->setDecl(docD);
    }
  }

  return doc;
}

/// Retrieve the brief comment for a given decl \p D, without attempting to
/// walk any requirements or overrides.
static std::optional<StringRef> getDirectBriefComment(const Decl *D) {
  if (!D->canHaveComment())
    return std::nullopt;

  auto *ModuleDC = D->getDeclContext()->getModuleScopeContext();
  auto &Ctx = ModuleDC->getASTContext();

  // If we expect the comment to be in the nextcodedoc, check for it if we loaded a
  // nextcodedoc. If missing from the nextcodedoc, we know it will not be in the
  // nextcodesourceinfo either, so we can bail early.
  if (auto *Unit = dyn_cast<FileUnit>(ModuleDC)) {
    if (Unit->hasLoadedNeXTCodeDoc()) {
      auto target = getDocCommentSerializationTargetFor(D);
      if (target == DocCommentSerializationTarget::NeXTCodeDocAndSourceInfo) {
        auto C = Unit->getCommentForDecl(D);
        if (!C)
          return std::nullopt;

        return C->Brief;
      }
    }
  }

  // Otherwise, parse the brief from the raw comment itself. This will look into
  // the nextcodesourceinfo if needed.
  auto RC = D->getRawComment();
  if (RC.isEmpty())
    return std::nullopt;

  SmallString<256> BriefStr;
  llvm::raw_svector_ostream OS(BriefStr);
  printBriefComment(RC, OS);
  return Ctx.AllocateCopy(BriefStr.str());
}

StringRef SemanticBriefCommentRequest::evaluate(Evaluator &evaluator,
                                                const Decl *D) const {
  // Perform a walk over the potential providers of the brief comment,
  // retrieving the first one we come across.
  CommentProviderFinder<StringRef> finder(getDirectBriefComment);
  auto result = finder.findCommentProvider(D);
  return result ? result->first : StringRef();
}

StringRef Decl::getSemanticBriefComment() const {
  if (!this->canHaveComment())
    return StringRef();

  auto &eval = getASTContext().evaluator;
  return evaluateOrDefault(eval, SemanticBriefCommentRequest{this},
                           StringRef());
}
