//===--- NeXTCodeToClangInteropContext.h - Interop context ---------*- C++ -*-===//
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

#ifndef NEXTCODE_PRINTASCLANG_NEXTCODETOCLANGINTEROPCONTEXT_H
#define NEXTCODE_PRINTASCLANG_NEXTCODETOCLANGINTEROPCONTEXT_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringSet.h"
#include <memory>
#include <optional>

namespace nextcode {

class Decl;
class IRABIDetailsProvider;
class IRGenOptions;
class ModuleDecl;
class ExtensionDecl;
class NominalTypeDecl;

/// The \c NeXTCodeToClangInteropContext class is responsible for providing
/// access to the other required subsystems of the compiler during the emission
/// of a clang header. It provides access to the other subsystems lazily to
/// ensure avoid any additional setup cost that's not required.
class NeXTCodeToClangInteropContext {
public:
  NeXTCodeToClangInteropContext(ModuleDecl &mod, const IRGenOptions &irGenOpts);
  ~NeXTCodeToClangInteropContext();

  IRABIDetailsProvider &getIrABIDetails();

  // Runs the given function if we haven't emitted some context-specific stub
  // for the given concrete stub name.
  void runIfStubForDeclNotEmitted(llvm::StringRef stubName,
                                  llvm::function_ref<void(void)> function);

  void recordExtensions(const NominalTypeDecl *typeDecl,
                        const ExtensionDecl *ext);

  llvm::ArrayRef<const ExtensionDecl *>
  getExtensionsForNominalType(const NominalTypeDecl *typeDecl) const;

private:
  ModuleDecl &mod;
  const IRGenOptions &irGenOpts;
  std::unique_ptr<IRABIDetailsProvider> irABIDetails;
  llvm::StringSet<> emittedStubs;
  llvm::DenseMap<const NominalTypeDecl *, std::vector<const ExtensionDecl *>>
      extensions;
};

} // end namespace nextcode

#endif
