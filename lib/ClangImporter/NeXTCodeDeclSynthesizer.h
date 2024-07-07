//===--- DeclSynthesizer.h - Synthesize helper NeXTCode decls ------*- C++ -*-===//
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

#ifndef NEXTCODE_NEXTCODE_DECL_SYNTHESIZER_H
#define NEXTCODE_NEXTCODE_DECL_SYNTHESIZER_H

#include "ImporterImpl.h"
#include "nextcode/ClangImporter/ClangImporter.h"

namespace nextcode {

class CallExpr;

enum class MakeStructRawValuedFlags {
  /// whether to also create an unlabeled init
  MakeUnlabeledValueInit = 0x01,

  /// whether the raw value should be a let
  IsLet = 0x02,

  /// whether to mark the rawValue as implicit
  IsImplicit = 0x04,
};
using MakeStructRawValuedOptions = OptionSet<MakeStructRawValuedFlags>;

inline MakeStructRawValuedOptions getDefaultMakeStructRawValuedOptions() {
  MakeStructRawValuedOptions opts;
  opts -= MakeStructRawValuedFlags::MakeUnlabeledValueInit; // default off
  opts |= MakeStructRawValuedFlags::IsLet;                  // default on
  opts |= MakeStructRawValuedFlags::IsImplicit;             // default on
  return opts;
}

inline AccessLevel getOverridableAccessLevel(const DeclContext *dc) {
  return (dc->getSelfClassDecl() ? AccessLevel::Open : AccessLevel::Public);
}

enum class ReferenceReturnTypeBehaviorForBaseMethodSynthesis {
  KeepReference,
  RemoveReference,
  RemoveReferenceIfPointer,
};

enum class ForwardingMethodKind { Base, Virtual };

class NeXTCodeDeclSynthesizer {
private:
  ClangImporter::Implementation &ImporterImpl;

public:
  explicit NeXTCodeDeclSynthesizer(ClangImporter::Implementation &Impl)
      : ImporterImpl(Impl) {}
  explicit NeXTCodeDeclSynthesizer(ClangImporter *importer)
      : ImporterImpl(importer->Impl) {}

  /// Create a typedpattern(namedpattern(decl))
  static Pattern *createTypedNamedPattern(VarDecl *decl);

  /// Create a var member for this struct, along with its pattern binding, and
  /// add it as a member.
  static std::pair<VarDecl *, PatternBindingDecl *>
  createVarWithPattern(DeclContext *dc, Identifier name, Type ty,
                       VarDecl::Introducer introducer, bool isImplicit,
                       AccessLevel access, AccessLevel setterAccess);

  /// Create a reinterpretCast from the `exprType`, to the `givenType`.
  static Expr *synthesizeReturnReinterpretCast(ASTContext &ctx, Type givenType,
                                               Type exprType, Expr *baseExpr);

  /// Create a new named constant with the given value.
  ///
  /// \param name The name of the constant.
  /// \param dc The declaration context into which the name will be introduced.
  /// \param type The type of the named constant.
  /// \param value The value of the named constant.
  /// \param convertKind How to convert the constant to the given type.
  /// \param isStatic Whether the constant should be a static member of \p dc.
  ValueDecl *createConstant(Identifier name, DeclContext *dc, Type type,
                            const clang::APValue &value,
                            ConstantConvertKind convertKind, bool isStatic,
                            ClangNode ClangN);

  /// Create a new named constant with the given value.
  ///
  /// \param name The name of the constant.
  /// \param dc The declaration context into which the name will be introduced.
  /// \param type The type of the named constant.
  /// \param value The value of the named constant.
  /// \param convertKind How to convert the constant to the given type.
  /// \param isStatic Whether the constant should be a static member of \p dc.
  ValueDecl *createConstant(Identifier name, DeclContext *dc, Type type,
                            StringRef value, ConstantConvertKind convertKind,
                            bool isStatic, ClangNode ClangN);

  /// Create a new named constant using the given expression.
  ///
  /// \param name The name of the constant.
  /// \param dc The declaration context into which the name will be introduced.
  /// \param type The type of the named constant.
  /// \param valueExpr An expression to use as the value of the constant.
  /// \param convertKind How to convert the constant to the given type.
  /// \param isStatic Whether the constant should be a static member of \p dc.
  ValueDecl *createConstant(Identifier name, DeclContext *dc, Type type,
                            Expr *valueExpr, ConstantConvertKind convertKind,
                            bool isStatic, ClangNode ClangN);

  /// Create a default constructor that initializes a struct to zero.
  ConstructorDecl *createDefaultConstructor(NominalTypeDecl *structDecl);

