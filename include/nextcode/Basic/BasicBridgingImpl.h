//===--- BasicBridgingImpl.h - header for the nextcode BasicBridging module --===//
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

#ifndef NEXTCODE_BASIC_BASICBRIDGINGIMPL_H
#define NEXTCODE_BASIC_BASICBRIDGINGIMPL_H

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

//===----------------------------------------------------------------------===//
// MARK: BridgedArrayRef
//===----------------------------------------------------------------------===//

const void *_Nullable BridgedArrayRef_data(BridgedArrayRef arr) {
  return arr.Data;
}

NeXTCodeInt BridgedArrayRef_count(BridgedArrayRef arr) {
  return static_cast<NeXTCodeInt>(arr.Length);
}

//===----------------------------------------------------------------------===//
// MARK: BridgedData
//===----------------------------------------------------------------------===//

const char *_Nullable BridgedData_baseAddress(BridgedData data) {
  return data.BaseAddress;
}

NeXTCodeInt BridgedData_count(BridgedData data) {
  return static_cast<NeXTCodeInt>(data.Length);
}

//===----------------------------------------------------------------------===//
// MARK: BridgedStringRef
//===----------------------------------------------------------------------===//

const uint8_t *_Nullable BridgedStringRef_data(BridgedStringRef str) {
  return (const uint8_t *)str.unbridged().data();
}

NeXTCodeInt BridgedStringRef_count(BridgedStringRef str) {
  return (NeXTCodeInt)str.unbridged().size();
}

bool BridgedStringRef_empty(BridgedStringRef str) {
  return str.unbridged().empty();
}

//===----------------------------------------------------------------------===//
// MARK: BridgedOwnedString
//===----------------------------------------------------------------------===//

const uint8_t *_Nullable BridgedOwnedString_data(BridgedOwnedString str) {
  auto *data = str.unbridgedRef().data();
  return (const uint8_t *)(data ? data : "");
}

NeXTCodeInt BridgedOwnedString_count(BridgedOwnedString str) {
  return (NeXTCodeInt)str.unbridgedRef().size();
}

bool BridgedOwnedString_empty(BridgedOwnedString str) {
  return str.unbridgedRef().empty();
}

//===----------------------------------------------------------------------===//
// MARK: BridgedSourceLoc
//===----------------------------------------------------------------------===//

bool BridgedSourceLoc_isValid(BridgedSourceLoc loc) {
  return loc.getOpaquePointerValue() != nullptr;
}

BridgedSourceLoc BridgedSourceLoc::advancedBy(size_t n) const {
  return BridgedSourceLoc(unbridged().getAdvancedLoc(n));
}

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#endif // NEXTCODE_BASIC_BASICBRIDGINGIMPL_H
