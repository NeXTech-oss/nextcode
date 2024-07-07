//===--- PassesFwd.h - Creation functions for LLVM passes -------*- C++ -*-===//
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

#ifndef NEXTCODE_LLVMPASSES_PASSESFWD_H
#define NEXTCODE_LLVMPASSES_PASSESFWD_H

namespace llvm {
  class ModulePass;
  class FunctionPass;
  class ImmutablePass;
  class PassRegistry;

  void initializeNeXTCodeAAWrapperPassPass(PassRegistry &);
  void initializeNeXTCodeARCOptPass(PassRegistry &);
  void initializeNeXTCodeARCContractPass(PassRegistry &);
  void initializeInlineTreePrinterPass(PassRegistry &);
  void initializeLegacyNeXTCodeMergeFunctionsPass(PassRegistry &);
}

namespace nextcode {
  llvm::FunctionPass *createNeXTCodeARCOptPass();
  llvm::FunctionPass *createNeXTCodeARCContractPass();
  llvm::ModulePass *createInlineTreePrinterPass();
  llvm::ModulePass *createLegacyNeXTCodeMergeFunctionsPass(bool ptrAuthEnabled,
                                                        unsigned ptrAuthKey);
  llvm::ImmutablePass *createNeXTCodeAAWrapperPass();
} // end namespace nextcode

#endif
