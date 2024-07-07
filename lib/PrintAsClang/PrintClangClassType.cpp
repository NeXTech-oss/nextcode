//===--- PrintClangClassType.cpp - Print class types in C/C++ ---*- C++ -*-===//
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

#include "PrintClangClassType.h"
#include "ClangSyntaxPrinter.h"
#include "DeclAndTypePrinter.h"
#include "PrintClangValueType.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/IRGen/Linking.h"

using namespace nextcode;

void ClangClassTypePrinter::printClassTypeDecl(
    const ClassDecl *typeDecl, llvm::function_ref<void(void)> bodyPrinter,
    DeclAndTypePrinter &declAndTypePrinter) {
  auto printCxxImplClassName = ClangValueTypePrinter::printCxxImplClassName;

  ClangSyntaxPrinter printer(os);

  auto typeMetadataFunc = irgen::LinkEntity::forTypeMetadataAccessFunction(
      typeDecl->getDeclaredType()->getCanonicalType());
  std::string typeMetadataFuncName = typeMetadataFunc.mangleAsString();

  // Print out a forward declaration of the "hidden" _impl class.
  printer.printNamespace(cxx_synthesis::getCxxImplNamespaceName(),
                         [&](raw_ostream &os) {
                           os << "class";
                           declAndTypePrinter.printAvailability(os, typeDecl);
                           os << ' ';
                           printCxxImplClassName(os, typeDecl);
                           os << ";\n";
                           // Print out special functions, like functions that
                           // access type metadata.
                           printer.printCTypeMetadataTypeFunction(
                               typeDecl, typeMetadataFuncName, {});
                         });

  std::string baseClassName;
  std::string baseClassQualifiedName;

  if (auto *parentClass = typeDecl->getSuperclassDecl()) {
    llvm::raw_string_ostream baseNameOS(baseClassName);
    ClangSyntaxPrinter(baseNameOS).printBaseName(parentClass);
    llvm::raw_string_ostream baseQualNameOS(baseClassQualifiedName);
    ClangSyntaxPrinter(baseQualNameOS)
        .printModuleNamespaceQualifiersIfNeeded(parentClass->getModuleContext(),
                                                typeDecl->getModuleContext());
    baseQualNameOS << baseNameOS.str();
  } else {
    baseClassName = "RefCountedClass";
    baseClassQualifiedName = "nextcode::_impl::RefCountedClass";
  }

  os << "class";
  declAndTypePrinter.printAvailability(os, typeDecl);
  ClangSyntaxPrinter(os).printSymbolUSRAttribute(typeDecl);
  os << ' ';
  printer.printBaseName(typeDecl);
  if (typeDecl->isFinal())
    os << " final";
  os << " : public " << baseClassQualifiedName;
  os << " {\n";
  os << "public:\n";

  os << "  using " << baseClassName << "::" << baseClassName << ";\n";
  os << "  using " << baseClassName << "::operator=;\n";
  bodyPrinter();
  os << "protected:\n";
  os << "  ";
  printer.printInlineForThunk();
  printer.printBaseName(typeDecl);
  os << "(void * _Nonnull ptr) noexcept : " << baseClassName << "(ptr) {}\n";
  os << "private:\n";
  os << "  friend class " << cxx_synthesis::getCxxImplNamespaceName() << "::";
  printCxxImplClassName(os, typeDecl);
  os << ";\n";

  printer.printNeXTCodeMangledNameForDebugger(typeDecl);

  os << "};\n\n";

  // Print out the "hidden" _impl class.
  printer.printNamespace(
      cxx_synthesis::getCxxImplNamespaceName(), [&](raw_ostream &os) {
        os << "class";
        declAndTypePrinter.printAvailability(os, typeDecl);
        os << ' ';
        printCxxImplClassName(os, typeDecl);
        os << " {\n";
        os << "public:\n";
        os << "static ";
        printer.printInlineForThunk();
        printer.printBaseName(typeDecl);
        os << " makeRetained(void * _Nonnull ptr) noexcept { return ";
        printer.printBaseName(typeDecl);
        os << "(ptr); }\n";
        os << "};\n";
      });

  ClangValueTypePrinter::printTypeGenericTraits(
      os, typeDecl, typeMetadataFuncName, /*genericRequirements=*/{},
      typeDecl->getModuleContext(), declAndTypePrinter);
}

void ClangClassTypePrinter::printClassTypeReturnScaffold(
    raw_ostream &os, const ClassDecl *type, const ModuleDecl *moduleContext,
    llvm::function_ref<void(void)> bodyPrinter) {
  os << "  return ";
  ClangSyntaxPrinter(os).printModuleNamespaceQualifiersIfNeeded(
      type->getModuleContext(), moduleContext);
  os << cxx_synthesis::getCxxImplNamespaceName() << "::";
  ClangValueTypePrinter::printCxxImplClassName(os, type);
  os << "::makeRetained(";
  bodyPrinter();
  os << ");\n";
}

void ClangClassTypePrinter::printParameterCxxtoCUseScaffold(
    raw_ostream &os, const ClassDecl *type, const ModuleDecl *moduleContext,
    llvm::function_ref<void(void)> bodyPrinter, bool isInOut) {
  if (isInOut)
    os << '&';
  os << "::nextcode::" << cxx_synthesis::getCxxImplNamespaceName()
     << "::_impl_RefCountedClass"
     << "::getOpaquePointer";
  if (isInOut)
    os << "Ref";
  os << '(';
  bodyPrinter();
  os << ')';
}
