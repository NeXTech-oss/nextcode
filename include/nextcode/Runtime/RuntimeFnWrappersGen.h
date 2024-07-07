//===--- RuntimeFnWrappersGen.h - LLVM IR Generation for runtime functions ===//
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
//  Helper functions providing the LLVM IR generation for runtime entry points.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_RUNTIME_RUNTIMEFNWRAPPERSGEN_H
#define NEXTCODE_RUNTIME_RUNTIMEFNWRAPPERSGEN_H

#include "nextcode/SIL/RuntimeEffect.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/IR/Module.h"

namespace nextcode {

class AvailabilityContext;
class ASTContext;

namespace irgen {
class IRGenModule;
}

enum class RuntimeAvailability {
  AlwaysAvailable,
  AvailableByCompatibilityLibrary,
  ConditionallyAvailable
};

/// Generate an llvm declaration for a runtime entry with a
/// given name, return types, argument types, attributes and
/// a calling convention.
llvm::Constant *getRuntimeFn(llvm::Module &Module, llvm::Constant *&cache,
                             char const *name, llvm::CallingConv::ID cc,
                             RuntimeAvailability availability,
                             llvm::ArrayRef<llvm::Type *> retTypes,
                             llvm::ArrayRef<llvm::Type *> argTypes,
                             llvm::ArrayRef<llvm::Attribute::AttrKind> attrs,
                             llvm::ArrayRef<llvm::MemoryEffects> memEffects,
                             irgen::IRGenModule *IGM = nullptr);

llvm::FunctionType *getRuntimeFnType(llvm::Module &Module,
                             llvm::ArrayRef<llvm::Type *> retTypes,
                             llvm::ArrayRef<llvm::Type *> argTypes);

} // namespace nextcode
#endif
