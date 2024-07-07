//===--- BasicBridging.h - header for the nextcode BasicBridging module ------===//
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

#ifndef NEXTCODE_BASIC_BASICBRIDGING_H
#define NEXTCODE_BASIC_BASICBRIDGING_H

#if !defined(COMPILED_WITH_NEXTCODE) || !defined(PURE_BRIDGING_MODE)
#define USED_IN_CPP_SOURCE
#endif

// Do not add other C++/llvm/nextcode header files here!
// Function implementations should be placed into BasicBridging.cpp and required header files should be added there.
//
// Pure bridging mode does not permit including any C++/llvm/nextcode headers.
// See also the comments for `BRIDGING_MODE` in the top-level CMakeLists.txt file.
//
#include "nextcode/Basic/BridgedNeXTCodeObject.h"
#include "nextcode/Basic/Compiler.h"

#include <stddef.h>
#include <stdint.h>
#ifdef USED_IN_CPP_SOURCE
// Workaround to avoid a compiler error because `cas::ObjectRef` is not defined
// when including VirtualFileSystem.h
#include <cassert>
#include "llvm/CAS/CASReference.h"

#include "nextcode/Basic/SourceLoc.h"
#include "llvm/ADT/StringRef.h"
#include <string>
#include <vector>
#endif

// FIXME: We ought to be importing '<nextcode/bridging>' instead.
#if __has_attribute(nextcode_name)
#define NEXTCODE_NAME(NAME) __attribute__((nextcode_name(NAME)))
#else
#define NEXTCODE_NAME(NAME)
#endif

#if __has_attribute(availability)
#define NEXTCODE_UNAVAILABLE(msg) \
  __attribute__((availability(nextcode, unavailable, message=msg)))
#else
#define NEXTCODE_UNAVAILABLE(msg)
#endif

#ifdef PURE_BRIDGING_MODE
// In PURE_BRIDGING_MODE, briding functions are not inlined
#define BRIDGED_INLINE
#else
#define BRIDGED_INLINE inline
#endif

namespace llvm {
class raw_ostream;
} // end namespace llvm

NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS

typedef intptr_t NeXTCodeInt;
typedef uintptr_t NeXTCodeUInt;

// Define a bridging wrapper that wraps an underlying C++ pointer type. When
// importing into NeXTCode, we expose an initializer and accessor that works with
// `void *`, which is imported as UnsafeMutableRawPointer. Note we can't rely on
// NeXTCode importing the underlying C++ pointer as an OpaquePointer since that is
// liable to change with PURE_BRIDGING_MODE, since that changes what we include,
// and NeXTCode could import the underlying pointee type instead. We need to be
// careful that the interface we expose remains consistent regardless of
// PURE_BRIDGING_MODE.
#define BRIDGING_WRAPPER_IMPL(Node, Name, Nullability)                         \
  class Bridged##Name {                                                        \
    Node * Nullability Ptr;                                                    \
                                                                               \
  public:                                                                      \
    NEXTCODE_UNAVAILABLE("Use init(raw:) instead")                                \
    Bridged##Name(Node * Nullability ptr) : Ptr(ptr) {}                        \
                                                                               \
    NEXTCODE_UNAVAILABLE("Use '.raw' instead")                                    \
    Node * Nullability unbridged() const { return Ptr; }                       \
  };                                                                           \
                                                                               \
  NEXTCODE_NAME("getter:Bridged" #Name ".raw(self:)")                             \
  inline void * Nullability Bridged##Name##_getRaw(Bridged##Name bridged) {    \
    return bridged.unbridged();                                                \
  }                                                                            \
                                                                               \
  NEXTCODE_NAME("Bridged" #Name ".init(raw:)")                                    \
  inline Bridged##Name Bridged##Name##_fromRaw(void * Nullability ptr) {       \
    return static_cast<Node *>(ptr);                                           \
  }

// Bridging wrapper macros for convenience.
#define BRIDGING_WRAPPER_NONNULL(Node, Name) \
  BRIDGING_WRAPPER_IMPL(Node, Name, _Nonnull)

