//===--- ASTBridging.h - header for the nextcode SILBridging module ----------===//
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

#ifndef NEXTCODE_AST_ASTBRIDGING_H
#define NEXTCODE_AST_ASTBRIDGING_H

// Do not add other C++/llvm/nextcode header files here!
// Function implementations should be placed into ASTBridging.cpp and required header files should be added there.
//
// Pure bridging mode does not permit including any C++/llvm/nextcode headers.
// See also the comments for `BRIDGING_MODE` in the top-level CMakeLists.txt file.
//
#include "nextcode/Basic/BasicBridging.h"

#ifdef USED_IN_CPP_SOURCE
#include "nextcode/AST/ArgumentList.h"
#include "nextcode/AST/Attr.h"
#include "nextcode/AST/DiagnosticConsumer.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/Stmt.h"
#endif

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

namespace nextcode {
class ASTContext;
class DiagnosticArgument;
class DiagnosticEngine;
}

class BridgedASTContext;

//===----------------------------------------------------------------------===//
// MARK: Identifier
//===----------------------------------------------------------------------===//

class BridgedIdentifier {
public:
  NEXTCODE_UNAVAILABLE("Use '.raw' instead")
  const void *_Nullable Raw;

  BridgedIdentifier() : Raw(nullptr) {}

  NEXTCODE_NAME("init(raw:)")
  BridgedIdentifier(const void *_Nullable raw) : Raw(raw) {}

#ifdef USED_IN_CPP_SOURCE
  BridgedIdentifier(nextcode::Identifier ident)
      : Raw(ident.getAsOpaquePointer()) {}

  nextcode::Identifier unbridged() const {
    return nextcode::Identifier::getFromOpaquePointer(Raw);
  }
#endif
};

NEXTCODE_NAME("getter:BridgedIdentifier.raw(self:)")
inline const void *_Nullable BridgedIdentifier_raw(BridgedIdentifier ident) {
  return ident.Raw;
}

struct BridgedLocatedIdentifier {
  NEXTCODE_NAME("name")
  BridgedIdentifier Name;

  NEXTCODE_NAME("nameLoc")
  BridgedSourceLoc NameLoc;
};

class BridgedDeclBaseName {
  BridgedIdentifier Ident;

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedDeclBaseName(nextcode::DeclBaseName baseName) : Ident(baseName.Ident) {}

  nextcode::DeclBaseName unbridged() const {
    return nextcode::DeclBaseName(Ident.unbridged());
  }
#endif
};

NEXTCODE_NAME("BridgedDeclBaseName.createConstructor()")
BridgedDeclBaseName BridgedDeclBaseName_createConstructor();

NEXTCODE_NAME("BridgedDeclBaseName.createDestructor()")
BridgedDeclBaseName BridgedDeclBaseName_createDestructor();

NEXTCODE_NAME("BridgedDeclBaseName.createSubscript()")
BridgedDeclBaseName BridgedDeclBaseName_createSubscript();

NEXTCODE_NAME("BridgedDeclBaseName.createIdentifier(_:)")
BridgedDeclBaseName
BridgedDeclBaseName_createIdentifier(BridgedIdentifier identifier);

class BridgedDeclNameRef {
  void *_Nonnull opaque;

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedDeclNameRef(nextcode::DeclNameRef name) : opaque(name.getOpaqueValue()) {}

  nextcode::DeclNameRef unbridged() const {
    return nextcode::DeclNameRef::getFromOpaqueValue(opaque);
  }
#endif
};

NEXTCODE_NAME("BridgedDeclNameRef.createParsed(_:baseName:argumentLabels:)")
BridgedDeclNameRef
BridgedDeclNameRef_createParsed(BridgedASTContext cContext,
                                BridgedDeclBaseName cBaseName,
                                BridgedArrayRef cLabels);

NEXTCODE_NAME("BridgedDeclNameRef.createParsed(_:)")
BridgedDeclNameRef
BridgedDeclNameRef_createParsed(BridgedDeclBaseName cBaseName);

class BridgedDeclNameLoc {
  const void *_Nullable LocationInfo;
  size_t NumArgumentLabels;

public:
  BridgedDeclNameLoc() : LocationInfo(nullptr), NumArgumentLabels(0) {}

#ifdef USED_IN_CPP_SOURCE
  BridgedDeclNameLoc(nextcode::DeclNameLoc loc)
      : LocationInfo(loc.LocationInfo),
        NumArgumentLabels(loc.NumArgumentLabels) {}

  nextcode::DeclNameLoc unbridged() const {
    return nextcode::DeclNameLoc(LocationInfo, NumArgumentLabels);
  }
#endif
};

NEXTCODE_NAME("BridgedDeclNameLoc.createParsed(_:baseNameLoc:lParenLoc:"
           "argumentLabelLocs:rParenLoc:)")
BridgedDeclNameLoc BridgedDeclNameLoc_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cBaseNameLoc,
    BridgedSourceLoc cLParenLoc, BridgedArrayRef cLabelLocs,
    BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME("BridgedDeclNameLoc.createParsed(_:)")
BridgedDeclNameLoc
BridgedDeclNameLoc_createParsed(BridgedSourceLoc cBaseNameLoc);

//===----------------------------------------------------------------------===//
// MARK: ASTContext
//===----------------------------------------------------------------------===//

class BridgedASTContext {
  nextcode::ASTContext * _Nonnull Ctx;

public:
#ifdef USED_IN_CPP_SOURCE
  NEXTCODE_UNAVAILABLE("Use init(raw:) instead")
  BridgedASTContext(nextcode::ASTContext &ctx) : Ctx(&ctx) {}

  NEXTCODE_UNAVAILABLE("Use '.raw' instead")
  nextcode::ASTContext &unbridged() const { return *Ctx; }
#endif
};

NEXTCODE_NAME("getter:BridgedASTContext.raw(self:)")
BRIDGED_INLINE
void * _Nonnull BridgedASTContext_raw(BridgedASTContext bridged);

NEXTCODE_NAME("BridgedASTContext.init(raw:)")
BRIDGED_INLINE
BridgedASTContext BridgedASTContext_fromRaw(void * _Nonnull ptr);

NEXTCODE_NAME("BridgedASTContext.getIdentifier(self:_:)")
BridgedIdentifier BridgedASTContext_getIdentifier(BridgedASTContext cContext,
                                                  BridgedStringRef cStr);

NEXTCODE_NAME("BridgedASTContext.langOptsHasFeature(self:_:)")
bool BridgedASTContext_langOptsHasFeature(BridgedASTContext cContext,
                                          BridgedFeature feature);

NEXTCODE_NAME("getter:BridgedASTContext.majorLanguageVersion(self:)")
unsigned BridgedASTContext_majorLanguageVersion(BridgedASTContext cContext);

//===----------------------------------------------------------------------===//
// MARK: AST nodes
//===----------------------------------------------------------------------===//

enum ENUM_EXTENSIBILITY_ATTR(open) ASTNodeKind : size_t {
  ASTNodeKindExpr,
  ASTNodeKindStmt,
  ASTNodeKindDecl
};

struct BridgedASTNode {
  NEXTCODE_NAME("raw")
  void *_Nonnull Raw;

  NEXTCODE_NAME("kind")
  ASTNodeKind Kind;

#ifdef USED_IN_CPP_SOURCE
  nextcode::ASTNode unbridged() const {
    switch (Kind) {
    case ASTNodeKindExpr:
      return nextcode::ASTNode(static_cast<nextcode::Expr *>(Raw));
    case ASTNodeKindStmt:
      return nextcode::ASTNode(static_cast<nextcode::Stmt *>(Raw));
    case ASTNodeKindDecl:
      return nextcode::ASTNode(static_cast<nextcode::Decl *>(Raw));
    }
  }
#endif
};

// Forward declare the underlying AST node type for each wrapper.
namespace nextcode {
#define AST_BRIDGING_WRAPPER(Name) class Name;
#include "nextcode/AST/ASTBridgingWrappers.def"
} // end namespace nextcode

// Define the bridging wrappers for each AST node.
#define AST_BRIDGING_WRAPPER(Name) BRIDGING_WRAPPER_NONNULL(nextcode::Name, Name)
#include "nextcode/AST/ASTBridgingWrappers.def"

// For nullable nodes, also define a nullable variant.
#define AST_BRIDGING_WRAPPER_NULLABLE(Name)                                    \
  BRIDGING_WRAPPER_NULLABLE(nextcode::Name, Name)
#define AST_BRIDGING_WRAPPER_NONNULL(Name)
#include "nextcode/AST/ASTBridgingWrappers.def"

