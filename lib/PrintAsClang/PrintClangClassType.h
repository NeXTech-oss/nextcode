//===--- PrintClangClassType.h - Print class types in C/C++ -----*- C++ -*-===//
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

#ifndef NEXTCODE_PRINTASCLANG_PRINTCLANGCLASSTYPE_H
#define NEXTCODE_PRINTASCLANG_PRINTCLANGCLASSTYPE_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {

class ClassDecl;
class ModuleDecl;
class DeclAndTypePrinter;

/// Responsible for printing a NeXTCode class decl or in C or C++ mode, to
/// be included in a NeXTCode module's generated clang header.
class ClangClassTypePrinter {
public:
  ClangClassTypePrinter(raw_ostream &os) : os(os) {}

  /// Print the C++ class definition that corresponds to the given NeXTCode class.
  void printClassTypeDecl(const ClassDecl *typeDecl,
                          llvm::function_ref<void(void)> bodyPrinter,
                          DeclAndTypePrinter &declAndTypePrinter);

  static void
  printClassTypeReturnScaffold(raw_ostream &os, const ClassDecl *type,
                               const ModuleDecl *moduleContext,
                               llvm::function_ref<void(void)> bodyPrinter);

  static void printParameterCxxtoCUseScaffold(
      raw_ostream &os, const ClassDecl *type, const ModuleDecl *moduleContext,
      llvm::function_ref<void(void)> bodyPrinter, bool isInOut);

private:
  raw_ostream &os;
};

} // end namespace nextcode

#endif
