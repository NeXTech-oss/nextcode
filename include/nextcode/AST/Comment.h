//===--- Comment.h - NeXTCode-specific comment parsing -------------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_COMMENT_H
#define NEXTCODE_AST_COMMENT_H

#include "nextcode/Markup/Markup.h"
#include <optional>

namespace nextcode {
class Decl;
class TypeDecl;
struct RawComment;

class DocComment {
  const Decl *D;
  nextcode::markup::Document *Doc = nullptr;
  nextcode::markup::CommentParts Parts;

  DocComment(const Decl *D, nextcode::markup::Document *Doc,
             nextcode::markup::CommentParts Parts)
      : D(D), Doc(Doc), Parts(Parts) {}

public:
  static DocComment *create(const Decl *D, nextcode::markup::MarkupContext &MC,
                            RawComment RC);

  void addInheritanceNote(nextcode::markup::MarkupContext &MC, TypeDecl *base);

  const Decl *getDecl() const { return D; }
  void setDecl(const Decl *D) { this->D = D; }

  const nextcode::markup::Document *getDocument() const { return Doc; }

  nextcode::markup::CommentParts getParts() const {
    return Parts;
  }

  ArrayRef<StringRef> getTags() const {
    return llvm::ArrayRef(Parts.Tags.begin(), Parts.Tags.end());
  }

  std::optional<const nextcode::markup::Paragraph *> getBrief() const {
    return Parts.Brief;
  }

  std::optional<const nextcode::markup::ReturnsField *> getReturnsField() const {
    return Parts.ReturnsField;
  }

  std::optional<const nextcode::markup::ThrowsField *> getThrowsField() const {
    return Parts.ThrowsField;
  }

  ArrayRef<const nextcode::markup::ParamField *> getParamFields() const {
    return Parts.ParamFields;
  }

  ArrayRef<const nextcode::markup::MarkupASTNode *> getBodyNodes() const {
    return Parts.BodyNodes;
  }

  std::optional<const markup::LocalizationKeyField *>
  getLocalizationKeyField() const {
    return Parts.LocalizationKeyField;
  }

  bool isEmpty() const {
    return Parts.isEmpty();
  }

  // Only allow allocation using the allocator in MarkupContext or by
  // placement new.
  void *operator new(size_t Bytes, nextcode::markup::MarkupContext &MC,
                     unsigned Alignment = alignof(DocComment));
  void *operator new(size_t Bytes, void *Mem) {
    assert(Mem);
    return Mem;
  }

  // Make vanilla new/delete illegal.
  void *operator new(size_t Bytes) = delete;
  void operator delete(void *Data) = delete;
};

/// Get a parsed documentation comment for the declaration, if there is one.
DocComment *getSingleDocComment(nextcode::markup::MarkupContext &Context,
                                const Decl *D);

/// Get the declaration that actually provides a doc comment for another.
const Decl *getDocCommentProvidingDecl(const Decl *D);

/// Attempt to get a doc comment from the declaration, or other inherited
/// sources, like from base classes or protocols.
DocComment *getCascadingDocComment(nextcode::markup::MarkupContext &MC,
                                   const Decl *D);

/// Extract comments parts from the given Markup node.
nextcode::markup::CommentParts
extractCommentParts(nextcode::markup::MarkupContext &MC,
                    nextcode::markup::MarkupASTNode *Node);

/// Extract brief comment from \p RC, and print it to \p OS .
void printBriefComment(RawComment RC, llvm::raw_ostream &OS);

/// Describes the intended serialization target for a doc comment.
enum class DocCommentSerializationTarget : uint8_t {
  /// The doc comment should not be serialized.
  None = 0,

  /// The doc comment should only be serialized in the 'nextcodesourceinfo'.
  SourceInfoOnly,

  /// The doc comment should be serialized in both the 'nextcodedoc' and
  /// 'nextcodesourceinfo'.
  NeXTCodeDocAndSourceInfo,
};

/// Retrieve the expected serialization target for a documentation comment
/// attached to the given decl.
DocCommentSerializationTarget
getDocCommentSerializationTargetFor(const Decl *D);

} // namespace nextcode

#endif // LLVM_NEXTCODE_AST_COMMENT_H