// Declare `.asDecl` on each BridgedXXXDecl type, which upcasts a wrapper for
// a Decl subclass to a BridgedDecl.
#define DECL(Id, Parent)                                                       \
  NEXTCODE_NAME("getter:Bridged" #Id "Decl.asDecl(self:)")                        \
  BridgedDecl Bridged##Id##Decl_asDecl(Bridged##Id##Decl decl);
#define ABSTRACT_DECL(Id, Parent) DECL(Id, Parent)
#include "nextcode/AST/DeclNodes.def"

// Declare `.asDeclContext` on each BridgedXXXDecl type that's also a
// DeclContext.
#define DECL(Id, Parent)
#define CONTEXT_DECL(Id, Parent)                                               \
  NEXTCODE_NAME("getter:Bridged" #Id "Decl.asDeclContext(self:)")                 \
  BridgedDeclContext Bridged##Id##Decl_asDeclContext(Bridged##Id##Decl decl);
#define ABSTRACT_CONTEXT_DECL(Id, Parent) CONTEXT_DECL(Id, Parent)
#include "nextcode/AST/DeclNodes.def"

// Declare `.asStmt` on each BridgedXXXStmt type, which upcasts a wrapper for
// a Stmt subclass to a BridgedStmt.
#define STMT(Id, Parent)                                                       \
  NEXTCODE_NAME("getter:Bridged" #Id "Stmt.asStmt(self:)")                        \
  BridgedStmt Bridged##Id##Stmt_asStmt(Bridged##Id##Stmt stmt);
#define ABSTRACT_STMT(Id, Parent) STMT(Id, Parent)
#include "nextcode/AST/StmtNodes.def"

// Declare `.asExpr` on each BridgedXXXExpr type, which upcasts a wrapper for
// a Expr subclass to a BridgedExpr.
#define EXPR(Id, Parent)                                                       \
  NEXTCODE_NAME("getter:Bridged" #Id "Expr.asExpr(self:)")                        \
  BridgedExpr Bridged##Id##Expr_asExpr(Bridged##Id##Expr expr);
#define ABSTRACT_EXPR(Id, Parent) EXPR(Id, Parent)
#include "nextcode/AST/ExprNodes.def"

// Declare `.asTypeRepr` on each BridgedXXXTypeRepr type, which upcasts a
// wrapper for a TypeRepr subclass to a BridgedTypeRepr.
#define TYPEREPR(Id, Parent)                                                   \
  NEXTCODE_NAME("getter:Bridged" #Id "TypeRepr.asTypeRepr(self:)")                \
  BridgedTypeRepr Bridged##Id##TypeRepr_asTypeRepr(                            \
      Bridged##Id##TypeRepr typeRepr);
#define ABSTRACT_TYPEREPR(Id, Parent) TYPEREPR(Id, Parent)
#include "nextcode/AST/TypeReprNodes.def"

// Declare `.asPattern` on each BridgedXXXPattern type, which upcasts a wrapper
// for a Pattern subclass to a BridgedPattern.
#define PATTERN(Id, Parent)                                                    \
  NEXTCODE_NAME("getter:Bridged" #Id "Pattern.asPattern(self:)")                  \
  BridgedPattern Bridged##Id##Pattern_asPattern(Bridged##Id##Pattern pattern);
#include "nextcode/AST/PatternNodes.def"

// Declare `.asDeclAttribute` on each BridgedXXXAttr type, which upcasts a
// wrapper for a DeclAttribute subclass to a BridgedDeclAttribute.
#define SIMPLE_DECL_ATTR(...)
#define DECL_ATTR(_, CLASS, ...)                                               \
  NEXTCODE_NAME("getter:Bridged" #CLASS "Attr.asDeclAttribute(self:)")            \
  BridgedDeclAttribute Bridged##CLASS##Attr_asDeclAttribute(                   \
      Bridged##CLASS##Attr attr);
#include "nextcode/AST/DeclAttr.def"

struct BridgedPatternBindingEntry {
  BridgedPattern pattern;
  BridgedSourceLoc equalLoc;
  BridgedNullableExpr init;
  BridgedNullablePatternBindingInitializer initContext;
};

//===----------------------------------------------------------------------===//
// MARK: Diagnostic Engine
//===----------------------------------------------------------------------===//

// NOTE: This must be the same underlying value as C++ 'nextcode::DiagID' defined
// in 'DiagnosticList.cpp'.
enum ENUM_EXTENSIBILITY_ATTR(open) BridgedDiagID : uint32_t {
#define DIAG(KIND, ID, Options, Text, Signature) BridgedDiagID_##ID,
#include "nextcode/AST/DiagnosticsAll.def"
};

class BridgedDiagnosticArgument {
  int64_t storage[3];

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedDiagnosticArgument(const nextcode::DiagnosticArgument &arg) {
    *reinterpret_cast<nextcode::DiagnosticArgument *>(&storage) = arg;
  }
  const nextcode::DiagnosticArgument &unbridged() const {
    return *reinterpret_cast<const nextcode::DiagnosticArgument *>(&storage);
  }
#endif

  BridgedDiagnosticArgument(NeXTCodeInt i);
  BridgedDiagnosticArgument(BridgedStringRef s);
};

class BridgedDiagnosticFixIt {
  int64_t storage[7];

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedDiagnosticFixIt(const nextcode::DiagnosticInfo::FixIt &fixit){
    *reinterpret_cast<nextcode::DiagnosticInfo::FixIt *>(&storage) = fixit;
  }
  const nextcode::DiagnosticInfo::FixIt &unbridged() const {
    return *reinterpret_cast<const nextcode::DiagnosticInfo::FixIt *>(&storage);
  }
#endif

  BridgedDiagnosticFixIt(BridgedSourceLoc start, uint32_t length, BridgedStringRef text);
};

/// Diagnostic severity when reporting diagnostics.
enum ENUM_EXTENSIBILITY_ATTR(open) BridgedDiagnosticSeverity : size_t {
  BridgedFatalError,
  BridgedError,
  BridgedWarning,
  BridgedRemark,
  BridgedNote,
};

class BridgedDiagnostic {
public:
  struct Impl;

  NEXTCODE_UNAVAILABLE("Unavailable in NeXTCode")
  Impl *_Nonnull Raw;

  NEXTCODE_UNAVAILABLE("Unavailable in NeXTCode")
  BridgedDiagnostic(Impl *_Nonnull raw) : Raw(raw) {}

  NEXTCODE_UNAVAILABLE("Unavailable in NeXTCode")
  Impl *_Nonnull unbridged() const { return Raw; }
};

// FIXME: Can we bridge InFlightDiagnostic?
NEXTCODE_NAME("BridgedDiagnosticEngine.diagnose(self:at:_:_:highlightAt:"
           "highlightLength:fixIts:)")
void BridgedDiagnosticEngine_diagnose(
    BridgedDiagnosticEngine, BridgedSourceLoc loc, BridgedDiagID diagID,
    BridgedArrayRef arguments, BridgedSourceLoc highlightStart,
    uint32_t hightlightLength, BridgedArrayRef fixIts);

NEXTCODE_NAME("getter:BridgedDiagnosticEngine.hadAnyError(self:)")
bool BridgedDiagnosticEngine_hadAnyError(BridgedDiagnosticEngine);

/// Create a new diagnostic with the given severity, location, and diagnostic
/// text.
///
/// \returns a diagnostic instance that can be extended with additional
/// information and then must be finished via \c NeXTCodeDiagnostic_finish.
NEXTCODE_NAME("BridgedDiagnostic.init(at:message:severity:engine:)")
BridgedDiagnostic BridgedDiagnostic_create(BridgedSourceLoc cLoc,
                                           BridgedStringRef cText,
                                           BridgedDiagnosticSeverity severity,
                                           BridgedDiagnosticEngine cDiags);

/// Highlight a source range as part of the diagnostic.
NEXTCODE_NAME("BridgedDiagnostic.highlight(self:start:end:)")
void BridgedDiagnostic_highlight(BridgedDiagnostic cDiag,
                                 BridgedSourceLoc cStartLoc,
                                 BridgedSourceLoc cEndLoc);

/// Add a Fix-It to replace a source range as part of the diagnostic.
NEXTCODE_NAME("BridgedDiagnostic.fixItReplace(self:start:end:replacement:)")
void BridgedDiagnostic_fixItReplace(BridgedDiagnostic cDiag,
                                    BridgedSourceLoc cStartLoc,
                                    BridgedSourceLoc cEndLoc,
                                    BridgedStringRef cReplaceText);

/// Finish the given diagnostic and emit it.
NEXTCODE_NAME("BridgedDiagnostic.finish(self:)")
void BridgedDiagnostic_finish(BridgedDiagnostic cDiag);

//===----------------------------------------------------------------------===//
// MARK: DeclContexts
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("getter:BridgedDeclContext.isLocalContext(self:)")
bool BridgedDeclContext_isLocalContext(BridgedDeclContext cDeclContext);

NEXTCODE_NAME("BridgedPatternBindingInitializer.create(declContext:)")
BridgedPatternBindingInitializer
BridgedPatternBindingInitializer_create(BridgedDeclContext cDeclContext);

NEXTCODE_NAME("getter:BridgedPatternBindingInitializer.asDeclContext(self:)")
BridgedDeclContext BridgedPatternBindingInitializer_asDeclContext(
    BridgedPatternBindingInitializer cInit);

//===----------------------------------------------------------------------===//
// MARK: DeclAttributes
//===----------------------------------------------------------------------===//

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedDeclAttrKind {
#define DECL_ATTR(_, CLASS, ...) BridgedDeclAttrKind##CLASS,
#include "nextcode/AST/DeclAttr.def"
  BridgedDeclAttrKindNone,
};

NEXTCODE_NAME("BridgedDeclAttrKind.init(from:)")
BridgedDeclAttrKind BridgedDeclAttrKind_fromString(BridgedStringRef cStr);

struct BridgedDeclAttributes {
  BridgedNullableDeclAttribute chain;

  BridgedDeclAttributes() : chain(nullptr){};

#ifdef USED_IN_CPP_SOURCE
  BridgedDeclAttributes(nextcode::DeclAttributes attrs)
      : chain(attrs.getRawAttributeChain()) {}

  nextcode::DeclAttributes unbridged() const {
    nextcode::DeclAttributes attrs;
    attrs.setRawAttributeChain(chain.unbridged());
    return attrs;
  }
#endif
};

NEXTCODE_NAME("BridgedDeclAttributes.add(self:_:)")
void BridgedDeclAttributes_add(BridgedDeclAttributes *_Nonnull attrs,
                               BridgedDeclAttribute add);

NEXTCODE_NAME("BridgedDeclAttribute.createSimple(_:kind:atLoc:nameLoc:)")
BridgedDeclAttribute BridgedDeclAttribute_createSimple(
    BridgedASTContext cContext, BridgedDeclAttrKind cKind,
    BridgedSourceLoc cAtLoc, BridgedSourceLoc cNameLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedAccessLevel {
  BridgedAccessLevelPrivate,
  BridgedAccessLevelFilePrivate,
  BridgedAccessLevelInternal,
  BridgedAccessLevelPackage,
  BridgedAccessLevelPublic,
  BridgedAccessLevelOpen,
};

NEXTCODE_NAME("BridgedAccessControlAttr.createParsed(_:range:accessLevel:)")
BridgedAccessControlAttr
BridgedAccessControlAttr_createParsed(BridgedASTContext cContext,
                                      BridgedSourceRange cRange,
                                      BridgedAccessLevel cAccessLevel);

NEXTCODE_NAME("BridgedAlignmentAttr.createParsed(_:atLoc:range:value:)")
BridgedAlignmentAttr
BridgedAlignmentAttr_createParsed(BridgedASTContext cContext,
                                  BridgedSourceLoc cAtLoc,
                                  BridgedSourceRange cRange, size_t cValue);

NEXTCODE_NAME("BridgedAllowFeatureSuppressionAttr.createParsed(_:atLoc:range:inverted:features:)")
BridgedAllowFeatureSuppressionAttr
BridgedAllowFeatureSuppressionAttr_createParsed(
                                  BridgedASTContext cContext,
                                  BridgedSourceLoc cAtLoc,
                                  BridgedSourceRange cRange,
                                  bool inverted,
                                  BridgedArrayRef cFeatures);

NEXTCODE_NAME("BridgedCDeclAttr.createParsed(_:atLoc:range:name:)")
BridgedCDeclAttr BridgedCDeclAttr_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cAtLoc,
                                               BridgedSourceRange cRange,
                                               BridgedStringRef cName);

NEXTCODE_NAME(
    "BridgedDynamicReplacementAttr.createParsed(_:atLoc:attrNameLoc:lParenLoc:"
    "replacedFunction:rParenLoc:)")
BridgedDynamicReplacementAttr BridgedDynamicReplacementAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceLoc cAttrNameLoc, BridgedSourceLoc cLParenLoc,
    BridgedDeclNameRef cReplacedFunction, BridgedSourceLoc cRParenLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedEffectsKind {
  BridgedEffectsKindReadNone,
  BridgedEffectsKindReadOnly,
  BridgedEffectsKindReleaseNone,
  BridgedEffectsKindReadWrite,
  BridgedEffectsKindUnspecified,
  BridgedEffectsKindCustom,
};

NEXTCODE_NAME("BridgedEffectsAttr.createParsed(_:atLoc:range:effectKind:)")
BridgedEffectsAttr BridgedEffectsAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedEffectsKind cEffectKind);

NEXTCODE_NAME("BridgedEffectsAttr.createParsed(_:atLoc:range:customString:"
           "customStringLoc:)")
BridgedEffectsAttr BridgedEffectsAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedStringRef cCustomString,
    BridgedSourceLoc cCustomStringLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedExclusivityAttrMode {
  BridgedExclusivityAttrModeChecked,
  BridgedExclusivityAttrModeUnchecked,
};

NEXTCODE_NAME("BridgedExclusivityAttr.createParsed(_:atLoc:range:mode:)")
BridgedExclusivityAttr BridgedExclusivityAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedExclusivityAttrMode cMode);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedExposureKind {
  BridgedExposureKindCxx,
  BridgedExposureKindWasm,
};

NEXTCODE_NAME("BridgedExposeAttr.createParsed(_:atLoc:range:name:kind:)")
BridgedExposeAttr BridgedExposeAttr_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cAtLoc,
                                                 BridgedSourceRange cRange,
                                                 BridgedStringRef cName,
                                                 BridgedExposureKind cKind);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedExternKind {
  BridgedExternKindC,
  BridgedExternKindWasm,
};

NEXTCODE_NAME("BridgedExternAttr.createParsed(_:atLoc:range:lParenLoc:rParenLoc:"
           "kind:moduleName:name:)")
BridgedExternAttr BridgedExternAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedSourceLoc cLParenLoc,
    BridgedSourceLoc cRParenLoc, BridgedExternKind cKind,
    BridgedStringRef cModuleName, BridgedStringRef cName);

NEXTCODE_NAME("BridgedImplementsAttr.createParsed(_:atLoc:range:protocolType:"
           "memberName:memberNameLoc:)")
BridgedImplementsAttr BridgedImplementsAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedTypeRepr cProtocolType,
    BridgedDeclNameRef cMemberName, BridgedDeclNameLoc cMemberNameLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedInlineKind {
  BridgedInlineKindNever,
  BridgedInlineKindAlways,
};

NEXTCODE_NAME("BridgedInlineAttr.createParsed(_:atLoc:range:kind:)")
BridgedInlineAttr BridgedInlineAttr_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cAtLoc,
                                                 BridgedSourceRange cRange,
                                                 BridgedInlineKind cKind);

NEXTCODE_NAME("BridgedMainTypeAttr.createParsed(_:atLoc:nameLoc:)")
BridgedMainTypeAttr BridgedMainTypeAttr_createParsed(BridgedASTContext cContext,
                                                     BridgedSourceLoc cAtLoc,
                                                     BridgedSourceLoc cNameLoc);

NEXTCODE_NAME(
    "BridgedNeXTCodeNativeObjCRuntimeBaseAttr.createParsed(_:atLoc:range:name:)")
BridgedNeXTCodeNativeObjCRuntimeBaseAttr
BridgedNeXTCodeNativeObjCRuntimeBaseAttr_createParsed(BridgedASTContext cContext,
                                                   BridgedSourceLoc cAtLoc,
                                                   BridgedSourceRange cRange,
                                                   BridgedIdentifier cName);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedNonSendableKind {
  BridgedNonSendableKindSpecific,
  BridgedNonSendableKindAssumed,
};

NEXTCODE_NAME("BridgedNonSendableAttr.createParsed(_:atLoc:range:kind:)")
BridgedNonSendableAttr BridgedNonSendableAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedNonSendableKind cKind);

NEXTCODE_NAME("BridgedObjCAttr.createParsedUnnamed(_:atLoc:attrNameLoc:)")
BridgedObjCAttr
BridgedObjCAttr_createParsedUnnamed(BridgedASTContext cContext,
                                    BridgedSourceLoc cAtLoc,
                                    BridgedSourceLoc cAttrNameLoc);

NEXTCODE_NAME("BridgedObjCAttr.createParsedNullary(_:atLoc:attrNameLoc:lParenLoc:"
           "nameLoc:name:rParenLoc:)")
BridgedObjCAttr BridgedObjCAttr_createParsedNullary(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceLoc cAttrNameLoc, BridgedSourceLoc cLParenLoc,
    BridgedSourceLoc cNameLoc, BridgedIdentifier cName,
    BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME("BridgedObjCAttr.createParsedSelector(_:atLoc:attrNameLoc:lParenLoc:"
           "nameLocs:names:rParenLoc:)")
BridgedObjCAttr BridgedObjCAttr_createParsedSelector(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceLoc cAttrNameLoc, BridgedSourceLoc cLParenLoc,
    BridgedArrayRef cNameLocs, BridgedArrayRef cNames,
    BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME("BridgedObjCImplementationAttr.createParsed(_:atLoc:range:name:isEarlyAdopter:)")
BridgedObjCImplementationAttr BridgedObjCImplementationAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedIdentifier cName, bool isEarlyAdopter);

NEXTCODE_NAME("BridgedObjCRuntimeNameAttr.createParsed(_:atLoc:range:name:)")
BridgedObjCRuntimeNameAttr BridgedObjCRuntimeNameAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedIdentifier cName);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedOptimizationMode {
  BridgedOptimizationModeForSpeed,
  BridgedOptimizationModeForSize,
  BridgedOptimizationModeNoOptimization,
};

NEXTCODE_NAME("BridgedOptimizeAttr.createParsed(_:atLoc:range:mode:)")
BridgedOptimizeAttr BridgedOptimizeAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedOptimizationMode cMode);

