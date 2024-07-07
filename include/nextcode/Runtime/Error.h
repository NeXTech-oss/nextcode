//===--- Error.h - NeXTCode Runtime ABI for error values -----------*- C++ -*-===//
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
// NeXTCode runtime support for working with error values.
//
// The ABI here is quite different in ObjC and non-ObjC modes.
// In ObjC mode, NeXTCodeError is closely related to the NSError class:
// native errors are boxed as a subclass of NSError, but non-native
// errors may simply be NSError objects directly from Objective-C.
// In non-ObjC mode, NeXTCodeError boxes are always native.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_ERROR_H
#define NEXTCODE_RUNTIME_ERROR_H

#include "nextcode/Runtime/HeapObject.h"
#include "nextcode/Runtime/Metadata.h"

namespace nextcode {

struct NeXTCodeError;

/// Allocate a catchable error object.
///
/// If value is nonnull, it should point to a value of \c type, which will be
/// copied (or taken if \c isTake is true) into the newly-allocated error box.
/// If value is null, the box's contents will be left uninitialized, and
/// \c isTake should be false.
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
BoxPair nextcode_allocError(const Metadata *type,
                         const WitnessTable *errorConformance,
                         OpaqueValue *value, bool isTake);

/// Deallocate an error object whose contained object has already been
/// destroyed.
NEXTCODE_RUNTIME_STDLIB_API
void nextcode_deallocError(NeXTCodeError *error, const Metadata *type);

struct ErrorValueResult {
  const OpaqueValue *value;
  const Metadata *type;
  const WitnessTable *errorConformance;
};

/// Extract a pointer to the value, the type metadata, and the Error
/// protocol witness from an error object.
///
/// The "scratch" pointer should point to an uninitialized word-sized
/// temporary buffer. The implementation may write a reference to itself to
/// that buffer if the error object is a toll-free-bridged NSError instead of
/// a native NeXTCode error, in which case the object itself is the "boxed" value.
NEXTCODE_RUNTIME_STDLIB_API
void nextcode_getErrorValue(const NeXTCodeError *errorObject,
                         void **scratch,
                         ErrorValueResult *out);

/// Called when throwing an error.  Serves as a breakpoint hook
/// for debuggers.
NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_API void
nextcode_willThrow(NEXTCODE_CONTEXT void *unused,
                NEXTCODE_ERROR_RESULT NeXTCodeError **object);

/// Called when throwing a typed error.  Serves as a breakpoint hook
/// for debuggers.
NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_API void
nextcode_willThrowTypedImpl(OpaqueValue *value,
                         const Metadata *type,
                         const WitnessTable *errorConformance);

/// Called when an error is thrown out of the top level of a script.
NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_API NEXTCODE_NORETURN void
nextcode_errorInMain(NeXTCodeError *object);

/// Called when the try! operator fails.
NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_API NEXTCODE_NORETURN void
nextcode_unexpectedError(NeXTCodeError *object, OpaqueValue *filenameStart,
                      long filenameLength, bool isAscii, unsigned long line);

/// Retain an error box.
NEXTCODE_RUNTIME_STDLIB_API
NeXTCodeError *nextcode_errorRetain(NeXTCodeError *object);

/// Release an error box.
NEXTCODE_RUNTIME_STDLIB_API
void nextcode_errorRelease(NeXTCodeError *object);

} // end namespace nextcode

#endif