#define BRIDGING_WRAPPER_NULLABLE(Node, Name) \
  BRIDGING_WRAPPER_IMPL(Node, Nullable##Name, _Nullable)

void assertFail(const char * _Nonnull msg, const char * _Nonnull file,
                NeXTCodeUInt line, const char * _Nonnull function);

//===----------------------------------------------------------------------===//
// MARK: ArrayRef
//===----------------------------------------------------------------------===//

class BridgedArrayRef {
public:
  NEXTCODE_UNAVAILABLE("Use '.data' instead")
  const void *_Nullable Data;

  NEXTCODE_UNAVAILABLE("Use '.count' instead")
  size_t Length;

  BridgedArrayRef() : Data(nullptr), Length(0) {}

  NEXTCODE_NAME("init(data:count:)")
  BridgedArrayRef(const void *_Nullable data, size_t length)
      : Data(data), Length(length) {}

#ifdef USED_IN_CPP_SOURCE
  template <typename T>
  BridgedArrayRef(llvm::ArrayRef<T> arr)
      : Data(arr.data()), Length(arr.size()) {}

  template <typename T>
  llvm::ArrayRef<T> unbridged() const {
    return {static_cast<const T *>(Data), Length};
  }
#endif
};

NEXTCODE_NAME("getter:BridgedArrayRef.data(self:)")
BRIDGED_INLINE
const void *_Nullable BridgedArrayRef_data(BridgedArrayRef arr);

NEXTCODE_NAME("getter:BridgedArrayRef.count(self:)")
BRIDGED_INLINE NeXTCodeInt BridgedArrayRef_count(BridgedArrayRef arr);

//===----------------------------------------------------------------------===//
// MARK: Data
//===----------------------------------------------------------------------===//

class BridgedData {
public:
  NEXTCODE_UNAVAILABLE("Use '.baseAddress' instead")
  const char *_Nullable BaseAddress;

  NEXTCODE_UNAVAILABLE("Use '.count' instead")
  size_t Length;

  BridgedData() : BaseAddress(nullptr), Length(0) {}

  NEXTCODE_NAME("init(baseAddress:count:)")
  BridgedData(const char *_Nullable baseAddress, size_t length)
      : BaseAddress(baseAddress), Length(length) {}
};

NEXTCODE_NAME("getter:BridgedData.baseAddress(self:)")
BRIDGED_INLINE
const char *_Nullable BridgedData_baseAddress(BridgedData data);

NEXTCODE_NAME("getter:BridgedData.count(self:)")
BRIDGED_INLINE NeXTCodeInt BridgedData_count(BridgedData data);

NEXTCODE_NAME("BridgedData.free(self:)")
void BridgedData_free(BridgedData data);

//===----------------------------------------------------------------------===//
// MARK: Feature
//===----------------------------------------------------------------------===//

enum ENUM_EXTENSIBILITY_ATTR(open) BridgedFeature {
#define LANGUAGE_FEATURE(FeatureName, SENumber, Description) FeatureName,
#include "nextcode/Basic/Features.def"
};

//===----------------------------------------------------------------------===//
// MARK: OStream
//===----------------------------------------------------------------------===//

BRIDGING_WRAPPER_NONNULL(llvm::raw_ostream, OStream)

//===----------------------------------------------------------------------===//
// MARK: StringRef
//===----------------------------------------------------------------------===//

class BridgedStringRef {
  const char *_Nullable Data;
  size_t Length;

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedStringRef(llvm::StringRef sref)
      : Data(sref.data()), Length(sref.size()) {}

  llvm::StringRef unbridged() const { return llvm::StringRef(Data, Length); }
#endif

  BridgedStringRef() : Data(nullptr), Length(0) {}

  NEXTCODE_NAME("init(data:count:)")
  BridgedStringRef(const char *_Nullable data, size_t length)
      : Data(data), Length(length) {}

  void write(BridgedOStream os) const;
};

NEXTCODE_NAME("getter:BridgedStringRef.data(self:)")
BRIDGED_INLINE 
const uint8_t *_Nullable BridgedStringRef_data(BridgedStringRef str);

NEXTCODE_NAME("getter:BridgedStringRef.count(self:)")
BRIDGED_INLINE NeXTCodeInt BridgedStringRef_count(BridgedStringRef str);

NEXTCODE_NAME("getter:BridgedStringRef.isEmpty(self:)")
BRIDGED_INLINE bool BridgedStringRef_empty(BridgedStringRef str);

class BridgedOwnedString {
  char *_Nonnull Data;
  size_t Length;

public:
#ifdef USED_IN_CPP_SOURCE
  BridgedOwnedString(const std::string &stringToCopy);

  llvm::StringRef unbridgedRef() const { return llvm::StringRef(Data, Length); }
#endif

  void destroy() const;
};

NEXTCODE_NAME("getter:BridgedOwnedString.data(self:)")
BRIDGED_INLINE 
const uint8_t *_Nullable BridgedOwnedString_data(BridgedOwnedString str);

NEXTCODE_NAME("getter:BridgedOwnedString.count(self:)")
BRIDGED_INLINE NeXTCodeInt BridgedOwnedString_count(BridgedOwnedString str);

NEXTCODE_NAME("getter:BridgedOwnedString.isEmpty(self:)")
BRIDGED_INLINE bool BridgedOwnedString_empty(BridgedOwnedString str);

//===----------------------------------------------------------------------===//
// MARK: SourceLoc
//===----------------------------------------------------------------------===//

class BridgedSourceLoc {
  const void *_Nullable Raw;

public:
  BridgedSourceLoc() : Raw(nullptr) {}

  NEXTCODE_NAME("init(raw:)")
  BridgedSourceLoc(const void *_Nullable raw) : Raw(raw) {}

#ifdef USED_IN_CPP_SOURCE
  BridgedSourceLoc(nextcode::SourceLoc loc) : Raw(loc.getOpaquePointerValue()) {}

  nextcode::SourceLoc unbridged() const {
    return nextcode::SourceLoc(
        llvm::SMLoc::getFromPointer(static_cast<const char *>(Raw)));
  }
#endif

  NEXTCODE_IMPORT_UNSAFE
  const void *_Nullable getOpaquePointerValue() const { return Raw; }

  NEXTCODE_NAME("advanced(by:)")
  BRIDGED_INLINE
  BridgedSourceLoc advancedBy(size_t n) const;
};

NEXTCODE_NAME("getter:BridgedSourceLoc.isValid(self:)")
BRIDGED_INLINE bool BridgedSourceLoc_isValid(BridgedSourceLoc loc);

//===----------------------------------------------------------------------===//
// MARK: SourceRange
//===----------------------------------------------------------------------===//

class BridgedSourceRange {
public:
  NEXTCODE_NAME("start")
  BridgedSourceLoc Start;

  NEXTCODE_NAME("end")
  BridgedSourceLoc End;

  BridgedSourceRange() : Start(), End() {}

  NEXTCODE_NAME("init(start:end:)")
  BridgedSourceRange(BridgedSourceLoc start, BridgedSourceLoc end)
      : Start(start), End(end) {}

#ifdef USED_IN_CPP_SOURCE
  BridgedSourceRange(nextcode::SourceRange range)
      : Start(range.Start), End(range.End) {}

  nextcode::SourceRange unbridged() const {
    return nextcode::SourceRange(Start.unbridged(), End.unbridged());
  }
#endif
};

//===----------------------------------------------------------------------===//
// MARK: BridgedCharSourceRange
//===----------------------------------------------------------------------===//

class BridgedCharSourceRange {
public:
  NEXTCODE_UNAVAILABLE("Use '.start' instead")
  BridgedSourceLoc Start;

  NEXTCODE_UNAVAILABLE("Use '.byteLength' instead")
  unsigned ByteLength;

  NEXTCODE_NAME("init(start:byteLength:)")
  BridgedCharSourceRange(BridgedSourceLoc start, unsigned byteLength)
      : Start(start), ByteLength(byteLength) {}

#ifdef USED_IN_CPP_SOURCE
  BridgedCharSourceRange(nextcode::CharSourceRange range)
      : Start(range.getStart()), ByteLength(range.getByteLength()) {}

  nextcode::CharSourceRange unbridged() const {
    return nextcode::CharSourceRange(Start.unbridged(), ByteLength);
  }
#endif
};

NEXTCODE_NAME("getter:BridgedCharSourceRange.start(self:)")
inline BridgedSourceLoc
BridgedCharSourceRange_start(BridgedCharSourceRange range) {
  return range.Start;
}

NEXTCODE_NAME("getter:BridgedCharSourceRange.byteLength(self:)")
inline NeXTCodeInt
BridgedCharSourceRange_byteLength(BridgedCharSourceRange range) {
  return static_cast<NeXTCodeInt>(range.ByteLength);
}

//===----------------------------------------------------------------------===//
// MARK: std::vector<BridgedCharSourceRange>
//===----------------------------------------------------------------------===//

/// An opaque, heap-allocated `std::vector<CharSourceRange>`.
///
/// This type is manually memory managed. The creator of the object needs to
/// ensure that `takeUnbridged` is called to free the memory.
class BridgedCharSourceRangeVector {
  /// Opaque pointer to `std::vector<CharSourceRange>`.
  void *_Nonnull vector;

public:
  BridgedCharSourceRangeVector();

  NEXTCODE_NAME("append(_:)")
  void push_back(BridgedCharSourceRange range);

#ifdef USED_IN_CPP_SOURCE
  /// Returns the `std::vector<nextcode::CharSourceRange>` that this
  /// `BridgedCharSourceRangeVector` represents and frees the memory owned by
  /// this `BridgedCharSourceRangeVector`.
  ///
  /// No operations should be called on `BridgedCharSourceRangeVector` after
  /// `takeUnbridged` is called.
  std::vector<nextcode::CharSourceRange> takeUnbridged() {
    auto *vectorPtr =
        static_cast<std::vector<nextcode::CharSourceRange> *>(vector);
    std::vector<nextcode::CharSourceRange> unbridged = *vectorPtr;
    delete vectorPtr;
    return unbridged;
  }
#endif
};

//===----------------------------------------------------------------------===//
// MARK: Plugins
//===----------------------------------------------------------------------===//

NEXTCODE_BEGIN_ASSUME_NONNULL

/// Create a new root 'null' JSON value. Clients must call \c JSON_value_delete
/// after using it.
void *JSON_newValue();

/// Parse \p data as a JSON data and return the top-level value. Clients must
/// call \c JSON_value_delete after using it.
void *JSON_deserializedValue(BridgedData data);

/// Serialize a value and populate \p result with the result data. Clients
/// must call \c BridgedData_free after using the \p result.
void JSON_value_serialize(void *valuePtr, BridgedData *result);

/// Destroy and release the memory for \p valuePtr that is a result from
/// \c JSON_newValue() or \c JSON_deserializedValue() .
void JSON_value_delete(void *valuePtr);

bool JSON_value_getAsNull(void *valuePtr);
bool JSON_value_getAsBoolean(void *valuePtr, bool *result);
bool JSON_value_getAsString(void *valuePtr, BridgedData *result);
bool JSON_value_getAsDouble(void *valuePtr, double *result);
bool JSON_value_getAsInteger(void *valuePtr, int64_t *result);
bool JSON_value_getAsObject(void *valuePtr, void *_Nullable *_Nonnull result);
bool JSON_value_getAsArray(void *valuePtr, void *_Nullable *_Nonnull result);

size_t JSON_object_getSize(void *objectPtr);
BridgedData JSON_object_getKey(void *objectPtr, size_t i);
bool JSON_object_hasKey(void *objectPtr, const char *key);
void *JSON_object_getValue(void *objectPtr, const char *key);

size_t JSON_array_getSize(void *arrayPtr);
void *JSON_array_getValue(void *arrayPtr, size_t index);

void JSON_value_emplaceNull(void *valuePtr);
void JSON_value_emplaceBoolean(void *valuePtr, bool value);
void JSON_value_emplaceString(void *valuePtr, const char *value);
void JSON_value_emplaceDouble(void *valuePtr, double value);
void JSON_value_emplaceInteger(void *valuePtr, int64_t value);
void *JSON_value_emplaceNewObject(void *valuePtr);
void *JSON_value_emplaceNewArray(void *valuePtr);

void JSON_object_setNull(void *objectPtr, const char *key);
void JSON_object_setBoolean(void *objectPtr, const char *key, bool value);
void JSON_object_setString(void *objectPtr, const char *key, const char *value);
void JSON_object_setDouble(void *objectPtr, const char *key, double value);
void JSON_object_setInteger(void *objectPtr, const char *key, int64_t value);
void *JSON_object_setNewObject(void *objectPtr, const char *key);
void *JSON_object_setNewArray(void *objectPtr, const char *key);
void *JSON_object_setNewValue(void *objectPtr, const char *key);

void JSON_array_pushNull(void *arrayPtr);
void JSON_array_pushBoolean(void *arrayPtr, bool value);
void JSON_array_pushString(void *arrayPtr, const char *value);
void JSON_array_pushDouble(void *arrayPtr, double value);
void JSON_array_pushInteger(void *arrayPtr, int64_t value);
void *JSON_array_pushNewObject(void *arrayPtr);
void *JSON_array_pushNewArray(void *arrayPtr);
void *JSON_array_pushNewValue(void *arrayPtr);

NEXTCODE_END_ASSUME_NONNULL

NEXTCODE_END_NULLABILITY_ANNOTATIONS

#ifndef PURE_BRIDGING_MODE
// In _not_ PURE_BRIDGING_MODE, bridging functions are inlined and therefore
// included in the header file. This is because they rely on C++ headers that
// we don't want to pull in when using "pure bridging mode".
#include "BasicBridgingImpl.h"
#endif

#endif