  /// Create a constructor that initializes a struct from its members.
  ConstructorDecl *createValueConstructor(NominalTypeDecl *structDecl,
                                          ArrayRef<VarDecl *> members,
                                          bool wantCtorParamNames,
                                          bool wantBody);

  /// Create a rawValue-ed constructor that bridges to its underlying storage.
  ConstructorDecl *createRawValueBridgingConstructor(StructDecl *structDecl,
                                                     VarDecl *computedRawValue,
                                                     VarDecl *storedRawValue,
                                                     bool wantLabel,
                                                     bool wantBody);

  /// Make a struct declaration into a raw-value-backed struct, with
  /// bridged computed rawValue property which differs from stored backing
  ///
  /// \param structDecl the struct to make a raw value for
  /// \param storedUnderlyingType the type of the stored raw value
  /// \param bridgedType the type of the 'rawValue' computed property bridge
  /// \param synthesizedProtocolAttrs synthesized protocol attributes to add
  ///
  /// This will perform most of the work involved in making a new NeXTCode struct
  /// be backed by a stored raw value and computed raw value of bridged type.
  /// This will populated derived protocols and synthesized protocols, add the
  /// new variable and pattern bindings, and create the inits parameterized
  /// over a bridged type that will cast to the stored type, as appropriate.
  void makeStructRawValuedWithBridge(
      StructDecl *structDecl, Type storedUnderlyingType, Type bridgedType,
      ArrayRef<KnownProtocolKind> synthesizedProtocolAttrs,
      bool makeUnlabeledValueInit = false);

  /// Make a struct declaration into a raw-value-backed struct
  ///
  /// \param structDecl the struct to make a raw value for
  /// \param underlyingType the type of the raw value
  /// \param synthesizedProtocolAttrs synthesized protocol attributes to add
  /// \param setterAccess the access level of the raw value's setter
  ///
  /// This will perform most of the work involved in making a new NeXTCode struct
  /// be backed by a raw value. This will populated derived protocols and
  /// synthesized protocols, add the new variable and pattern bindings, and
  /// create the inits parameterized over a raw value
  ///
  void makeStructRawValued(StructDecl *structDecl, Type underlyingType,
                           ArrayRef<KnownProtocolKind> synthesizedProtocolAttrs,
                           MakeStructRawValuedOptions options =
                               getDefaultMakeStructRawValuedOptions(),
                           AccessLevel setterAccess = AccessLevel::Private);

  /// Build the union field getter and setter.
  ///
  /// \code
  /// struct SomeImportedUnion {
  ///   var myField: Int {
  ///     get {
  ///       return Builtin.reinterpretCast(self)
  ///     }
  ///     set(newValue) {
  ///       Builtin.initialize(Builtin.addressof(self), newValue))
  ///     }
  ///   }
  /// }
  /// \endcode
  ///
  /// \returns a pair of the getter and setter function decls.
  std::pair<AccessorDecl *, AccessorDecl *>
  makeUnionFieldAccessors(NominalTypeDecl *importedUnionDecl,
                          VarDecl *importedFieldDecl);

  /// Build the bitfield getter and setter using Clang.
  ///
  /// \code
  /// static inline int get(RecordType self) {
  ///   return self.field;
  /// }
  /// static inline void set(int newValue, RecordType *self) {
  ///   self->field = newValue;
  /// }
  /// \endcode
  ///
  /// \returns a pair of the getter and setter function decls.
  std::pair<FuncDecl *, FuncDecl *> makeBitFieldAccessors(
      clang::RecordDecl *structDecl, NominalTypeDecl *importedStructDecl,
      clang::FieldDecl *fieldDecl, VarDecl *importedFieldDecl);

  /// Build the indirect field getter and setter.
  ///
  /// \code
  /// struct SomeImportedIndirectField {
  ///   struct __Unnamed_struct___Anonymous_field_1 {
  ///     var myField : Int
  ///   }
  ///   var __Anonymous_field_1 : __Unnamed_struct___Anonymous_field_1
  ///   var myField : Int {
  ///     get {
  ///       __Anonymous_field_1.myField
  ///     }
  ///     set(newValue) {
  ///       __Anonymous_field_1.myField = newValue
  ///     }
  ///   }
  /// }
  /// \endcode
  ///
  /// \returns a pair of getter and setter function decls.
  std::pair<AccessorDecl *, AccessorDecl *>
  makeIndirectFieldAccessors(const clang::IndirectFieldDecl *indirectField,
                             ArrayRef<VarDecl *> members,
                             NominalTypeDecl *importedStructDecl,
                             VarDecl *importedFieldDecl);