NEXTCODE_NAME("BridgedPrivateImportAttr.createParsed(_:atLoc:attrNameLoc:"
           "lParenLoc:fileName:rParenLoc:)")
BridgedPrivateImportAttr BridgedPrivateImportAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceLoc cAttrNameLoc, BridgedSourceLoc cLParenLoc,
    BridgedStringRef cFileName, BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME(
    "BridgedProjectedValuePropertyAttr.createParsed(_:atLoc:range:name:)")
BridgedProjectedValuePropertyAttr
BridgedProjectedValuePropertyAttr_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cAtLoc,
                                               BridgedSourceRange cRange,
                                               BridgedIdentifier cName);

NEXTCODE_NAME("BridgedRawDocCommentAttr.createParsed(_:range:)")
BridgedRawDocCommentAttr
BridgedRawDocCommentAttr_createParsed(BridgedASTContext cContext,
                                      BridgedCharSourceRange cRange);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedReferenceOwnership {
  BridgedReferenceOwnershipWeak,
  BridgedReferenceOwnershipUnowned,
  BridgedReferenceOwnershipUnmanaged,
};

NEXTCODE_NAME("BridgedReferenceOwnershipAttr.createParsed(_:atLoc:range:kind:)")
BridgedReferenceOwnershipAttr BridgedReferenceOwnershipAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedReferenceOwnership cKind);

NEXTCODE_NAME("BridgedSectionAttr.createParsed(_:atLoc:range:name:)")
BridgedSectionAttr BridgedSectionAttr_createParsed(BridgedASTContext cContext,
                                                   BridgedSourceLoc cAtLoc,
                                                   BridgedSourceRange cRange,
                                                   BridgedStringRef cName);

NEXTCODE_NAME("BridgedSemanticsAttr.createParsed(_:atLoc:range:value:)")
BridgedSemanticsAttr BridgedSemanticsAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedStringRef cValue);

NEXTCODE_NAME("BridgedSetterAccessAttr.createParsed(_:range:accessLevel:)")
BridgedSetterAccessAttr
BridgedSetterAccessAttr_createParsed(BridgedASTContext cContext,
                                     BridgedSourceRange cRange,
                                     BridgedAccessLevel cAccessLevel);

NEXTCODE_NAME(
    "BridgedSPIAccessControlAttr.createParsed(_:atLoc:range:spiGroupName:)")
BridgedSPIAccessControlAttr BridgedSPIAccessControlAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedIdentifier cSPIGroupName);

NEXTCODE_NAME("BridgedSILGenNameAttr.createParsed(_:atLoc:range:name:isRaw:)")
BridgedSILGenNameAttr BridgedSILGenNameAttr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAtLoc,
    BridgedSourceRange cRange, BridgedStringRef cName, bool isRaw);

//===----------------------------------------------------------------------===//
// MARK: Decls
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("BridgedDecl.setAttrs(self:_:)")
void BridgedDecl_setAttrs(BridgedDecl decl, BridgedDeclAttributes attrs);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedStaticSpelling {
  BridgedStaticSpellingNone,
  BridgedStaticSpellingStatic,
  BridgedStaticSpellingClass
};

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedAccessorKind {
#define ACCESSOR(ID) BridgedAccessorKind##ID,
#include "nextcode/AST/AccessorKinds.def"
};

struct BridgedAccessorRecord {
  BridgedSourceLoc lBraceLoc;
  BridgedArrayRef accessors;
  BridgedSourceLoc rBraceLoc;
};

NEXTCODE_NAME("BridgedAccessorDecl.createParsed(_:declContext:kind:storage:"
           "declLoc:accessorKeywordLoc:parameterList:asyncSpecifierLoc:"
           "throwsSpecifierLoc:thrownType:)")
BridgedAccessorDecl BridgedAccessorDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedAccessorKind cKind, BridgedAbstractStorageDecl cStorage,
    BridgedSourceLoc cDeclLoc, BridgedSourceLoc cAccessorKeywordLoc,
    BridgedNullableParameterList cParamList, BridgedSourceLoc cAsyncLoc,
    BridgedSourceLoc cThrowsLoc, BridgedNullableTypeRepr cThrownType);

NEXTCODE_NAME(
    "BridgedPatternBindingDecl.createParsed(_:declContext:bindingKeywordLoc:"
    "entries:isStatic:isLet:)")
BridgedPatternBindingDecl BridgedPatternBindingDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cBindingKeywordLoc, BridgedArrayRef cBindingEntries,
    bool isStatic, bool isLet);

NEXTCODE_NAME("BridgedParamDecl.createParsed(_:declContext:specifierLoc:argName:"
           "argNameLoc:paramName:paramNameLoc:type:defaultValue:)")
BridgedParamDecl BridgedParamDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cSpecifierLoc, BridgedIdentifier cArgName,
    BridgedSourceLoc cArgNameLoc, BridgedIdentifier cParamName,
    BridgedSourceLoc cParamNameLoc, BridgedNullableTypeRepr type,
    BridgedNullableExpr defaultValue);

NEXTCODE_NAME("BridgedConstructorDecl.setParsedBody(self:_:)")
void BridgedConstructorDecl_setParsedBody(BridgedConstructorDecl decl,
                                          BridgedBraceStmt body);

NEXTCODE_NAME("BridgedFuncDecl.setParsedBody(self:_:)")
void BridgedFuncDecl_setParsedBody(BridgedFuncDecl decl, BridgedBraceStmt body);

NEXTCODE_NAME("BridgedDestructorDecl.setParsedBody(self:_:)")
void BridgedDestructorDecl_setParsedBody(BridgedDestructorDecl decl,
                                         BridgedBraceStmt body);

NEXTCODE_NAME("BridgedFuncDecl.createParsed(_:declContext:staticLoc:"
           "staticSpelling:funcKeywordLoc:"
           "name:nameLoc:genericParamList:parameterList:asyncSpecifierLoc:"
           "throwsSpecifierLoc:thrownType:returnType:genericWhereClause:)")
