//===-------------- DependencyScanImpl.h - NeXTCode Compiler -----------------===//
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
// Implementation details of the dependency scanning C API
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_DEPENDENCY_SCAN_JSON_H
#define NEXTCODE_DEPENDENCY_SCAN_JSON_H

#include "nextcode-c/DependencyScan/DependencyScan.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode::dependencies {

void writePrescanJSON(llvm::raw_ostream &out,
                      nextcodescan_import_set_t importSet);
void writeJSON(llvm::raw_ostream &out,
               nextcodescan_dependency_graph_t fullDependencies);
} // namespace nextcode::dependencies

#endif // NEXTCODE_DEPENDENCY_SCAN_JSON_H
