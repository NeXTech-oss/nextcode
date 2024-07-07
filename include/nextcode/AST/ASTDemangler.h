//===--- ASTDemangler.h - NeXTCode AST symbol demangling -----------*- C++ -*-===//
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
//
// Defines a builder concept for the TypeDecoder and MetadataReader which builds
// AST Types, and a utility function wrapper which takes a mangled string and
// feeds it through the TypeDecoder instance.
//
// The RemoteAST library defines a MetadataReader instance that uses this
// concept, together with some additional utilities.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_ASTDEMANGLER_H
#define NEXTCODE_AST_ASTDEMANGLER_H

#include "nextcode/AST/Types.h"
#include "nextcode/Demangling/Demangler.h"
#include "nextcode/Demangling/NamespaceMacros.h"
#include "nextcode/Demangling/TypeDecoder.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include <optional>

namespace nextcode {
 
class TypeDecl;

namespace Demangle {
NEXTCODE_BEGIN_INLINE_NAMESPACE

Type getTypeForMangling(ASTContext &ctx,
                        llvm::StringRef mangling,
                        GenericSignature genericSig=GenericSignature());

TypeDecl *getTypeDeclForMangling(ASTContext &ctx,
                                 llvm::StringRef mangling,
                                 GenericSignature genericSig=GenericSignature());

TypeDecl *getTypeDeclForUSR(ASTContext &ctx,
                            llvm::StringRef usr,
                            GenericSignature genericSig=GenericSignature());

/// An implementation of MetadataReader's BuilderType concept that
/// just finds and builds things in the AST.
class ASTBuilder {
  ASTContext &Ctx;
  Demangle::NodeFactory Factory;

  /// The notional context in which we're writing and type-checking code.
  /// Created lazily.
  DeclContext *NotionalDC = nullptr;

  /// The depth and index of each parameter pack in the current generic
  /// signature. We need this because the mangling for a type parameter
  /// doesn't record whether it is a pack or not; we find the correct
  /// depth and index in this array, and use its pack-ness.
  llvm::SmallVector<std::pair<unsigned, unsigned>, 2> ParameterPacks;

  /// For saving and restoring generic parameters.
  llvm::SmallVector<decltype(ParameterPacks), 2> ParameterPackStack;

  /// This builder doesn't perform "on the fly" substitutions, so we preserve
  /// all pack expansions. We still need an active expansion stack though,
  /// for the dummy implementation of these methods:
  /// - beginPackExpansion()
  /// - advancePackExpansion()
  /// - createExpandedPackElement()
  /// - endPackExpansion()
  llvm::SmallVector<Type, 2> ActivePackExpansions;

public:
  using BuiltType = nextcode::Type;
  using BuiltTypeDecl = nextcode::GenericTypeDecl *; // nominal or type alias
  using BuiltProtocolDecl = nextcode::ProtocolDecl *;
  using BuiltGenericSignature = nextcode::GenericSignature;
  using BuiltRequirement = nextcode::Requirement;
  using BuiltInverseRequirement = nextcode::InverseRequirement;
  using BuiltSubstitutionMap = nextcode::SubstitutionMap;

  static constexpr bool needsToPrecomputeParentGenericContextShapes = false;

  explicit ASTBuilder(ASTContext &ctx, GenericSignature genericSig)
    : Ctx(ctx) {
    for (auto *paramTy : genericSig.getGenericParams()) {
      if (paramTy->isParameterPack())
        ParameterPacks.emplace_back(paramTy->getDepth(), paramTy->getIndex());
    }
  }

  ASTContext &getASTContext() { return Ctx; }
  DeclContext *getNotionalDC();

  Demangle::NodeFactory &getNodeFactory() { return Factory; }

  Type decodeMangledType(NodePointer node, bool forRequirement = true);
  Type createBuiltinType(StringRef builtinName, StringRef mangledName);

  TypeDecl *createTypeDecl(NodePointer node);

  GenericTypeDecl *createTypeDecl(StringRef mangledName, bool &typeAlias);
  
  GenericTypeDecl *createTypeDecl(NodePointer node,
                                  bool &typeAlias);

  ProtocolDecl *createProtocolDecl(NodePointer node);

  Type createNominalType(GenericTypeDecl *decl);

  Type createNominalType(GenericTypeDecl *decl, Type parent);

  Type createTypeAliasType(GenericTypeDecl *decl, Type parent);

  Type createBoundGenericType(GenericTypeDecl *decl, ArrayRef<Type> args);
  
  Type resolveOpaqueType(NodePointer opaqueDescriptor,
                         ArrayRef<ArrayRef<Type>> args,
                         unsigned ordinal);

  Type createBoundGenericType(GenericTypeDecl *decl, ArrayRef<Type> args,
                              Type parent);

  Type createTupleType(ArrayRef<Type> eltTypes, ArrayRef<StringRef> labels);

  Type createPackType(ArrayRef<Type> eltTypes);

  Type createSILPackType(ArrayRef<Type> eltTypes, bool isElementAddress);

  size_t beginPackExpansion(Type countType);

  void advancePackExpansion(size_t index);

  Type createExpandedPackElement(Type patternType);

  void endPackExpansion();