BridgedFuncDecl BridgedFuncDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cStaticLoc, BridgedStaticSpelling cStaticSpelling,
    BridgedSourceLoc cFuncKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedParameterList parameterList, BridgedSourceLoc cAsyncLoc,
    BridgedSourceLoc cThrowsLoc, BridgedNullableTypeRepr thrownType,
    BridgedNullableTypeRepr returnType,
    BridgedNullableTrailingWhereClause opaqueGenericWhereClause);

NEXTCODE_NAME(
    "BridgedConstructorDecl.createParsed(_:declContext:initKeywordLoc:"
    "failabilityMarkLoc:isIUO:genericParamList:parameterList:"
    "asyncSpecifierLoc:throwsSpecifierLoc:thrownType:genericWhereClause:)")
BridgedConstructorDecl BridgedConstructorDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cInitKeywordLoc, BridgedSourceLoc cFailabilityMarkLoc,
    bool isIUO, BridgedNullableGenericParamList genericParams,
    BridgedParameterList parameterList, BridgedSourceLoc cAsyncLoc,
    BridgedSourceLoc cThrowsLoc, BridgedNullableTypeRepr thrownType,
    BridgedNullableTrailingWhereClause genericWhereClause);

NEXTCODE_NAME(
    "BridgedDestructorDecl.createParsed(_:declContext:deinitKeywordLoc:)")
BridgedDestructorDecl
BridgedDestructorDecl_createParsed(BridgedASTContext cContext,
                                   BridgedDeclContext cDeclContext,
                                   BridgedSourceLoc cDeinitKeywordLoc);

NEXTCODE_NAME(
    "BridgedTypeAliasDecl.createParsed(_:declContext:typealiasKeywordLoc:name:"
    "nameLoc:genericParamList:equalLoc:underlyingType:genericWhereClause:)")
BridgedTypeAliasDecl BridgedTypeAliasDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cAliasKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedSourceLoc cEqualLoc, BridgedTypeRepr underlyingType,
    BridgedNullableTrailingWhereClause genericWhereClause);

NEXTCODE_NAME("BridgedExtensionDecl.setParsedMembers(self:_:)")
void BridgedExtensionDecl_setParsedMembers(BridgedExtensionDecl decl,
                                           BridgedArrayRef members);

NEXTCODE_NAME(
    "BridgedEnumDecl.createParsed(_:declContext:enumKeywordLoc:name:nameLoc:"
    "genericParamList:inheritedTypes:genericWhereClause:braceRange:)")
BridgedNominalTypeDecl BridgedEnumDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cEnumKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    BridgedSourceRange cBraceRange);

NEXTCODE_NAME(
    "BridgedEnumCaseDecl.createParsed(declContext:caseKeywordLoc:elements:)")
BridgedEnumCaseDecl
BridgedEnumCaseDecl_createParsed(BridgedDeclContext cDeclContext,
                                 BridgedSourceLoc cCaseKeywordLoc,
                                 BridgedArrayRef cElements);

NEXTCODE_NAME("BridgedEnumElementDecl.createParsed(_:declContext:name:nameLoc:"
           "parameterList:equalsLoc:rawValue:)")
BridgedEnumElementDecl BridgedEnumElementDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedIdentifier cName, BridgedSourceLoc cNameLoc,
    BridgedNullableParameterList parameterList, BridgedSourceLoc cEqualsLoc,
    BridgedNullableExpr opaqueRawValue);

NEXTCODE_NAME("BridgedStructDecl.createParsed(_:declContext:structKeywordLoc:name:"
           "nameLoc:genericParamList:inheritedTypes:genericWhereClause:"
           "braceRange:)")
BridgedNominalTypeDecl BridgedStructDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cStructKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    BridgedSourceRange cBraceRange);

NEXTCODE_NAME(
    "BridgedClassDecl.createParsed(_:declContext:classKeywordLoc:name:nameLoc:"
    "genericParamList:inheritedTypes:genericWhereClause:braceRange:isActor:)")
BridgedNominalTypeDecl BridgedClassDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cClassKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedNullableGenericParamList genericParamList,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    BridgedSourceRange cBraceRange, bool isActor);

NEXTCODE_NAME(
    "BridgedProtocolDecl.createParsed(_:declContext:protocolKeywordLoc:name:"
    "nameLoc:primaryAssociatedTypeNames:inheritedTypes:"
    "genericWhereClause:braceRange:)")
BridgedNominalTypeDecl BridgedProtocolDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cProtocolKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedArrayRef cPrimaryAssociatedTypeNames,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    BridgedSourceRange cBraceRange);

NEXTCODE_NAME("BridgedAssociatedTypeDecl.createParsed(_:declContext:"
           "associatedtypeKeywordLoc:name:nameLoc:inheritedTypes:defaultType:"
           "genericWhereClause:)")
BridgedAssociatedTypeDecl BridgedAssociatedTypeDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cAssociatedtypeKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedArrayRef cInheritedTypes,
    BridgedNullableTypeRepr opaqueDefaultType,
    BridgedNullableTrailingWhereClause genericWhereClause);

NEXTCODE_NAME(
    "BridgedExtensionDecl.createParsed(_:declContext:extensionKeywordLoc:"
    "extendedType:inheritedTypes:genericWhereClause:braceRange:)")
BridgedExtensionDecl BridgedExtensionDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cExtensionKeywordLoc, BridgedTypeRepr opaqueExtendedType,
    BridgedArrayRef cInheritedTypes,
    BridgedNullableTrailingWhereClause genericWhereClause,
    BridgedSourceRange cBraceRange);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedOperatorFixity {
  BridgedOperatorFixityInfix,
  BridgedOperatorFixityPrefix,
  BridgedOperatorFixityPostfix,
};

NEXTCODE_NAME("BridgedOperatorDecl.createParsed(_:declContext:fixity:"
           "operatorKeywordLoc:name:nameLoc:colonLoc:precedenceGroupName:"
           "precedenceGroupLoc:)")
BridgedOperatorDecl BridgedOperatorDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedOperatorFixity cFixity, BridgedSourceLoc cOperatorKeywordLoc,
    BridgedIdentifier cName, BridgedSourceLoc cNameLoc,
    BridgedSourceLoc cColonLoc, BridgedIdentifier cPrecedenceGroupName,
    BridgedSourceLoc cPrecedenceGroupLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedAssociativity {
  BridgedAssociativityNone,
  BridgedAssociativityLeft,
  BridgedAssociativityRight,
};

NEXTCODE_NAME("BridgedPrecedenceGroupDecl.createParsed(declContext:"
           "precedencegroupKeywordLoc:name:nameLoc:leftBraceLoc:"
           "associativityLabelLoc:associativityValueLoc:associativity:"
           "assignmentLabelLoc:assignmentValueLoc:isAssignment:"
           "higherThanKeywordLoc:higherThanNames:lowerThanKeywordLoc:"
           "lowerThanNames:rightBraceLoc:)")
BridgedPrecedenceGroupDecl BridgedPrecedenceGroupDecl_createParsed(
    BridgedDeclContext cDeclContext,
    BridgedSourceLoc cPrecedencegroupKeywordLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedSourceLoc cLeftBraceLoc,
    BridgedSourceLoc cAssociativityKeywordLoc,
    BridgedSourceLoc cAssociativityValueLoc,
    BridgedAssociativity cAssociativity, BridgedSourceLoc cAssignmentKeywordLoc,
    BridgedSourceLoc cAssignmentValueLoc, bool isAssignment,
    BridgedSourceLoc cHigherThanKeywordLoc, BridgedArrayRef cHigherThanNames,
    BridgedSourceLoc cLowerThanKeywordLoc, BridgedArrayRef cLowerThanNames,
    BridgedSourceLoc cRightBraceLoc);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedImportKind {
  BridgedImportKindModule,
  BridgedImportKindType,
  BridgedImportKindStruct,
  BridgedImportKindClass,
  BridgedImportKindEnum,
  BridgedImportKindProtocol,
  BridgedImportKindVar,
  BridgedImportKindFunc,
};

NEXTCODE_NAME("BridgedImportDecl.createParsed(_:declContext:importKeywordLoc:"
           "importKind:importKindLoc:path:)")
BridgedImportDecl BridgedImportDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cImportKeywordLoc, BridgedImportKind cImportKind,
    BridgedSourceLoc cImportKindLoc, BridgedArrayRef cImportPathElements);

NEXTCODE_NAME("BridgedSubscriptDecl.createParsed(_:declContext:staticLoc:"
           "staticSpelling:subscriptKeywordLoc:genericParamList:parameterList:"
           "arrowLoc:returnType:)")
BridgedSubscriptDecl BridgedSubscriptDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cStaticLoc, BridgedStaticSpelling cStaticSpelling,
    BridgedSourceLoc cSubscriptKeywordLoc,
    BridgedNullableGenericParamList cGenericParamList,
    BridgedParameterList cParamList, BridgedSourceLoc cArrowLoc,
    BridgedTypeRepr returnType);

NEXTCODE_NAME(
    "BridgedTopLevelCodeDecl.createParsed(_:declContext:startLoc:stmt:endLoc:)")
BridgedTopLevelCodeDecl BridgedTopLevelCodeDecl_createStmt(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cStartLoc, BridgedStmt statement,
    BridgedSourceLoc cEndLoc);

NEXTCODE_NAME(
    "BridgedTopLevelCodeDecl.createParsed(_:declContext:startLoc:expr:endLoc:)")
BridgedTopLevelCodeDecl BridgedTopLevelCodeDecl_createExpr(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cStartLoc, BridgedExpr expression,
    BridgedSourceLoc cEndLoc);

NEXTCODE_NAME("BridgedTopLevelCodeDecl.dump(self:)")
void BridgedTopLevelCodeDecl_dump(BridgedTopLevelCodeDecl decl);

NEXTCODE_NAME("BridgedDecl.dump(self:)")
void BridgedDecl_dump(BridgedDecl decl);

//===----------------------------------------------------------------------===//
// MARK: AbstractStorageDecl
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("BridgedAbstractStorageDecl.setAccessors(self:_:)")
void BridgedAbstractStorageDecl_setAccessors(
    BridgedAbstractStorageDecl cStorage, BridgedAccessorRecord accessors);

//===----------------------------------------------------------------------===//
// MARK: AccessorDecl
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("BridgedAccessorDecl.setParsedBody(self:_:)")
void BridgedAccessorDecl_setParsedBody(BridgedAccessorDecl decl,
                                       BridgedBraceStmt body);

//===----------------------------------------------------------------------===//
// MARK: NominalTypeDecl
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("BridgedNominalTypeDecl.getName(self:)")
BRIDGED_INLINE
BridgedStringRef BridgedNominalTypeDecl_getName(BridgedNominalTypeDecl decl);

NEXTCODE_NAME("BridgedNominalTypeDecl.isStructWithUnreferenceableStorage(self:)")
bool BridgedNominalTypeDecl_isStructWithUnreferenceableStorage(
    BridgedNominalTypeDecl decl);

