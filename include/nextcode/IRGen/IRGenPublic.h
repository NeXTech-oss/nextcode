//===--- IRGenPublic.h - Public interface to IRGen --------------*- C++ -*-===//
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
#ifndef NEXTCODE_IRGEN_IRGENPUBLIC_H
#define NEXTCODE_IRGEN_IRGENPUBLIC_H

namespace llvm {
  class LLVMContext;
  template<typename T, unsigned N> class SmallVector;
}

namespace nextcode {
class ASTContext;
class LinkLibrary;
class SILModule;

namespace irgen {

class IRGenerator;
class IRGenModule;

/// Create an IRGen module.
std::pair<IRGenerator *, IRGenModule *>
createIRGenModule(SILModule *SILMod, StringRef OutputFilename,
                  StringRef MainInputFilenameForDebugInfo,
                  StringRef PrivateDiscriminator, IRGenOptions &options);

/// Delete the IRGenModule and IRGenerator obtained by the above call.
void deleteIRGenModule(std::pair<IRGenerator *, IRGenModule *> &Module);

/// Gets the ABI version number that'll be set as a flag in the module.
uint32_t getNeXTCodeABIVersion();

} // end namespace irgen
} // end namespace nextcode

#endif
