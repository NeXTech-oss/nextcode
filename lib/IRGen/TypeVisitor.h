//===--- TypeVisitor.h - IR-gen TypeVisitor specialization ------*- C++ -*-===//
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
// This file defines various type visitors that are useful in
// IR-generation.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_TYPEVISITOR_H
#define NEXTCODE_IRGEN_TYPEVISITOR_H

#include "nextcode/AST/CanTypeVisitor.h"

namespace nextcode {
namespace irgen {

/// ReferenceTypeVisitor - This is a specialization of CanTypeVisitor
/// which automatically ignores non-reference types.
template <typename ImplClass, typename RetTy = void, typename... Args>
class ReferenceTypeVisitor : public CanTypeVisitor<ImplClass, RetTy, Args...> {
#define TYPE(Id) \
  RetTy visit##Id##Type(Can##Id##Type T, Args... args) { \
    llvm_unreachable(#Id "Type is not a reference type"); \
  }
  TYPE(BoundGenericEnum)
  TYPE(BoundGenericStruct)
  TYPE(BuiltinFloat)
  TYPE(BuiltinInteger)
  TYPE(BuiltinRawPointer)
  TYPE(BuiltinVector)
  TYPE(LValue)
  TYPE(Metatype)
  TYPE(Module)
  TYPE(Enum)
  TYPE(ReferenceStorage)
  TYPE(Struct)
  TYPE(Tuple)
#undef TYPE

  // BuiltinNativeObject
  // BuiltinBridgeObject
  // Class
  // BoundGenericClass
  // Protocol
  // ProtocolComposition
  // Archetype
  // Function
};
  
} // end namespace irgen
} // end namespace nextcode
  
#endif