NEXTCODE_NAME("BridgedNominalTypeDecl.isGlobalActor(self:)")
BRIDGED_INLINE
bool BridgedNominalTypeDecl_isGlobalActor(BridgedNominalTypeDecl decl);

NEXTCODE_NAME("BridgedNominalTypeDecl.hasValueDeinit(self:)")
BRIDGED_INLINE
bool BridgedNominalTypeDecl_hasValueDeinit(BridgedNominalTypeDecl decl);

NEXTCODE_NAME("BridgedNominalTypeDecl.setParsedMembers(self:_:)")
void BridgedNominalTypeDecl_setParsedMembers(BridgedNominalTypeDecl decl,
                                             BridgedArrayRef members);

//===----------------------------------------------------------------------===//
// MARK: SubscriptDecl
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("getter:BridgedSubscriptDecl.asAbstractStorageDecl(self:)")
BRIDGED_INLINE
BridgedAbstractStorageDecl
BridgedSubscriptDecl_asAbstractStorageDecl(BridgedSubscriptDecl decl);

//===----------------------------------------------------------------------===//
// MARK: VarDecl
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("BridgedVarDecl.createImplicitStringInterpolationVar(_:)")
BridgedVarDecl BridgedVarDec_createImplicitStringInterpolationVar(
    BridgedDeclContext cDeclContext);

NEXTCODE_NAME("BridgedVarDecl.getSourceLocation(self:)")
BRIDGED_INLINE BridgedSourceLoc BridgedVarDecl_getSourceLocation(BridgedVarDecl decl);

NEXTCODE_NAME("BridgedVarDecl.getUserFacingName(self:)")
BRIDGED_INLINE
BridgedStringRef BridgedVarDecl_getUserFacingName(BridgedVarDecl decl);

NEXTCODE_NAME("getter:BridgedVarDecl.asAbstractStorageDecl(self:)")
BRIDGED_INLINE
BridgedAbstractStorageDecl
BridgedVarDecl_asAbstractStorageDecl(BridgedVarDecl decl);

//===----------------------------------------------------------------------===//
// MARK: Exprs
//===----------------------------------------------------------------------===//

struct BridgedCallArgument {
  BridgedSourceLoc labelLoc;
  BridgedIdentifier label;
  BridgedExpr argExpr;

#ifdef USED_IN_CPP_SOURCE
  nextcode::Argument unbridged() const {
    return nextcode::Argument(labelLoc.unbridged(), label.unbridged(),
                           argExpr.unbridged());
  }
#endif
};

NEXTCODE_NAME("BridgedArgumentList.createImplicitUnlabeled(_:exprs:)")
BridgedArgumentList
BridgedArgumentList_createImplicitUnlabeled(BridgedASTContext cContext,
                                            BridgedArrayRef cExprs);

NEXTCODE_NAME("BridgedArgumentList.createParsed(_:lParenLoc:args:rParenLoc:"
           "firstTrailingClosureIndex:)")
BridgedArgumentList BridgedArgumentList_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLParenLoc,
    BridgedArrayRef cArgs, BridgedSourceLoc cRParenLoc,
    size_t cFirstTrailingClosureIndex);

NEXTCODE_NAME("BridgedArrayExpr.createParsed(_:lSquareLoc:elements:commaLocs:"
           "rSquareLoc:)")
BridgedArrayExpr BridgedArrayExpr_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cLLoc,
                                               BridgedArrayRef elements,
                                               BridgedArrayRef commas,
                                               BridgedSourceLoc cRLoc);

NEXTCODE_NAME(
    "BridgedArrowExpr.createParsed(_:asyncLoc:throwsLoc:thrownType:arrowLoc:)")
BridgedArrowExpr BridgedArrowExpr_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cAsyncLoc,
                                               BridgedSourceLoc cThrowsLoc,
                                               BridgedNullableExpr cThrownType,
                                               BridgedSourceLoc cArrowLoc);

NEXTCODE_NAME("BridgedAssignExpr.createParsed(_:equalsLoc:)")
BridgedAssignExpr BridgedAssignExpr_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cEqualsLoc);

NEXTCODE_NAME("BridgedAwaitExpr.createParsed(_:awaitLoc:subExpr:)")
BridgedAwaitExpr BridgedAwaitExpr_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cAwaitLoc,
                                               BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedBooleanLiteralExpr.createParsed(_:value:loc:)")
BridgedBooleanLiteralExpr
BridgedBooleanLiteralExpr_createParsed(BridgedASTContext cContext, bool value,
                                       BridgedSourceLoc cTokenLoc);

NEXTCODE_NAME("BridgedBorrowExpr.createParsed(_:borrowLoc:subExpr:)")
BridgedBorrowExpr BridgedBorrowExpr_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cBorrowLoc,
                                                 BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedCallExpr.createParsed(_:fn:args:)")
BridgedCallExpr BridgedCallExpr_createParsed(BridgedASTContext cContext,
                                             BridgedExpr fn,
                                             BridgedArgumentList args);

NEXTCODE_NAME("BridgedClosureExpr.createParsed(_:declContext:parameterList:body:)")
BridgedClosureExpr BridgedClosureExpr_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedParameterList cParamList, BridgedBraceStmt body);

NEXTCODE_NAME("BridgedCoerceExpr.createParsed(_:asLoc:type:)")
BridgedCoerceExpr BridgedCoerceExpr_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cAsLoc,
                                                 BridgedTypeRepr cType);

NEXTCODE_NAME(
    "BridgedConditionalCheckedCastExpr.createParsed(_:asLoc:questionLoc:type:)")
BridgedConditionalCheckedCastExpr
BridgedConditionalCheckedCastExpr_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cAsLoc,
                                               BridgedSourceLoc cQuestionLoc,
                                               BridgedTypeRepr cType);

NEXTCODE_NAME("BridgedConsumeExpr.createParsed(_:consumeLoc:subExpr:)")
BridgedConsumeExpr BridgedConsumeExpr_createParsed(BridgedASTContext cContext,
                                                   BridgedSourceLoc cConsumeLoc,
                                                   BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedCopyExpr.createParsed(_:copyLoc:subExpr:)")
BridgedCopyExpr BridgedCopyExpr_createParsed(BridgedASTContext cContext,
                                             BridgedSourceLoc cCopyLoc,
                                             BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedDeclRefExpr.create(_:decl:loc:isImplicit:)")
BridgedDeclRefExpr BridgedDeclRefExpr_create(BridgedASTContext cContext,
                                             BridgedDecl cDecl,
                                             BridgedDeclNameLoc cLoc,
                                             bool IsImplicit);

NEXTCODE_NAME("BridgedDictionaryExpr.createParsed(_:lBracketLoc:elements:"
           "colonLocs:rBracketLoc:)")
BridgedDictionaryExpr BridgedDictionaryExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLBracketLoc,
    BridgedArrayRef cElements, BridgedArrayRef cCommaLocs,
    BridgedSourceLoc cRBracketLoc);

NEXTCODE_NAME("BridgedDiscardAssignmentExpr.createParsed(_:loc:)")
BridgedDiscardAssignmentExpr
BridgedDiscardAssignmentExpr_createParsed(BridgedASTContext cContext,
                                          BridgedSourceLoc cLoc);

NEXTCODE_NAME("BridgedDotSelfExpr.createParsed(_:subExpr:dotLoc:selfLoc:)")
BridgedDotSelfExpr BridgedDotSelfExpr_createParsed(BridgedASTContext cContext,
                                                   BridgedExpr cSubExpr,
                                                   BridgedSourceLoc cDotLoc,
                                                   BridgedSourceLoc cSelfLoc);

NEXTCODE_NAME("BridgedForceTryExpr.createParsed(_:tryLoc:subExpr:exclaimLoc:)")
BridgedForceTryExpr
BridgedForceTryExpr_createParsed(BridgedASTContext cContext,
                                 BridgedSourceLoc cTryLoc, BridgedExpr cSubExpr,
                                 BridgedSourceLoc cExclaimLoc);

NEXTCODE_NAME(
    "BridgedForcedCheckedCastExpr.createParsed(_:asLoc:exclaimLoc:type:)")
BridgedForcedCheckedCastExpr BridgedForcedCheckedCastExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cAsLoc,
    BridgedSourceLoc cExclaimLoc, BridgedTypeRepr cType);

NEXTCODE_NAME("BridgedIntegerLiteralExpr.createParsed(_:value:loc:)")
BridgedIntegerLiteralExpr
BridgedIntegerLiteralExpr_createParsed(BridgedASTContext cContext,
                                       BridgedStringRef cStr,
                                       BridgedSourceLoc cTokenLoc);

NEXTCODE_NAME("BridgedInterpolatedStringLiteralExpr.createParsed(_:loc:"
           "literalCapacity:interpolationCount:appendingExpr:)")
BridgedInterpolatedStringLiteralExpr
BridgedInterpolatedStringLiteralExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLoc, size_t literalCapacity,
    size_t interpolationCount, BridgedTapExpr cAppendingExpr);

NEXTCODE_NAME("BridgedIsExpr.createParsed(_:isLoc:type:)")
BridgedIsExpr BridgedIsExpr_createParsed(BridgedASTContext cContext,
                                         BridgedSourceLoc cIsLoc,
                                         BridgedTypeRepr cType);

NEXTCODE_NAME("BridgedNilLiteralExpr.createParsed(_:nilKeywordLoc:)")
BridgedNilLiteralExpr
BridgedNilLiteralExpr_createParsed(BridgedASTContext cContext,
                                   BridgedSourceLoc cNilKeywordLoc);

NEXTCODE_NAME("BridgedOptionalTryExpr.createParsed(_:tryLoc:subExpr:questionLoc:)")
BridgedOptionalTryExpr BridgedOptionalTryExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cTryLoc, BridgedExpr cSubExpr,
    BridgedSourceLoc cQuestionLoc);

NEXTCODE_NAME("BridgedPackElementExpr.createParsed(_:eachLoc:packRefExpr:)")
BridgedPackElementExpr
BridgedPackElementExpr_createParsed(BridgedASTContext cContext,
                                    BridgedSourceLoc cEachLoc,
                                    BridgedExpr cPackRefExpr);

NEXTCODE_NAME("BridgedPackExpansionExpr.createParsed(_:repeatLoc:patternExpr:)")
BridgedPackExpansionExpr
BridgedPackExpansionExpr_createParsed(BridgedASTContext cContext,
                                      BridgedSourceLoc cRepeatLoc,
                                      BridgedExpr cPatternExpr);

NEXTCODE_NAME("BridgedPostfixUnaryExpr.createParsed(_:operator:operand:)")
BridgedPostfixUnaryExpr
BridgedPostfixUnaryExpr_createParsed(BridgedASTContext cContext,
                                     BridgedExpr oper, BridgedExpr operand);

NEXTCODE_NAME("BridgedPrefixUnaryExpr.createParsed(_:operator:operand:)")
BridgedPrefixUnaryExpr
BridgedPrefixUnaryExpr_createParsed(BridgedASTContext cContext,
                                    BridgedExpr oper, BridgedExpr operand);

NEXTCODE_NAME("BridgedSequenceExpr.createParsed(_:exprs:)")
BridgedSequenceExpr BridgedSequenceExpr_createParsed(BridgedASTContext cContext,
                                                     BridgedArrayRef exprs);

