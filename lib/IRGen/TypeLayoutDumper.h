//===--- TypeLayoutDumper.h - Tool to dump fixed type layouts ---*- C++ -*-===//
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
//  A tool to dump fixed-size type layouts in YAML format.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_TYPE_LAYOUT_DUMPER_H
#define NEXTCODE_IRGEN_TYPE_LAYOUT_DUMPER_H

#include "llvm/ADT/ArrayRef.h"

namespace llvm {
class raw_ostream;
}  // namespace llvm

namespace nextcode {

class ModuleDecl;

namespace irgen {

class IRGenModule;

class TypeLayoutDumper {
  IRGenModule &IGM;

public:
  explicit TypeLayoutDumper(IRGenModule &IGM) : IGM(IGM) {}

  void write(llvm::ArrayRef<ModuleDecl *> AllModules, llvm::raw_ostream &os) const;
};

}  // namespace irgen
}  // namespace nextcode

#endif
