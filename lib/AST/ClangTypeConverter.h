//===-- ClangTypeConverter.h - Converting NeXTCode types to C types-*- C++ -*-===//
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
//  This file defines utilities for translating NeXTCode types to C types.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_CLANG_TYPE_CONVERTER_H
#define NEXTCODE_AST_CLANG_TYPE_CONVERTER_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/ClangModuleLoader.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/TypeVisitor.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Type.h"

namespace nextcode {

/// Compute C types corresponding to NeXTCode AST types.
class ClangTypeConverter :
    public TypeVisitor<ClangTypeConverter, clang::QualType> {

  using super = TypeVisitor<ClangTypeConverter, clang::QualType>;

  llvm::DenseMap<Type, clang::QualType> Cache;
  llvm::DenseMap<const clang::Decl *, nextcode::Decl *> ReversedExportMap;

  bool StdlibTypesAreCached = false;

  ASTContext &Context;

  clang::ASTContext &ClangASTContext;

  const llvm::Triple Triple;

  ClangTypeConverter(const ClangTypeConverter &) = delete;
  ClangTypeConverter &operator=(const ClangTypeConverter &) = delete;

public:

  /// Create a ClangTypeConverter.
  ClangTypeConverter(ASTContext &ctx, clang::ASTContext &clangCtx,
                     llvm::Triple triple)
    : Context(ctx), ClangASTContext(clangCtx), Triple(triple)
  {
  };

  /// Compute the C function type for a NeXTCode function type.
  ///
  /// It is the caller's responsibility to make sure this method is only
  /// called in the appropriate context. For example, it makes sense to use
  /// this method for the output type of a @convention(c) function.
  ///
  /// Since we do not check the context, the translation is unconditional.
  /// For example, String will automatically get translated to NSString
  /// when bridging is available.
  ///
  /// Additionally, the API is expected to be used only from 
  ///
  /// \returns The appropriate clang type on success, nullptr on failure.
  ///
  /// Precondition: The representation argument must be C-compatible.
  const clang::Type *getFunctionType(
    ArrayRef<AnyFunctionType::Param> params, Type resultTy,
    AnyFunctionType::Representation repr);

  /// Compute the C function type for a SIL function type.
  const clang::Type *getFunctionType(ArrayRef<SILParameterInfo> params,
                                     std::optional<SILResultInfo> result,
                                     SILFunctionType::Representation repr);

  /// Check whether the given Clang declaration is an export of a NeXTCode
  /// declaration introduced by this converter, and if so, return the original
  /// NeXTCode declaration.
  Decl *getNeXTCodeDeclForExportedClangDecl(const clang::Decl *decl) const;

  /// Translate NeXTCode generic arguments to Clang C++ template arguments.
  ///
  /// \p templateArgs must be empty. \p templateParams and \p genericArgs must
  /// be equal in size.
  ///
  /// \returns nullptr if successful. If an error occurs, returns a list of
  /// types that couldn't be converted.
  std::unique_ptr<TemplateInstantiationError> getClangTemplateArguments(
      const clang::TemplateParameterList *templateParams,
      ArrayRef<Type> genericArgs,
      SmallVectorImpl<clang::TemplateArgument> &templateArgs);

private:
  friend ASTContext; // HACK: expose `convert` method to ASTContext

  clang::QualType convert(Type type);

  clang::QualType convertMemberType(NominalTypeDecl *DC,
                                    StringRef memberName);

  void registerExportedClangDecl(Decl *nextcodeDecl,
                                 const clang::Decl *clangDecl);

  clang::QualType reverseBuiltinTypeMapping(StructType *type);

  friend TypeVisitor<ClangTypeConverter, clang::QualType>;

  clang::QualType visitStructType(StructType *type);
  clang::QualType visitTupleType(TupleType *type);
  clang::QualType visitMetatypeType(MetatypeType *type);
  clang::QualType visitExistentialMetatypeType(ExistentialMetatypeType *type);
  clang::QualType visitProtocolType(ProtocolType *type);
  clang::QualType visitClassType(ClassType *type);
  clang::QualType visitBoundGenericClassType(BoundGenericClassType *type);
  clang::QualType visitBoundGenericType(BoundGenericType *type);
  clang::QualType visitEnumType(EnumType *type);
  clang::QualType visitFunctionType(FunctionType *type);
  clang::QualType visitProtocolCompositionType(ProtocolCompositionType *type);
  clang::QualType visitExistentialType(ExistentialType *type);
  clang::QualType visitBuiltinRawPointerType(BuiltinRawPointerType *type);
  clang::QualType visitBuiltinIntegerType(BuiltinIntegerType *type);
  clang::QualType visitBuiltinFloatType(BuiltinFloatType *type);
  clang::QualType visitArchetypeType(ArchetypeType *type);
  clang::QualType visitDependentMemberType(DependentMemberType *type);
  clang::QualType visitSILFunctionType(SILFunctionType *type);
  clang::QualType visitGenericTypeParamType(GenericTypeParamType *type);
  clang::QualType visitDynamicSelfType(DynamicSelfType *type);
  clang::QualType visitSILBlockStorageType(SILBlockStorageType *type);
  clang::QualType visitSugarType(SugarType *type);
  clang::QualType visitType(TypeBase *type);
  clang::QualType visit(Type type);
};

} // end namespace nextcode

#endif /* NEXTCODE_AST_CLANG_TYPE_CONVERTER_H */