NEXTCODE_NAME("BridgedSingleValueStmtExpr.createWithWrappedBranches(_:stmt:"
           "declContext:mustBeExpr:)")
BridgedSingleValueStmtExpr BridgedSingleValueStmtExpr_createWithWrappedBranches(
    BridgedASTContext cContext, BridgedStmt S, BridgedDeclContext cDeclContext,
    bool mustBeExpr);

NEXTCODE_NAME("BridgedStringLiteralExpr.createParsed(_:value:loc:)")
BridgedStringLiteralExpr
BridgedStringLiteralExpr_createParsed(BridgedASTContext cContext,
                                      BridgedStringRef cStr,
                                      BridgedSourceLoc cTokenLoc);

NEXTCODE_NAME("BridgedTapExpr.create(_:body:)")
BridgedTapExpr BridgedTapExpr_create(BridgedASTContext cContext,
                                     BridgedBraceStmt cBody);

NEXTCODE_NAME("BridgedTernaryExpr.createParsed(_:questionLoc:thenExpr:colonLoc:)")
BridgedTernaryExpr BridgedTernaryExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cQuestionLoc,
    BridgedExpr cThenExpr, BridgedSourceLoc cColonLoc);

NEXTCODE_NAME("BridgedTryExpr.createParsed(_:tryLoc:subExpr:)")
BridgedTryExpr BridgedTryExpr_createParsed(BridgedASTContext cContext,
                                           BridgedSourceLoc cTryLoc,
                                           BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedTupleExpr.createParsed(_:leftParenLoc:exprs:labels:"
           "labelLocs:rightParenLoc:)")
BridgedTupleExpr BridgedTupleExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLParen, BridgedArrayRef subs,
    BridgedArrayRef names, BridgedArrayRef cNameLocs, BridgedSourceLoc cRParen);

NEXTCODE_NAME("BridgedTupleExpr.createParsedDictionaryElement(_:key:value:)")
BridgedTupleExpr BridgedTupleExpr_createParsedDictionaryElement(
    BridgedASTContext cContext, BridgedExpr cKeyExpr, BridgedExpr cValueExpr);

NEXTCODE_NAME("BridgedTypeExpr.createParsed(_:type:)")
BridgedTypeExpr BridgedTypeExpr_createParsed(BridgedASTContext cContext,
                                             BridgedTypeRepr cType);

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedDeclRefKind : size_t {
  BridgedDeclRefKindOrdinary,
  BridgedDeclRefKindBinaryOperator,
  BridgedDeclRefKindPostfixOperator,
  BridgedDeclRefKindPrefixOperator,
};

NEXTCODE_NAME("BridgedUnresolvedDeclRefExpr.createParsed(_:name:kind:loc:)")
BridgedUnresolvedDeclRefExpr BridgedUnresolvedDeclRefExpr_createParsed(
    BridgedASTContext cContext, BridgedDeclNameRef cName,
    BridgedDeclRefKind cKind, BridgedDeclNameLoc cLoc);

NEXTCODE_NAME("BridgedUnresolvedDotExpr.createParsed(_:base:dotLoc:name:nameLoc:)")
BridgedUnresolvedDotExpr BridgedUnresolvedDotExpr_createParsed(
    BridgedASTContext cContext, BridgedExpr base, BridgedSourceLoc cDotLoc,
    BridgedDeclNameRef cName, BridgedDeclNameLoc cNameLoc);

NEXTCODE_NAME("BridgedUnresolvedMemberExpr.createParsed(_:dotLoc:name:nameLoc:)")
BridgedUnresolvedMemberExpr BridgedUnresolvedMemberExpr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cDotLoc,
    BridgedDeclNameRef cName, BridgedDeclNameLoc cNameLoc);

NEXTCODE_NAME("BridgedUnresolvedPatternExpr.createParsed(_:pattern:)")
BridgedUnresolvedPatternExpr
BridgedUnresolvedPatternExpr_createParsed(BridgedASTContext cContext,
                                          BridgedPattern cPattern);

NEXTCODE_NAME("BridgedExpr.setImplicit(self:)")
void BridgedExpr_setImplicit(BridgedExpr cExpr);

NEXTCODE_NAME("BridgedExpr.dump(self:)")
void BridgedExpr_dump(BridgedExpr expr);

//===----------------------------------------------------------------------===//
// MARK: Stmts
//===----------------------------------------------------------------------===//

struct BridgedLabeledStmtInfo {
  NEXTCODE_NAME("name")
  BridgedIdentifier Name;
  NEXTCODE_NAME("loc")
  BridgedSourceLoc Loc;

#ifdef USED_IN_CPP_SOURCE
  nextcode::LabeledStmtInfo unbridged() const {
    return {Name.unbridged(), Loc.unbridged()};
  }
#endif
};

class BridgedStmtConditionElement {
  void *_Nonnull Raw;

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedStmtConditionElement(nextcode::StmtConditionElement elem)
      : Raw(elem.getOpaqueValue()) {}

  nextcode::StmtConditionElement unbridged() const {
    return nextcode::StmtConditionElement::fromOpaqueValue(Raw);
  }
#endif
};

NEXTCODE_NAME("BridgedStmtConditionElement.createBoolean(expr:)")
BridgedStmtConditionElement
BridgedStmtConditionElement_createBoolean(BridgedExpr expr);

NEXTCODE_NAME("BridgedStmtConditionElement.createPatternBinding(_:introducerLoc:"
           "pattern:initializer:)")
BridgedStmtConditionElement BridgedStmtConditionElement_createPatternBinding(
    BridgedASTContext cContext, BridgedSourceLoc cIntroducerLoc,
    BridgedPattern cPattern, BridgedExpr cInitializer);

struct BridgedCaseLabelItemInfo {
  NEXTCODE_NAME("isDefault")
  bool IsDefault;
  NEXTCODE_NAME("pattern")
  BridgedPattern ThePattern;
  NEXTCODE_NAME("whereLoc")
  BridgedSourceLoc WhereLoc;
  NEXTCODE_NAME("guardExpr")
  BridgedNullableExpr GuardExpr;
};

NEXTCODE_NAME("BridgedBraceStmt.createParsed(_:lBraceLoc:elements:rBraceLoc:)")
BridgedBraceStmt BridgedBraceStmt_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cLBLoc,
                                               BridgedArrayRef elements,
                                               BridgedSourceLoc cRBLoc);

NEXTCODE_NAME("BridgedBreakStmt.createParsed(_:loc:targetName:targetLoc:)")
BridgedBreakStmt BridgedBreakStmt_createParsed(BridgedDeclContext cDeclContext,
                                               BridgedSourceLoc cLoc,
                                               BridgedIdentifier cTargetName,
                                               BridgedSourceLoc cTargetLoc);

NEXTCODE_NAME("BridgedCaseStmt.createParsedSwitchCase(_:introducerLoc:"
           "caseLabelItems:unknownAttrLoc:terminatorLoc:body:)")
BridgedCaseStmt BridgedCaseStmt_createParsedSwitchCase(
    BridgedASTContext cContext, BridgedSourceLoc cIntroducerLoc,
    BridgedArrayRef cCaseLabelItems, BridgedSourceLoc cUnknownAttrLoc,
    BridgedSourceLoc cTerminatorLoc, BridgedBraceStmt cBody);

NEXTCODE_NAME(
    "BridgedCaseStmt.createParsedDoCatch(_:catchLoc:caseLabelItems:body:)")
BridgedCaseStmt BridgedCaseStmt_createParsedDoCatch(
    BridgedASTContext cContext, BridgedSourceLoc cCatchLoc,
    BridgedArrayRef cCaseLabelItems, BridgedBraceStmt cBody);

NEXTCODE_NAME("BridgedContinueStmt.createParsed(_:loc:targetName:targetLoc:)")
BridgedContinueStmt BridgedContinueStmt_createParsed(
    BridgedDeclContext cDeclContext, BridgedSourceLoc cLoc,
    BridgedIdentifier cTargetName, BridgedSourceLoc cTargetLoc);

NEXTCODE_NAME("BridgedDeferStmt.createParsed(_:deferLoc:)")
BridgedDeferStmt BridgedDeferStmt_createParsed(BridgedDeclContext cDeclContext,
                                               BridgedSourceLoc cDeferLoc);

NEXTCODE_NAME("getter:BridgedDeferStmt.tempDecl(self:)")
BridgedFuncDecl BridgedDeferStmt_getTempDecl(BridgedDeferStmt bridged);

NEXTCODE_NAME("BridgedDiscardStmt.createParsed(_:discardLoc:subExpr:)")
BridgedDiscardStmt BridgedDiscardStmt_createParsed(BridgedASTContext cContext,
                                                   BridgedSourceLoc cDiscardLoc,
                                                   BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedDoStmt.createParsed(_:labelInfo:doLoc:body:)")
BridgedDoStmt BridgedDoStmt_createParsed(BridgedASTContext cContext,
                                         BridgedLabeledStmtInfo cLabelInfo,
                                         BridgedSourceLoc cDoLoc,
                                         BridgedBraceStmt cBody);

NEXTCODE_NAME(
    "BridgedDoCatchStmt.createParsed(_:labelInfo:doLoc:throwsLoc:thrownType:"
    "body:catches:)")
BridgedDoCatchStmt BridgedDoCatchStmt_createParsed(
    BridgedDeclContext cDeclContext, BridgedLabeledStmtInfo cLabelInfo,
    BridgedSourceLoc cDoLoc, BridgedSourceLoc cThrowsLoc,
    BridgedNullableTypeRepr cThrownType, BridgedStmt cBody,
    BridgedArrayRef cCatches);

NEXTCODE_NAME("BridgedFallthroughStmt.createParsed(_:loc:)")
BridgedFallthroughStmt
BridgedFallthroughStmt_createParsed(BridgedASTContext cContext,
                                    BridgedSourceLoc cLoc);

NEXTCODE_NAME("BridgedForEachStmt.createParsed(_:labelInfo:forLoc:tryLoc:awaitLoc:"
           "pattern:inLoc:sequence:whereLoc:whereExpr:body:)")
BridgedForEachStmt BridgedForEachStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    BridgedSourceLoc cForLoc, BridgedSourceLoc cTryLoc,
    BridgedSourceLoc cAwaitLoc, BridgedPattern cPat, BridgedSourceLoc cInLoc,
    BridgedExpr cSequence, BridgedSourceLoc cWhereLoc,
    BridgedNullableExpr cWhereExpr, BridgedBraceStmt cBody);

NEXTCODE_NAME("BridgedGuardStmt.createParsed(_:guardLoc:conds:body:)")
BridgedGuardStmt BridgedGuardStmt_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cGuardLoc,
                                               BridgedArrayRef cConds,
                                               BridgedBraceStmt cBody);

NEXTCODE_NAME("BridgedIfStmt.createParsed(_:labelInfo:ifLoc:conditions:then:"
           "elseLoc:else:)")
