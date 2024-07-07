//===--- ImplementTypeIDZone.h - Implement a TypeID Zone --------*- C++ -*-===//
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
//  This file should be #included to implement the TypeIDs for a given zone
//  in a C++ file.
//  Two macros should be #define'd before inclusion, and will be #undef'd at
//  the end of this file:
//
//    NEXTCODE_TYPEID_ZONE: The ID number of the Zone being defined, which must
//    be unique. 0 is reserved for basic C and LLVM types; 255 is reserved
//    for test cases.
//
//    NEXTCODE_TYPEID_HEADER: A (quoted) name of the header to be
//    included to define the types in the zone.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_TYPEID_ZONE
#  error Must define the value of the TypeID zone with the given name.
#endif

#ifndef NEXTCODE_TYPEID_HEADER
#  error Must define the TypeID header name with NEXTCODE_TYPEID_HEADER
#endif

// Define a TypeID where the type name and internal name are the same.
#define NEXTCODE_REQUEST(Zone, Type, Sig, Caching, LocOptions) NEXTCODE_TYPEID_NAMED(Type, Type)
#define NEXTCODE_TYPEID(Type) NEXTCODE_TYPEID_NAMED(Type, Type)

// Out-of-line definitions.
#define NEXTCODE_TYPEID_NAMED(Type, Name)            \
  const uint64_t TypeID<Type>::value;

#define NEXTCODE_TYPEID_TEMPLATE1_NAMED(Template, Name, Param1, Arg1)

#include NEXTCODE_TYPEID_HEADER

#undef NEXTCODE_REQUEST

#undef NEXTCODE_TYPEID_NAMED
#undef NEXTCODE_TYPEID_TEMPLATE1_NAMED

#undef NEXTCODE_TYPEID
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
