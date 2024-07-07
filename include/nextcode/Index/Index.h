//===--- Index.h - NeXTCode Indexing -------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_INDEX_INDEX_H
#define NEXTCODE_INDEX_INDEX_H

#include "nextcode/Index/IndexDataConsumer.h"

namespace nextcode {
class ModuleDecl;
class SourceFile;
class DeclContext;

namespace index {

void indexDeclContext(DeclContext *DC, IndexDataConsumer &consumer);
void indexSourceFile(SourceFile *SF, IndexDataConsumer &consumer);
void indexModule(ModuleDecl *module, IndexDataConsumer &consumer);
bool printDisplayName(const nextcode::ValueDecl *D, llvm::raw_ostream &OS);

} // end namespace index
} // end namespace nextcode

#endif // NEXTCODE_INDEX_INDEX_H