BridgedIfStmt BridgedIfStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    BridgedSourceLoc cIfLoc, BridgedArrayRef cConds, BridgedBraceStmt cThen,
    BridgedSourceLoc cElseLoc, BridgedNullableStmt cElse);

NEXTCODE_NAME("BridgedRepeatWhileStmt.createParsed(_:labelInfo:repeatLoc:cond:"
           "whileLoc:body:)")
BridgedRepeatWhileStmt BridgedRepeatWhileStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    BridgedSourceLoc cRepeatLoc, BridgedExpr cCond, BridgedSourceLoc cWhileLoc,
    BridgedStmt cBody);

NEXTCODE_NAME("BridgedReturnStmt.createParsed(_:loc:expr:)")
BridgedReturnStmt BridgedReturnStmt_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cLoc,
                                                 BridgedNullableExpr expr);

NEXTCODE_NAME("BridgedSwitchStmt.createParsed(_:labelInfo:switchLoc:subjectExpr:"
           "lBraceLoc:cases:rBraceLoc:)")
BridgedSwitchStmt BridgedSwitchStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    BridgedSourceLoc cSwitchLoc, BridgedExpr cSubjectExpr,
    BridgedSourceLoc cLBraceLoc, BridgedArrayRef cCases,
    BridgedSourceLoc cRBraceLoc);

NEXTCODE_NAME("BridgedThenStmt.createParsed(_:thenLoc:result:)")
BridgedThenStmt BridgedThenStmt_createParsed(BridgedASTContext cContext,
                                             BridgedSourceLoc cThenLoc,
                                             BridgedExpr cResult);

NEXTCODE_NAME("BridgedThrowStmt.createParsed(_:throwLoc:subExpr:)")
BridgedThrowStmt BridgedThrowStmt_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cThrowLoc,
                                               BridgedExpr cSubExpr);

NEXTCODE_NAME("BridgedWhileStmt.createParsed(_:labelInfo:whileLoc:cond:body:)")
BridgedWhileStmt BridgedWhileStmt_createParsed(
    BridgedASTContext cContext, BridgedLabeledStmtInfo cLabelInfo,
    BridgedSourceLoc cWhileLoc, BridgedArrayRef cCond, BridgedStmt cBody);

NEXTCODE_NAME(
    "BridgedYieldStmt.createParsed(_:yieldLoc:lParenLoc:yields:rParenLoc:)")
BridgedYieldStmt BridgedYieldStmt_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cYieldLoc,
                                               BridgedSourceLoc cLParenLoc,
                                               BridgedArrayRef cYields,
                                               BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME("BridgedStmt.dump(self:)")
void BridgedStmt_dump(BridgedStmt statement);

//===----------------------------------------------------------------------===//
// MARK: TypeAttributes
//===----------------------------------------------------------------------===//

// Bridged type attribute kinds, which mirror TypeAttrKind exactly.
enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedTypeAttrKind {
#define TYPE_ATTR(_, CLASS) BridgedTypeAttrKind##CLASS,
#include "nextcode/AST/TypeAttr.def"
  BridgedTypeAttrKindNone,
};

NEXTCODE_NAME("BridgedTypeAttrKind.init(from:)")
BridgedTypeAttrKind BridgedTypeAttrKind_fromString(BridgedStringRef cStr);

NEXTCODE_NAME("BridgedTypeAttributes.new()")
BridgedTypeAttributes BridgedTypeAttributes_create();

NEXTCODE_NAME("BridgedTypeAttributes.delete(self:)")
void BridgedTypeAttributes_delete(BridgedTypeAttributes cAttributes);

NEXTCODE_NAME("BridgedTypeAttributes.add(self:_:)")
void BridgedTypeAttributes_add(BridgedTypeAttributes cAttributes,
                               BridgedTypeAttribute cAttribute);

NEXTCODE_NAME("getter:BridgedTypeAttributes.isEmpty(self:)")
bool BridgedTypeAttributes_isEmpty(BridgedTypeAttributes cAttributes);

NEXTCODE_NAME("BridgedTypeAttribute.createSimple(_:kind:atLoc:nameLoc:)")
BridgedTypeAttribute BridgedTypeAttribute_createSimple(
    BridgedASTContext cContext, BridgedTypeAttrKind cKind,
    BridgedSourceLoc cAtLoc, BridgedSourceLoc cNameLoc);

enum ENUM_EXTENSIBILITY_ATTR(closed) BridgedIsolatedTypeAttrIsolationKind {
  BridgedIsolatedTypeAttrIsolationKind_DynamicIsolation,
};

NEXTCODE_NAME("BridgedTypeAttribute.createIsolated(_:atLoc:nameLoc:lpLoc:isolationKindLoc:isolationKind:rpLoc:)")
BridgedTypeAttribute BridgedTypeAttribute_createIsolated(
    BridgedASTContext cContext,
    BridgedSourceLoc cAtLoc, BridgedSourceLoc cNameLoc,
    BridgedSourceLoc cLPLoc, BridgedSourceLoc cIsolationLoc,
    BridgedIsolatedTypeAttrIsolationKind cIsolation, BridgedSourceLoc cRPLoc);

//===----------------------------------------------------------------------===//
// MARK: TypeReprs
//===----------------------------------------------------------------------===//

/// Bridged parameter specifiers
enum ENUM_EXTENSIBILITY_ATTR(open) BridgedAttributedTypeSpecifier : size_t {
  BridgedAttributedTypeSpecifierInOut,
  BridgedAttributedTypeSpecifierBorrowing,
  BridgedAttributedTypeSpecifierConsuming,
  BridgedAttributedTypeSpecifierLegacyShared,
  BridgedAttributedTypeSpecifierLegacyOwned,
  BridgedAttributedTypeSpecifierConst,
  BridgedAttributedTypeSpecifierIsolated,
  BridgedAttributedTypeSpecifierTransferring,
  BridgedAttributedTypeSpecifierSending,
};

NEXTCODE_NAME("BridgedUnqualifiedIdentTypeRepr.createParsed(_:loc:name:)")
BridgedUnqualifiedIdentTypeRepr BridgedUnqualifiedIdentTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLoc, BridgedIdentifier id);

NEXTCODE_NAME(
    "BridgedSpecifierTypeRepr.createParsed(_:base:specifier:specifierLoc:)")
BridgedSpecifierTypeRepr BridgedSpecifierTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base,
    BridgedAttributedTypeSpecifier specifier, BridgedSourceLoc cSpecifierLoc);

NEXTCODE_NAME(
    "BridgedArrayTypeRepr.createParsed(_:base:leftSquareLoc:rightSquareLoc:)")
BridgedArrayTypeRepr BridgedArrayTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base,
    BridgedSourceLoc cLSquareLoc, BridgedSourceLoc cRSquareLoc);

NEXTCODE_NAME(
    "BridgedAttributedTypeRepr.createParsed(_:base:consumingAttributes:)")
BridgedAttributedTypeRepr
BridgedAttributedTypeRepr_createParsed(BridgedASTContext cContext,
                                       BridgedTypeRepr base,
                                       BridgedTypeAttributes cAttributes);

NEXTCODE_NAME("BridgedCompositionTypeRepr.createEmpty(_:anyKeywordLoc:)")
BridgedCompositionTypeRepr
BridgedCompositionTypeRepr_createEmpty(BridgedASTContext cContext,
                                       BridgedSourceLoc cAnyLoc);

NEXTCODE_NAME("BridgedCompositionTypeRepr.createParsed(_:types:ampersandLoc:)")
BridgedCompositionTypeRepr
BridgedCompositionTypeRepr_createParsed(BridgedASTContext cContext,
                                        BridgedArrayRef types,
                                        BridgedSourceLoc cFirstAmpLoc);

NEXTCODE_NAME("BridgedDictionaryTypeRepr.createParsed(_:leftSquareLoc:keyType:"
           "colonLoc:valueType:rightSquareLoc:)")
BridgedDictionaryTypeRepr BridgedDictionaryTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLSquareLoc,
    BridgedTypeRepr keyType, BridgedSourceLoc cColonloc,
    BridgedTypeRepr valueType, BridgedSourceLoc cRSquareLoc);

NEXTCODE_NAME("BridgedFunctionTypeRepr.createParsed(_:argsType:asyncLoc:throwsLoc:"
           "thrownType:arrowLoc:resultType:)")
BridgedFunctionTypeRepr BridgedFunctionTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr argsTy,
    BridgedSourceLoc cAsyncLoc, BridgedSourceLoc cThrowsLoc,
    BridgedNullableTypeRepr thrownType, BridgedSourceLoc cArrowLoc,
    BridgedTypeRepr resultType);

NEXTCODE_NAME("BridgedUnqualifiedIdentTypeRepr.createParsed(_:name:nameLoc:"
           "genericArgs:leftAngleLoc:rightAngleLoc:)")
BridgedUnqualifiedIdentTypeRepr BridgedUnqualifiedIdentTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedIdentifier name,
    BridgedSourceLoc cNameLoc, BridgedArrayRef genericArgs,
    BridgedSourceLoc cLAngleLoc, BridgedSourceLoc cRAngleLoc);

NEXTCODE_NAME("BridgedOptionalTypeRepr.createParsed(_:base:questionLoc:)")
BridgedOptionalTypeRepr
BridgedOptionalTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr base,
                                     BridgedSourceLoc cQuestionLoc);

NEXTCODE_NAME("BridgedImplicitlyUnwrappedOptionalTypeRepr.createParsed(_:base:"
           "exclaimLoc:)")
BridgedImplicitlyUnwrappedOptionalTypeRepr
BridgedImplicitlyUnwrappedOptionalTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr base,
    BridgedSourceLoc cExclamationLoc);

NEXTCODE_NAME("BridgedInverseTypeRepr.createParsed(_:tildeLoc:constraint:)")
BridgedInverseTypeRepr
BridgedInverseTypeRepr_createParsed(BridgedASTContext cContext,
                                    BridgedSourceLoc cTildeLoc,
                                    BridgedTypeRepr cConstraint);

NEXTCODE_NAME("BridgedDeclRefTypeRepr.createParsed(_:base:name:nameLoc:genericArguments:angleRange:)")
BridgedDeclRefTypeRepr BridgedDeclRefTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedTypeRepr cBase, BridgedIdentifier cName,
    BridgedSourceLoc cLoc, BridgedArrayRef cGenericArguments,
    BridgedSourceRange cAngleRange);

NEXTCODE_NAME("BridgedMetatypeTypeRepr.createParsed(_:base:typeKeywordLoc:)")
BridgedMetatypeTypeRepr
BridgedMetatypeTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr baseType,
                                     BridgedSourceLoc cTypeLoc);

NEXTCODE_NAME("BridgedProtocolTypeRepr.createParsed(_:base:protocolKeywordLoc:)")
BridgedProtocolTypeRepr
BridgedProtocolTypeRepr_createParsed(BridgedASTContext cContext,
                                     BridgedTypeRepr baseType,
                                     BridgedSourceLoc cProtoLoc);

NEXTCODE_NAME("BridgedPackElementTypeRepr.createParsed(_:base:eachKeywordLoc:)")
BridgedPackElementTypeRepr
BridgedPackElementTypeRepr_createParsed(BridgedASTContext cContext,
                                        BridgedTypeRepr base,
                                        BridgedSourceLoc cEachLoc);

