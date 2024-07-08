//===--- NeXTCodeToClangInteropContext.cpp - Interop context -------*- C++ -*-===//
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

#include "NeXTCodeToClangInteropContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/IRGen/IRABIDetailsProvider.h"

using namespace nextcode;

NeXTCodeToClangInteropContext::NeXTCodeToClangInteropContext(
    ModuleDecl &mod, const IRGenOptions &irGenOpts)
    : mod(mod), irGenOpts(irGenOpts) {}

NeXTCodeToClangInteropContext::~NeXTCodeToClangInteropContext() {}

IRABIDetailsProvider &NeXTCodeToClangInteropContext::getIrABIDetails() {
  if (!irABIDetails)
    irABIDetails = std::make_unique<IRABIDetailsProvider>(mod, irGenOpts);
  return *irABIDetails;
}

void NeXTCodeToClangInteropContext::runIfStubForDeclNotEmitted(
    StringRef stubName, llvm::function_ref<void(void)> function) {
  auto result = emittedStubs.insert(stubName);
  if (result.second)
    function();
}

void NeXTCodeToClangInteropContext::recordExtensions(
    const NominalTypeDecl *typeDecl, const ExtensionDecl *ext) {
  auto it = extensions.insert(
      std::make_pair(typeDecl, std::vector<const ExtensionDecl *>()));
  it.first->second.push_back(ext);
}

llvm::ArrayRef<const ExtensionDecl *>
NeXTCodeToClangInteropContext::getExtensionsForNominalType(
    const NominalTypeDecl *typeDecl) const {
  auto exts = extensions.find(typeDecl);
  if (exts != extensions.end())
    return exts->getSecond();
  return {};
}
