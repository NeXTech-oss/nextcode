//===--- PrintClangValueType.h - Printer for C/C++ value types --*- C++ -*-===//
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

#ifndef NEXTCODE_PRINTASCLANG_PRINTCLANGVALUETYPE_H
#define NEXTCODE_PRINTASCLANG_PRINTCLANGVALUETYPE_H

#include "OutputLanguageMode.h"
#include "nextcode/AST/Type.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/IRGen/GenericRequirement.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {

class DeclAndTypePrinter;
class ModuleDecl;
class NominalTypeDecl;
class PrimitiveTypeMapping;
class StructDecl;
class NeXTCodeToClangInteropContext;

/// Responsible for printing a NeXTCode struct or enum decl or in C or C++ mode, to
/// be included in a NeXTCode module's generated clang header.
class ClangValueTypePrinter {
public:
  ClangValueTypePrinter(raw_ostream &os, raw_ostream &cPrologueOS,
                        NeXTCodeToClangInteropContext &interopContext)
      : os(os), cPrologueOS(cPrologueOS), interopContext(interopContext) {}

  /// Print the C++ class definition that
  /// corresponds to the given structure or enum declaration.
  void printValueTypeDecl(const NominalTypeDecl *typeDecl,
                          llvm::function_ref<void(void)> bodyPrinter,

                          DeclAndTypePrinter &declAndTypePrinter);

  /// Print the use of a C++ struct/enum parameter value as it's passed to the
  /// underlying C function that represents the native NeXTCode function.
  void printParameterCxxToCUseScaffold(
      const ModuleDecl *moduleContext, llvm::function_ref<void()> typePrinter,
      llvm::function_ref<void()> cxxParamPrinter, bool isSelf);

  enum class TypeUseKind {
    // The name of the C++ class that corresponds to the NeXTCode value type (with
    // any qualifiers).
    CxxTypeName,
    // The name of the C++ _impl class that corresponds to the NeXTCode value type
    // (with any qualifiers).
    CxxImplTypeName
  };

  /// Print the return type that refers to a NeXTCode struct type in C/C++.
  void printValueTypeReturnType(const NominalTypeDecl *typeDecl,
                                OutputLanguageMode outputLang,
                                TypeUseKind typeUse,
                                const ModuleDecl *moduleContext);

  /// Print the supporting code  that's required to indirectly return a C++
  /// class that represents a NeXTCode value type as it's being indirectly passed
  /// from the C function that represents the native NeXTCode function.
  void
  printValueTypeReturnScaffold(const NominalTypeDecl *typeDecl,
                               const ModuleDecl *moduleContext,
                               llvm::function_ref<void()> typePrinter,
                               llvm::function_ref<void(StringRef)> bodyPrinter);

  /// Print out the C++ type name of the implementation class that provides
  /// hidden access to the private class APIs.
  static void printCxxImplClassName(raw_ostream &os,
                                    const NominalTypeDecl *type);

  /// Print a variable that can be used to access type's metadata function
  static void printMetadataAccessAsVariable(
      raw_ostream &os, StringRef metadataFuncName,
      ArrayRef<GenericRequirement> genericRequirements, int indent = 4,
      StringRef varName = "metadata");

  /// Print a variable that can be used to access type's metadata function and
  /// value witness table
  static void printValueWitnessTableAccessAsVariable(
      raw_ostream &os, StringRef metadataFuncName,
      ArrayRef<GenericRequirement> genericRequirements, int indent = 4,
      StringRef metadataVarName = "metadata",
      StringRef vwTableVarName = "vwTable");

  static void printTypeGenericTraits(
      raw_ostream &os, const TypeDecl *typeDecl, StringRef typeMetadataFuncName,
      ArrayRef<GenericRequirement> typeMetadataFuncRequirements,
      const ModuleDecl *moduleContext, DeclAndTypePrinter &declAndTypePrinter,
      bool isOpaqueLayout = false);

  static void printTypePrecedingGenericTraits(raw_ostream &os,
                                              const NominalTypeDecl *typeDecl,
                                              const ModuleDecl *moduleContext);

  static void forwardDeclType(raw_ostream &os, const NominalTypeDecl *typeDecl,
                              DeclAndTypePrinter &declAndTypePrinter);

  /// Print out the type traits that allow a C++ type be used a NeXTCode generic
  /// context.
  static void
  printClangTypeNeXTCodeGenericTraits(raw_ostream &os, const TypeDecl *typeDecl,
                                   const ModuleDecl *moduleContext,
                                   DeclAndTypePrinter &declAndTypePrinter);

private:
  raw_ostream &os;
  raw_ostream &cPrologueOS;
  NeXTCodeToClangInteropContext &interopContext;
};

} // end namespace nextcode

#endif
