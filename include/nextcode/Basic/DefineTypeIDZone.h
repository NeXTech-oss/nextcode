//===--- DefineTypeIDZone.h - Define a TypeID Zone --------------*- C++ -*-===//
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
//  This file should be #included to define the TypeIDs for a given zone.
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
#define NEXTCODE_TYPEID(Type) NEXTCODE_TYPEID_NAMED(Type, Type)
#define NEXTCODE_REQUEST(Zone, Type, Sig, Caching, LocOptions) NEXTCODE_TYPEID_NAMED(Type, Type)

// First pass: put all of the names into an enum so we get values for them.
template<> struct TypeIDZoneTypes<Zone::NEXTCODE_TYPEID_ZONE> {
  enum Types : uint8_t {
#define NEXTCODE_TYPEID_NAMED(Type, Name) Name,
#define NEXTCODE_TYPEID_TEMPLATE1_NAMED(Template, Name, Param1, Arg1) Name,
#define NEXTCODE_TYPEID_TEMPLATE2_NAMED(Template, Name, Param1, Arg1, Param2, Arg2) Name,
#include NEXTCODE_TYPEID_HEADER
#undef NEXTCODE_TYPEID_NAMED
#undef NEXTCODE_TYPEID_TEMPLATE1_NAMED
#undef NEXTCODE_TYPEID_TEMPLATE2_NAMED
    Count
  };
};

// Second pass: create specializations of TypeID for these types.
#define NEXTCODE_TYPEID_NAMED(Type, Name)                       \
template<> struct TypeID<Type> {                             \
  static constexpr Zone zone = Zone::NEXTCODE_TYPEID_ZONE;      \
  static const uint8_t zoneID = static_cast<uint8_t>(zone);  \
  static const uint8_t localID =                             \
    TypeIDZoneTypes<Zone::NEXTCODE_TYPEID_ZONE>::Name;          \
                                                             \
  static const uint64_t value = formTypeID(zoneID, localID); \
                                                             \
  static llvm::StringRef getName() { return #Name; }         \
};

#define NEXTCODE_TYPEID_TEMPLATE1_NAMED(Template, Name, Param1, Arg1)    \
template<Param1> struct TypeID<Template<Arg1>> {                      \
private:                                                              \
  static const uint64_t templateID =                                  \
    formTypeID(static_cast<uint8_t>(Zone::NEXTCODE_TYPEID_ZONE),         \
               TypeIDZoneTypes<Zone::NEXTCODE_TYPEID_ZONE>::Name);       \
                                                                      \
public:                                                               \
  static const uint64_t value =                                       \
    (TypeID<Arg1>::value << 16) | templateID;                         \
                                                                      \
  static std::string getName() {                                      \
    return std::string(#Name) + "<" + TypeID<Arg1>::getName() + ">";  \
  }                                                                   \
};                                                                    \
                                                                      \
template<Param1> const uint64_t TypeID<Template<Arg1>>::value;

#define NEXTCODE_TYPEID_TEMPLATE2_NAMED(Template, Name, Param1, Arg1, Param2, Arg2) \
template<Param1, Param2> struct TypeID<Template<Arg1, Arg2>> {        \
private:                                                              \
  static const uint64_t templateID =                                  \
    formTypeID(static_cast<uint8_t>(Zone::NEXTCODE_TYPEID_ZONE),         \
               TypeIDZoneTypes<Zone::NEXTCODE_TYPEID_ZONE>::Name);       \
                                                                      \
public:                                                               \
  static const uint64_t value =                                       \
    (TypeID<Arg1>::value << 32) |                                     \
    (TypeID<Arg2>::value << 16) |                                     \
    templateID;                                                       \
                                                                      \
  static std::string getName() {                                      \
    return std::string(#Name) + "<" + TypeID<Arg1>::getName() +       \
        ", " + TypeID<Arg2>::getName() + ">";                         \
  }                                                                   \
};                                                                    \
                                                                      \
template<Param1, Param2> const uint64_t TypeID<Template<Arg1, Arg2>>::value;

#include NEXTCODE_TYPEID_HEADER

#undef NEXTCODE_REQUEST

#undef NEXTCODE_TYPEID_NAMED
#undef NEXTCODE_TYPEID_TEMPLATE1_NAMED
#undef NEXTCODE_TYPEID_TEMPLATE2_NAMED

#undef NEXTCODE_TYPEID
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
