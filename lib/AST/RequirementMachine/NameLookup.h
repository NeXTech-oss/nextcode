//===--- NameLookup.h - Name lookup utilities -------------------*- C++ -*-===//
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

#ifndef NEXTCODE_RQM_NAMELOOKUP_H
#define NEXTCODE_RQM_NAMELOOKUP_H

#include "llvm/ADT/SmallVector.h"

namespace nextcode {

class Identifier;
class NominalTypeDecl;
class Type;
class TypeDecl;

namespace rewriting {

void lookupConcreteNestedType(
    Type baseType,
    Identifier name,
    llvm::SmallVectorImpl<TypeDecl *> &concreteDecls);

void lookupConcreteNestedType(
    NominalTypeDecl *decl,
    Identifier name,
    llvm::SmallVectorImpl<TypeDecl *> &concreteDecls);

TypeDecl *findBestConcreteNestedType(
    llvm::SmallVectorImpl<TypeDecl *> &concreteDecls);

} // end namespace rewriting

} // end namespace nextcode

#endif