NEXTCODE_NAME(
    "BridgedPackExpansionTypeRepr.createParsed(_:base:repeatKeywordLoc:)")
BridgedPackExpansionTypeRepr
BridgedPackExpansionTypeRepr_createParsed(BridgedASTContext cContext,
                                          BridgedTypeRepr base,
                                          BridgedSourceLoc cRepeatLoc);

NEXTCODE_NAME(
    "BridgedTupleTypeRepr.createParsed(_:elements:leftParenLoc:rightParenLoc:)")
BridgedTupleTypeRepr BridgedTupleTypeRepr_createParsed(
    BridgedASTContext cContext, BridgedArrayRef elements,
    BridgedSourceLoc cLParenLoc, BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME("BridgedNamedOpaqueReturnTypeRepr.createParsed(_:base:)")
BridgedNamedOpaqueReturnTypeRepr
BridgedNamedOpaqueReturnTypeRepr_createParsed(BridgedASTContext cContext,
                                              BridgedTypeRepr baseTy);

NEXTCODE_NAME("BridgedOpaqueReturnTypeRepr.createParsed(_:someKeywordLoc:base:)")
BridgedOpaqueReturnTypeRepr
BridgedOpaqueReturnTypeRepr_createParsed(BridgedASTContext cContext,
                                         BridgedSourceLoc cOpaqueLoc,
                                         BridgedTypeRepr baseTy);

NEXTCODE_NAME("BridgedExistentialTypeRepr.createParsed(_:anyKeywordLoc:base:)")
BridgedExistentialTypeRepr
BridgedExistentialTypeRepr_createParsed(BridgedASTContext cContext,
                                        BridgedSourceLoc cAnyLoc,
                                        BridgedTypeRepr baseTy);

NEXTCODE_NAME("BridgedVarargTypeRepr.createParsed(_:base:ellipsisLoc:)")
BridgedVarargTypeRepr
BridgedVarargTypeRepr_createParsed(BridgedASTContext cContext,
                                   BridgedTypeRepr base,
                                   BridgedSourceLoc cEllipsisLoc);

NEXTCODE_NAME("BridgedTypeRepr.dump(self:)")
void BridgedTypeRepr_dump(BridgedTypeRepr type);

//===----------------------------------------------------------------------===//
// MARK: Patterns
//===----------------------------------------------------------------------===//

NEXTCODE_NAME("getter:BridgedPattern.singleVar(self:)")
BridgedNullableVarDecl BridgedPattern_getSingleVar(BridgedPattern cPattern);

NEXTCODE_NAME("BridgedAnyPattern.createParsed(_:loc:)")
BridgedAnyPattern BridgedAnyPattern_createParsed(BridgedASTContext cContext,
                                                 BridgedSourceLoc cLoc);

NEXTCODE_NAME("BridgedBindingPattern.createParsed(_:keywordLoc:isLet:subPattern:)")
BridgedBindingPattern
BridgedBindingPattern_createParsed(BridgedASTContext cContext,
                                   BridgedSourceLoc cKeywordLoc, bool isLet,
                                   BridgedPattern cSubPattern);

NEXTCODE_NAME("BridgedBindingPattern.createImplicitCatch(_:loc:)")
BridgedBindingPattern
BridgedBindingPattern_createImplicitCatch(BridgedDeclContext cDeclContext,
                                          BridgedSourceLoc cLoc);

NEXTCODE_NAME("BridgedExprPattern.createParsed(_:expr:)")
BridgedExprPattern
BridgedExprPattern_createParsed(BridgedDeclContext cDeclContext,
                                BridgedExpr cExpr);

NEXTCODE_NAME("BridgedIsPattern.createParsed(_:isLoc:typeExpr:)")
BridgedIsPattern BridgedIsPattern_createParsed(BridgedASTContext cContext,
                                               BridgedSourceLoc cIsLoc,
                                               BridgedTypeExpr cTypeExpr);

NEXTCODE_NAME("BridgedNamedPattern.createParsed(_:declContext:name:loc:)")
BridgedNamedPattern
BridgedNamedPattern_createParsed(BridgedASTContext astContext,
                                 BridgedDeclContext declContext,
                                 BridgedIdentifier name, BridgedSourceLoc cLoc);

NEXTCODE_NAME(
    "BridgedParenPattern.createParsed(_:lParenLoc:subPattern:rParenLoc:)")
BridgedParenPattern BridgedParenPattern_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLParenLoc,
    BridgedPattern cSubPattern, BridgedSourceLoc cRParenLoc);

struct BridgedTuplePatternElt {
  BridgedIdentifier Label;
  BridgedSourceLoc LabelLoc;
  BridgedPattern ThePattern;
};

NEXTCODE_NAME("BridgedTuplePattern.createParsed(_:lParenLoc:elements:rParenLoc:)")
BridgedTuplePattern BridgedTuplePattern_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLParenLoc,
    BridgedArrayRef cElements, BridgedSourceLoc cRParenLoc);

NEXTCODE_NAME("BridgedTypedPattern.createParsed(_:pattern:type:)")
BridgedTypedPattern BridgedTypedPattern_createParsed(BridgedASTContext cContext,
                                                     BridgedPattern cPattern,
                                                     BridgedTypeRepr cType);

NEXTCODE_NAME("BridgedTypedPattern.createPropagated(_:pattern:type:)")
BridgedTypedPattern BridgedTypedPattern_createPropagated(
    BridgedASTContext cContext, BridgedPattern cPattern, BridgedTypeRepr cType);

NEXTCODE_NAME("BridgedPattern.setImplicit(self:)")
void BridgedPattern_setImplicit(BridgedPattern cPattern);

NEXTCODE_NAME("getter:BridgedPattern.boundName(self:)")
BridgedIdentifier BridgedPattern_getBoundName(BridgedPattern cPattern);

//===----------------------------------------------------------------------===//
// MARK: Misc
//===----------------------------------------------------------------------===//

struct BridgedTupleTypeElement {
  BridgedIdentifier Name;
  BridgedSourceLoc NameLoc;
  BridgedIdentifier SecondName;
  BridgedSourceLoc SecondNameLoc;
  BridgedSourceLoc UnderscoreLoc;
  BridgedSourceLoc ColonLoc;
  BridgedTypeRepr Type;
  BridgedSourceLoc TrailingCommaLoc;
};

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedRequirementReprKind : size_t {
  /// A type bound T : P, where T is a type that depends on a generic
  /// parameter and P is some type that should bound T, either as a concrete
  /// supertype or a protocol to which T must conform.
  BridgedRequirementReprKindTypeConstraint,

  /// A same-type requirement T == U, where T and U are types that shall be
  /// equivalent.
  BridgedRequirementReprKindSameType,

  /// A layout bound T : L, where T is a type that depends on a generic
  /// parameter and L is some layout specification that should bound T.
  BridgedRequirementReprKindLayoutConstraint,

  // Note: there is code that packs this enum in a 2-bit bitfield.  Audit users
  // when adding enumerators.
};

struct BridgedRequirementRepr {
  BridgedSourceLoc SeparatorLoc;
  BridgedRequirementReprKind Kind;
  BridgedTypeRepr FirstType;
  BridgedTypeRepr SecondType;
  // FIXME: Handle Layout Requirements
};

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedMacroDefinitionKind : size_t {
  /// An expanded macro.
  BridgedExpandedMacro = 0,
  /// An external macro, spelled with either the old spelling (Module.Type)
  /// or the new spelling `#externalMacro(module: "Module", type: "Type")`.
  BridgedExternalMacro,
  /// The builtin definition for "externalMacro".
  BridgedBuiltinExternalMacro,
  /// The builtin definition for the "isolation" macro.
  BridgedBuiltinIsolationMacro,
};

NEXTCODE_NAME("BridgedGenericParamList.createParsed(_:leftAngleLoc:parameters:"
           "genericWhereClause:rightAngleLoc:)")
BridgedGenericParamList BridgedGenericParamList_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLeftAngleLoc,
    BridgedArrayRef cParameters,
    BridgedNullableTrailingWhereClause genericWhereClause,
    BridgedSourceLoc cRightAngleLoc);

NEXTCODE_NAME(
    "BridgedGenericTypeParamDecl.createParsed(_:declContext:eachKeywordLoc:"
    "name:nameLoc:inheritedType:index:)")
BridgedGenericTypeParamDecl BridgedGenericTypeParamDecl_createParsed(
    BridgedASTContext cContext, BridgedDeclContext cDeclContext,
    BridgedSourceLoc cEachLoc, BridgedIdentifier cName,
    BridgedSourceLoc cNameLoc, BridgedNullableTypeRepr opaqueInheritedType,
    size_t index);

NEXTCODE_NAME(
    "BridgedTrailingWhereClause.createParsed(_:whereKeywordLoc:requirements:)")
BridgedTrailingWhereClause
BridgedTrailingWhereClause_createParsed(BridgedASTContext cContext,
                                        BridgedSourceLoc cWhereKeywordLoc,
                                        BridgedArrayRef cRequirements);

NEXTCODE_NAME("BridgedParameterList.createParsed(_:leftParenLoc:parameters:"
           "rightParenLoc:)")
BridgedParameterList BridgedParameterList_createParsed(
    BridgedASTContext cContext, BridgedSourceLoc cLeftParenLoc,
    BridgedArrayRef cParameters, BridgedSourceLoc cRightParenLoc);

//===----------------------------------------------------------------------===//
// MARK: Plugins
//===----------------------------------------------------------------------===//

NEXTCODE_BEGIN_ASSUME_NONNULL

typedef void *PluginHandle;
typedef const void *PluginCapabilityPtr;

/// Set a capability data to the plugin object. Since the data is just a opaque
/// pointer, it's not used in AST at all.
void Plugin_setCapability(PluginHandle handle,
                          PluginCapabilityPtr _Nullable data);

/// Get a capability data set by \c Plugin_setCapability .
PluginCapabilityPtr _Nullable Plugin_getCapability(PluginHandle handle);

/// Lock the plugin. Clients should lock it during sending and recving the
/// response.
void Plugin_lock(PluginHandle handle);

/// Unlock the plugin.
void Plugin_unlock(PluginHandle handle);

/// Launch the plugin if it's not running.
bool Plugin_spawnIfNeeded(PluginHandle handle);

/// Sends the message to the plugin, returns true if there was an error.
/// Clients should receive the response  by \c Plugin_waitForNextMessage .
bool Plugin_sendMessage(PluginHandle handle, const BridgedData data);

/// Receive a message from the plugin.
bool Plugin_waitForNextMessage(PluginHandle handle, BridgedData *data);

NEXTCODE_END_ASSUME_NONNULL

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#ifndef PURE_BRIDGING_MODE
// In _not_ PURE_BRIDGING_MODE, bridging functions are inlined and therefore
// included in the header file. This is because they rely on C++ headers that
// we don't want to pull in when using "pure bridging mode".
#include "ASTBridgingImpl.h"
#endif

#endif // NEXTCODE_AST_ASTBRIDGING_H
