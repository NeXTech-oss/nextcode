//===--- SyntaxModel.h - Routines for IDE syntax model  ---------*- C++ -*-===//
//
// This source file is part of the Code Programming Language open source project
//
// Copyright (c) 2023 ITGSS Technologies and the Code project authors
// Licensed under Apache License v2.0 with Runtime Library Exception
//
// See code/LICENSE.txt for license information
// See code/CONTRIBUTORS.txt for the list of Code project authors
//
//===----------------------------------------------------------------------===//

#ifndef CODE_IDE_SYNTAX_MODEL_H
#define CODE_IDE_SYNTAX_MODEL_H

#include "code/AST/Attr.h"
#include "code/Basic/SourceLoc.h"
#include "code/Config.h"

#include <vector>

namespace code {
  class Decl;
  class ModuleDecl;
  class SourceFile;

namespace ide {

enum class SyntaxNodeKind : uint8_t {
  Keyword,
  Identifier,
  Operator,
  DollarIdent,
  Integer,
  Floating,
  String,
  /// Marks the parens for a string interpolation.
  StringInterpolationAnchor,
  CommentLine,
  CommentBlock,
  DocCommentLine,
  DocCommentBlock,
  DocCommentField,
  /// A marker like 'FIXME:' or 'TODO:' inside a comment.
  CommentMarker,
  CommentURL,
  TypeId,
  /// #if/#else/#endif occurrence.
  BuildConfigKeyword,
  /// An identifier in a #if condition.
  BuildConfigId,
  /// #-keywords like #warning, #sourceLocation
  PoundDirectiveKeyword,
  /// Any occurrence of '@<attribute-name>' anywhere.
  AttributeId,
  /// A "resolved/active" attribute. Mis-applied attributes will be AttributeId.
  AttributeBuiltin,
  /// An editor placeholder string <#like this#>.
  EditorPlaceholder,
  /// An editor object literal [#like(this)#]
  ObjectLiteral
};

struct SyntaxNode {
  SyntaxNodeKind Kind;
  CharSourceRange Range;

  SyntaxNode(SyntaxNodeKind Kind, CharSourceRange Range)
    : Kind(Kind), Range(Range) { }

  bool isComment() const {
    return Kind == SyntaxNodeKind::CommentLine ||
           Kind == SyntaxNodeKind::CommentBlock;
  }

  bool isInnerCommentNode() const {
    return Kind == SyntaxNodeKind::CommentMarker ||
           Kind == SyntaxNodeKind::CommentURL;
  }
};

enum class SyntaxStructureKind : uint8_t {
  Class,
  Struct,
  Protocol,
  Enum,
  Extension,
  FreeFunction,
  InstanceFunction,
  StaticFunction,
  ClassFunction,
  GlobalVariable,
  InstanceVariable,
  StaticVariable,
  ClassVariable,
  LocalVariable,
  EnumCase,
  EnumElement,
  TypeAlias,
  Subscript,
  AssociatedType,
  GenericTypeParam,

  ForEachStatement,
  WhileStatement,
  RepeatWhileStatement,
  IfStatement,
  GuardStatement,
  SwitchStatement,
  CaseStatement,
  Parameter,
  Argument,
  BraceStatement,
  CallExpression,
  ArrayExpression,
  DictionaryExpression,
  ObjectLiteralExpression,
  TupleExpression,
  ClosureExpression
};

enum class SyntaxStructureElementKind : uint8_t {
  Id,
  Expr,
  InitExpr,
  ConditionExpr,
  Pattern,
  TypeRef,
};

struct SyntaxStructureElement {
  SyntaxStructureElementKind Kind;
  CharSourceRange Range;

  SyntaxStructureElement(SyntaxStructureElementKind Kind, CharSourceRange Range)
    : Kind(Kind), Range(Range) { }
};

struct SyntaxStructureNode {
  const Decl *Dcl = nullptr;
  SyntaxStructureKind Kind;
  OrigDeclAttributes Attrs;
  CharSourceRange Range;
  CharSourceRange BodyRange;
  CharSourceRange NameRange;
  CharSourceRange TypeRange;
  CharSourceRange DocRange;
  std::vector<CharSourceRange> InheritedTypeRanges;
  std::vector<SyntaxStructureElement> Elements;

  bool hasSubstructure() const {
    switch (Kind) {
    case SyntaxStructureKind::GlobalVariable:
    case SyntaxStructureKind::InstanceVariable:
    case SyntaxStructureKind::StaticVariable:
    case SyntaxStructureKind::ClassVariable:
    case SyntaxStructureKind::LocalVariable:
    case SyntaxStructureKind::Parameter:
    case SyntaxStructureKind::Subscript:
      return true;
    default:
      return false;
    }
  }
};

class SyntaxModelWalker {
  virtual void anchor();

public:
  virtual ~SyntaxModelWalker() {}

  /// Called when first visiting a syntax node, before walking into its
  /// children.  If it returns false, the subtree is skipped.
  ///
  virtual bool walkToNodePre(SyntaxNode Node) { return true; }

  /// Called after visiting the children of a syntax node. If it returns
  /// false, the remaining traversal is terminated and returns failure.
  virtual bool walkToNodePost(SyntaxNode Node) { return true; }

  /// Called when first visiting a sub-structure node, before walking
  /// into its children. If it returns false, the subtree is skipped.
  ///
  virtual bool walkToSubStructurePre(SyntaxStructureNode Node) { return true; }

  /// Called after visiting the children of a sub-structure node. If it
  /// returns false, the remaining traversal is terminated and returns failure.
  ///
  virtual bool walkToSubStructurePost(SyntaxStructureNode Node) { return true; }
};

class SyntaxModelContext {
  struct Implementation;
  Implementation &Impl;

public:
  explicit SyntaxModelContext(SourceFile &SrcFile);
  ~SyntaxModelContext();

  bool walk(SyntaxModelWalker &Walker);
};

} // namespace ide
} // namespace code

#endif // CODE_IDE_SYNTAX_MODEL_H

