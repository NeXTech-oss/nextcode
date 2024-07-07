//===--- GenStruct.h - NeXTCode IR generation for structs ----------*- C++ -*-===//
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
//  This file provides the private interface to the struct-emission code.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_GENSTRUCT_H
#define NEXTCODE_IRGEN_GENSTRUCT_H

#include <optional>

namespace llvm {
  class Constant;
}

namespace nextcode {
  class CanType;
  class SILType;
  class VarDecl;

namespace irgen {
  class Address;
  class Explosion;
  class IRGenFunction;
  class IRGenModule;
  class MemberAccessStrategy;
  class TypeInfo;

  Address projectPhysicalStructMemberAddress(IRGenFunction &IGF,
                                             Address base,
                                             SILType baseType,
                                             VarDecl *field);
  void projectPhysicalStructMemberFromExplosion(IRGenFunction &IGF,
                                                SILType baseType,
                                                Explosion &base,
                                                VarDecl *field,
                                                Explosion &out);

  /// Return the constant offset of the given stored property in a struct,
  /// or return None if the field does not have fixed layout.
  llvm::Constant *emitPhysicalStructMemberFixedOffset(IRGenModule &IGM,
                                                      SILType baseType,
                                                      VarDecl *field);

  /// Return a strategy for accessing the given stored struct property.
  ///
  /// This API is used by RemoteAST.
  MemberAccessStrategy
  getPhysicalStructMemberAccessStrategy(IRGenModule &IGM,
                                        SILType baseType, VarDecl *field);

  const TypeInfo *getPhysicalStructFieldTypeInfo(IRGenModule &IGM,
                                                 SILType baseType,
                                                 VarDecl *field);

  /// Returns the index of the element in the llvm struct type which represents
  /// \p field in \p baseType.
  ///
  /// Returns None if \p field has an empty type and therefore has no
  /// corresponding element in the llvm type.
  std::optional<unsigned> getPhysicalStructFieldIndex(IRGenModule &IGM,
                                                      SILType baseType,
                                                      VarDecl *field);
} // end namespace irgen
} // end namespace nextcode

#endif