  Type createFunctionType(
      ArrayRef<Demangle::FunctionParam<Type>> params,
      Type output, FunctionTypeFlags flags, ExtendedFunctionTypeFlags extFlags,
      FunctionMetadataDifferentiabilityKind diffKind, Type globalActor,
      Type thrownError);

  Type createImplFunctionType(
      Demangle::ImplParameterConvention calleeConvention,
      Demangle::ImplCoroutineKind coroutineKind,
      ArrayRef<Demangle::ImplFunctionParam<Type>> params,
      ArrayRef<Demangle::ImplFunctionYield<Type>> yields,
      ArrayRef<Demangle::ImplFunctionResult<Type>> results,
      std::optional<Demangle::ImplFunctionResult<Type>> errorResult,
      ImplFunctionTypeFlags flags);

  Type createProtocolCompositionType(ArrayRef<ProtocolDecl *> protocols,
                                     Type superclass,
                                     bool isClassBound,
                                     bool forRequirement = true);

  Type createProtocolTypeFromDecl(ProtocolDecl *protocol);

  Type createConstrainedExistentialType(
      Type base,
      ArrayRef<BuiltRequirement> constraints,
      ArrayRef<BuiltInverseRequirement> inverseRequirements);

  Type createSymbolicExtendedExistentialType(NodePointer shapeNode,
                                             ArrayRef<Type> genArgs);

  Type createExistentialMetatypeType(
      Type instance,
      std::optional<Demangle::ImplMetatypeRepresentation> repr = std::nullopt);

  Type createMetatypeType(
      Type instance,
      std::optional<Demangle::ImplMetatypeRepresentation> repr = std::nullopt);

  void pushGenericParams(ArrayRef<std::pair<unsigned, unsigned>> parameterPacks);
  void popGenericParams();

  Type createGenericTypeParameterType(unsigned depth, unsigned index);

  Type createDependentMemberType(StringRef member, Type base);

  Type createDependentMemberType(StringRef member, Type base,
                                 ProtocolDecl *protocol);

#define REF_STORAGE(Name, ...) \
  Type create##Name##StorageType(Type base);
#include "nextcode/AST/ReferenceStorage.def"

  Type createSILBoxType(Type base);
  using BuiltSILBoxField = llvm::PointerIntPair<Type, 1>;
  using BuiltSubstitution = std::pair<Type, Type>;
  using BuiltLayoutConstraint = nextcode::LayoutConstraint;
  Type createSILBoxTypeWithLayout(
      ArrayRef<BuiltSILBoxField> Fields,
      ArrayRef<BuiltSubstitution> Substitutions,
      ArrayRef<BuiltRequirement> Requirements,
      ArrayRef<BuiltInverseRequirement> inverseRequirements);

  bool isExistential(Type type) {
    return type->isExistentialType();
  }


  Type createObjCClassType(StringRef name);

  Type createBoundGenericObjCClassType(StringRef name, ArrayRef<Type> args);

  ProtocolDecl *createObjCProtocolDecl(StringRef name);

  Type createDynamicSelfType(Type selfType);

  Type createForeignClassType(StringRef mangledName);

  Type getUnnamedForeignClassType();

  Type getOpaqueType();

  Type createOptionalType(Type base);

  Type createArrayType(Type base);

  Type createDictionaryType(Type key, Type value);

  Type createParenType(Type base);

  BuiltGenericSignature
  createGenericSignature(ArrayRef<BuiltType> params,
                         ArrayRef<BuiltRequirement> requirements);

  BuiltSubstitutionMap createSubstitutionMap(BuiltGenericSignature sig,
                                             ArrayRef<BuiltType> replacements);

  Type subst(Type subject, const BuiltSubstitutionMap &Subs) const;

  LayoutConstraint getLayoutConstraint(LayoutConstraintKind kind);
  LayoutConstraint getLayoutConstraintWithSizeAlign(LayoutConstraintKind kind,
                                                    unsigned size,
                                                    unsigned alignment);

  InverseRequirement createInverseRequirement(
      Type subject, InvertibleProtocolKind kind);

private:
  bool validateParentType(TypeDecl *decl, Type parent);
  CanGenericSignature demangleGenericSignature(
      NominalTypeDecl *nominalDecl,
      NodePointer node);
  DeclContext *findDeclContext(NodePointer node);
  ModuleDecl *findModule(NodePointer node);
  Demangle::NodePointer findModuleNode(NodePointer node);

  enum class ForeignModuleKind {
    Imported,
    SynthesizedByImporter
  };

  std::optional<ForeignModuleKind> getForeignModuleKind(NodePointer node);

  GenericTypeDecl *findTypeDecl(DeclContext *dc,
                                Identifier name,
                                Identifier privateDiscriminator,
                                Demangle::Node::Kind kind);
  GenericTypeDecl *findForeignTypeDecl(StringRef name,
                                       StringRef relatedEntityKind,
                                       ForeignModuleKind lookupKind,
                                       Demangle::Node::Kind kind);

  static GenericTypeDecl *getAcceptableTypeDeclCandidate(ValueDecl *decl,
                                              Demangle::Node::Kind kind);
};

NEXTCODE_END_INLINE_NAMESPACE
}  // namespace Demangle

}  // namespace nextcode

#endif  // NEXTCODE_AST_ASTDEMANGLER_H