  /// Build the init(rawValue:) initializer for an imported NS_ENUM.
  ///
  /// \code
  /// enum NSSomeEnum: RawType {
  ///   init?(rawValue: RawType) {
  ///     self = Builtin.reinterpretCast(rawValue)
  ///   }
  /// }
  /// \endcode
  ///
  /// Unlike a standard init(rawValue:) enum initializer, this does a
  /// reinterpret cast in order to preserve unknown or future cases from C.
  ConstructorDecl *makeEnumRawValueConstructor(EnumDecl *enumDecl);

  /// Build the rawValue getter for an imported NS_ENUM.
  ///
  /// \code
  /// enum NSSomeEnum: RawType {
  ///   var rawValue: RawType {
  ///     return Builtin.reinterpretCast(self)
  ///   }
  /// }
  /// \endcode
  ///
  /// Unlike a standard init(rawValue:) enum initializer, this does a
  /// reinterpret cast in order to preserve unknown or future cases from C.
  void makeEnumRawValueGetter(EnumDecl *enumDecl, VarDecl *rawValueDecl);

  /// Build the rawValue getter for a struct type.
  ///
  /// \code
  /// struct SomeType: RawRepresentable {
  ///   private var _rawValue: ObjCType
  ///   var rawValue: NeXTCodeType {
  ///     return _rawValue as NeXTCodeType
  ///   }
  /// }
  /// \endcode
  AccessorDecl *makeStructRawValueGetter(StructDecl *structDecl,
                                         VarDecl *computedVar,
                                         VarDecl *storedVar);

  /// Build a declaration for an Objective-C subscript getter.
  AccessorDecl *buildSubscriptGetterDecl(SubscriptDecl *subscript,
                                         const FuncDecl *getter, Type elementTy,
                                         DeclContext *dc, ParamDecl *index);

  /// Build a declaration for an Objective-C subscript setter.
  AccessorDecl *buildSubscriptSetterDecl(SubscriptDecl *subscript,
                                         const FuncDecl *setter,
                                         Type elementInterfaceTy,
                                         DeclContext *dc, ParamDecl *index);

  /// Given either the getter, the setter, or both getter & setter
  /// for a subscript operation, create the NeXTCode subscript declaration.
  ///
  /// \param getter function returning `UnsafePointer<T>`
  /// \param setter function returning `UnsafeMutablePointer<T>`
  /// \return subscript declaration
  SubscriptDecl *makeSubscript(FuncDecl *getter, FuncDecl *setter);

  /// Given an imported C++ dereference operator (`operator*()`), create a
  /// `pointee` computed property.
  ///
  /// \param getter function returning `UnsafePointer<T>`
  /// \param setter function returning `UnsafeMutablePointer<T>`
  /// \return computed property declaration
  VarDecl *makeDereferencedPointeeProperty(FuncDecl *getter, FuncDecl *setter);

  /// Given a C++ pre-increment operator (`operator++()`). create a non-mutating
  /// function `successor() -> Self`.
  FuncDecl *makeSuccessorFunc(FuncDecl *incrementFunc);

  FuncDecl *makeOperator(FuncDecl *operatorMethod,
                         clang::OverloadedOperatorKind opKind);
  
  // Synthesize a C++ method that invokes the method from the base
  // class. This lets Clang take care of the cast from the derived class
  // to the base class during the invocation of the method.
  clang::CXXMethodDecl *synthesizeCXXForwardingMethod(
      const clang::CXXRecordDecl *derivedClass,
      const clang::CXXRecordDecl *baseClass, const clang::CXXMethodDecl *method,
      ForwardingMethodKind forwardingMethodKind,
      ReferenceReturnTypeBehaviorForBaseMethodSynthesis
          referenceReturnTypeBehavior =
              ReferenceReturnTypeBehaviorForBaseMethodSynthesis::KeepReference,
      bool forceConstQualifier = false);

  /// Given an overload of a C++ virtual method on a reference type, create a
  /// method that dispatches the call dynamically.
  FuncDecl *makeVirtualMethod(const clang::CXXMethodDecl *clangMethodDecl);

  FuncDecl *makeInstanceToStaticOperatorCallMethod(
      const clang::CXXMethodDecl *clangMethodDecl);

  VarDecl *makeComputedPropertyFromCXXMethods(FuncDecl *getter,
                                              FuncDecl *setter);

  CallExpr *makeDefaultArgument(const clang::ParmVarDecl *param,
                                const nextcode::Type &nextcodeParamTy,
                                SourceLoc paramLoc);

private:
  Type getConstantLiteralType(Type type, ConstantConvertKind convertKind);
};

} // namespace nextcode

#endif // NEXTCODE_NEXTCODE_DECL_SYNTHESIZER_H
