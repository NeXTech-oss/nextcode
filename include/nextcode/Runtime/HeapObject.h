//===--- HeapObject.h - NeXTCode Language Allocation ABI -----------*- C++ -*-===//
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
// NeXTCode Allocation ABI
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_ALLOC_H
#define NEXTCODE_RUNTIME_ALLOC_H

#include <cstddef>
#include <cstdint>
#include "nextcode/Runtime/Config.h"
#include "nextcode/Runtime/Heap.h"

#if NEXTCODE_OBJC_INTEROP
#include <objc/objc.h>
#endif // NEXTCODE_OBJC_INTEROP

// Bring in the definition of HeapObject 
#include "nextcode/shims/HeapObject.h"
#include "nextcode/shims/Visibility.h"

namespace nextcode {
  
struct InProcess;

template <typename Runtime> struct TargetMetadata;
using Metadata = TargetMetadata<InProcess>;
  
template <typename Runtime> struct TargetHeapMetadata;
using HeapMetadata = TargetHeapMetadata<InProcess>;

struct OpaqueValue;

/// Allocates a new heap object.  The returned memory is
/// uninitialized outside of the heap-object header.  The object
/// has an initial retain count of 1, and its metadata is set to
/// the given value.
///
/// At some point "soon after return", it will become an
/// invariant that metadata->getSize(returnValue) will equal
/// requiredSize.
///
/// Either aborts or throws a nextcode exception if the allocation fails.
///
/// \param requiredSize - the required size of the allocation,
///   including the header
/// \param requiredAlignmentMask - the required alignment of the allocation;
///   always one less than a power of 2 that's at least alignof(void*)
/// \return never null
///
/// POSSIBILITIES: The argument order is fair game.  It may be useful
/// to have a variant which guarantees zero-initialized memory.
NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
HeapObject *nextcode_allocObject(HeapMetadata const *metadata,
                              size_t requiredSize,
                              size_t requiredAlignmentMask);

/// Initializes the object header of a stack allocated object.
///
/// \param metadata - the object's metadata which is stored in the header
/// \param object - the pointer to the object's memory on the stack
/// \returns the passed object pointer.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_initStackObject(HeapMetadata const *metadata,
                                  HeapObject *object);

/// Initializes the object header of a static object which is statically
/// allocated in the data section.
///
/// \param metadata - the object's metadata which is stored in the header
/// \param object - the address of the object in the data section. It is assumed
///        that at offset -1 there is a nextcode_once token allocated.
/// \returns the passed object pointer.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_initStaticObject(HeapMetadata const *metadata,
                                   HeapObject *object);

/// Performs verification that the lifetime of a stack allocated object has
/// ended. It aborts if the reference counts of the object indicate that the
/// object did escape to some other location.
NEXTCODE_RUNTIME_EXPORT
void nextcode_verifyEndOfLifetime(HeapObject *object);

struct BoxPair {
  HeapObject *object;
  OpaqueValue *buffer;
};

/// Allocates a heap object that can contain a value of the given type.
/// Returns a Box structure containing a HeapObject* pointer to the
/// allocated object, and a pointer to the value inside the heap object.
/// The value pointer points to an uninitialized buffer of size and alignment
/// appropriate to store a value of the given type.
/// The heap object has an initial retain count of 1, and its metadata is set
/// such that destroying the heap object destroys the contained value.
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_EXPORT
BoxPair nextcode_allocBox(Metadata const *type);

/// Performs a uniqueness check on the pointer to a box structure. If the check
/// fails allocates a new box and stores the pointer in the buffer.
///
///  if (!isUnique(buffer[0]))
///    buffer[0] = nextcode_allocBox(type)
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_EXPORT
BoxPair nextcode_makeBoxUnique(OpaqueValue *buffer, Metadata const *type,
                                    size_t alignMask);

/// Returns the address of a heap object representing all empty box types.
NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
HeapObject* nextcode_allocEmptyBox();

/// Atomically increments the retain count of an object.
///
/// \param object - may be null, in which case this is a no-op
///
/// \return object - we return the object because this enables tail call
/// optimization and the argument register to be live through the call on
/// architectures whose argument and return register is the same register.
///
/// POSSIBILITIES: We may end up wanting a bunch of different variants:
///  - the general version which correctly handles null values, nextcode
///     objects, and ObjC objects
///    - a variant that assumes that its operand is a nextcode object
///      - a variant that can safely use non-atomic operations
///      - maybe a variant that can assume a non-null object
/// It may also prove worthwhile to have this use a custom CC
/// which preserves a larger set of registers.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_retain(HeapObject *object);

NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_retain_n(HeapObject *object, uint32_t n);

NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_nonatomic_retain(HeapObject *object);

NEXTCODE_RUNTIME_EXPORT
HeapObject* nextcode_nonatomic_retain_n(HeapObject *object, uint32_t n);

/// Atomically increments the reference count of an object, unless it has
/// already been destroyed. Returns nil if the object is dead.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_tryRetain(HeapObject *object);

/// Returns true if an object is in the process of being deallocated.
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isDeallocating(HeapObject *object);

/// Atomically decrements the retain count of an object.  If the
/// retain count reaches zero, the object is destroyed as follows:
///
///   size_t allocSize = object->metadata->destroy(object);
///   if (allocSize) nextcode_deallocObject(object, allocSize);
///
/// \param object - may be null, in which case this is a no-op
///
/// POSSIBILITIES: We may end up wanting a bunch of different variants:
///  - the general version which correctly handles null values, nextcode
///     objects, and ObjC objects
///    - a variant that assumes that its operand is a nextcode object
///      - a variant that can safely use non-atomic operations
///      - maybe a variant that can assume a non-null object
/// It's unlikely that a custom CC would be beneficial here.
NEXTCODE_RUNTIME_EXPORT
void nextcode_release(HeapObject *object);

NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_release(HeapObject *object);

/// Atomically decrements the retain count of an object n times. If the retain
/// count reaches zero, the object is destroyed
NEXTCODE_RUNTIME_EXPORT
void nextcode_release_n(HeapObject *object, uint32_t n);

/// Sets the RC_DEALLOCATING_FLAG flag. This is done non-atomically.
/// The strong reference count of \p object must be 1 and no other thread may
/// retain the object during executing this function.
NEXTCODE_RUNTIME_EXPORT
void nextcode_setDeallocating(HeapObject *object);

NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_release_n(HeapObject *object, uint32_t n);

// Refcounting observation hooks for memory tools. Don't use these.
NEXTCODE_RUNTIME_EXPORT
size_t nextcode_retainCount(HeapObject *object);
NEXTCODE_RUNTIME_EXPORT
size_t nextcode_unownedRetainCount(HeapObject *object);
NEXTCODE_RUNTIME_EXPORT
size_t nextcode_weakRetainCount(HeapObject *object);

/// Is this pointer a non-null unique reference to an object?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferenced(const void *);

/// Is this non-null pointer a unique reference to an object?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferenced_nonNull(const void *);

/// Is this non-null BridgeObject a unique reference to an object?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferenced_nonNull_bridgeObject(uintptr_t bits);

/// Is this pointer a non-null unique reference to an object
/// that uses NeXTCode reference counting?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferencedNonObjC(const void *);

/// Is this non-null pointer a unique reference to an object
/// that uses NeXTCode reference counting?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferencedNonObjC_nonNull(const void *);

/// Is this non-null BridgeObject a unique reference to an object
/// that uses NeXTCode reference counting?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferencedNonObjC_nonNull_bridgeObject(
  uintptr_t bits);

/// Is this native NeXTCode pointer a non-null unique reference to
/// an object?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferenced_native(const struct HeapObject *);

/// Is this non-null native NeXTCode pointer a unique reference to
/// an object?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isUniquelyReferenced_nonNull_native(const struct HeapObject *);

/// Is this native NeXTCode pointer non-null and has a reference count greater than
/// one.
/// This runtime call will print an error message with file name and location if
/// the closure is escaping but it will not abort.
///
/// \p type: 0 - withoutActuallyEscaping verification
///              Was the closure passed to a withoutActuallyEscaping block
///              escaped in the block?
///          1 - @objc closure sentinel verification
///              Was the closure passed to Objective-C escaped?
NEXTCODE_RUNTIME_EXPORT
bool nextcode_isEscapingClosureAtFileLocation(const struct HeapObject *object,
                                           const unsigned char *filename,
                                           int32_t filenameLength,
                                           int32_t line,
                                           int32_t column,
                                           unsigned type);

/// Deallocate the given memory.
///
/// It must have been returned by nextcode_allocObject and the strong reference
/// must have the RC_DEALLOCATING_FLAG flag set, but otherwise the object is
/// in an unknown state.
///
/// \param object - never null
/// \param allocatedSize - the allocated size of the object from the
///   program's perspective, i.e. the value
/// \param allocatedAlignMask - the alignment requirement that was passed
///   to allocObject
///
/// POSSIBILITIES: It may be useful to have a variant which
/// requires the object to have been fully zeroed from offsets
/// sizeof(NeXTCodeHeapObject) to allocatedSize.
NEXTCODE_RUNTIME_EXPORT
void nextcode_deallocObject(HeapObject *object, size_t allocatedSize,
                         size_t allocatedAlignMask);

/// Deallocate an uninitialized object with a strong reference count of +1.
///
/// It must have been returned by nextcode_allocObject, but otherwise the object is
/// in an unknown state.
///
/// \param object - never null
/// \param allocatedSize - the allocated size of the object from the
///   program's perspective, i.e. the value
/// \param allocatedAlignMask - the alignment requirement that was passed
///   to allocObject
///
NEXTCODE_RUNTIME_EXPORT
void nextcode_deallocUninitializedObject(HeapObject *object, size_t allocatedSize,
                                      size_t allocatedAlignMask);

/// Deallocate the given memory.
///
/// It must have been returned by nextcode_allocObject, possibly used as an
/// Objective-C class instance, and the strong reference must have the
/// RC_DEALLOCATING_FLAG flag set, but otherwise the object is in an unknown
/// state.
///
/// \param object - never null
/// \param allocatedSize - the allocated size of the object from the
///   program's perspective, i.e. the value
/// \param allocatedAlignMask - the alignment requirement that was passed
///   to allocObject
///
/// POSSIBILITIES: It may be useful to have a variant which
/// requires the object to have been fully zeroed from offsets
/// sizeof(NeXTCodeHeapObject) to allocatedSize.
NEXTCODE_RUNTIME_EXPORT
void nextcode_deallocClassInstance(HeapObject *object,
                                 size_t allocatedSize,
                                 size_t allocatedAlignMask);

/// Deallocate the given memory after destroying instance variables.
///
/// Destroys instance variables in classes more derived than the given metatype.
///
/// It must have been returned by nextcode_allocObject, possibly used as an
/// Objective-C class instance, and the strong reference must be equal to 1.
///
/// \param object - may be null
/// \param type - most derived class whose instance variables do not need to
///   be destroyed
/// \param allocatedSize - the allocated size of the object from the
///   program's perspective, i.e. the value
/// \param allocatedAlignMask - the alignment requirement that was passed
///   to allocObject
NEXTCODE_RUNTIME_EXPORT
void nextcode_deallocPartialClassInstance(HeapObject *object,
                                       const HeapMetadata *type,
                                       size_t allocatedSize,
                                       size_t allocatedAlignMask);

/// Deallocate the given memory allocated by nextcode_allocBox; it was returned
/// by nextcode_allocBox but is otherwise in an unknown state. The given Metadata
/// pointer must be the same metadata pointer that was passed to nextcode_allocBox
/// when the memory was allocated.
NEXTCODE_RUNTIME_EXPORT
void nextcode_deallocBox(HeapObject *object);

/// Project the value out of a box. `object` must have been allocated
/// using `nextcode_allocBox`, or by the compiler using a statically-emitted
/// box metadata object.
NEXTCODE_RUNTIME_EXPORT
OpaqueValue *nextcode_projectBox(HeapObject *object);

/// RAII object that wraps a NeXTCode heap object and releases it upon
/// destruction.
class NeXTCodeRAII {
  HeapObject *object;

public:
  NeXTCodeRAII(HeapObject *obj, bool AlreadyRetained) : object(obj) {
    if (!AlreadyRetained)
      nextcode_retain(obj);
  }

  ~NeXTCodeRAII() {
    if (object)
      nextcode_release(object);
  }

  NeXTCodeRAII(const NeXTCodeRAII &other) {
    nextcode_retain(*other);
    object = *other;
    ;
  }
  NeXTCodeRAII(NeXTCodeRAII &&other) : object(*other) {
    other.object = nullptr;
  }
  NeXTCodeRAII &operator=(const NeXTCodeRAII &other) {
    if (object)
      nextcode_release(object);
    nextcode_retain(*other);
    object = *other;
    return *this;
  }
  NeXTCodeRAII &operator=(NeXTCodeRAII &&other) {
    if (object)
      nextcode_release(object);
    object = *other;
    other.object = nullptr;
    return *this;
  }

  HeapObject *operator *() const { return object; }
};

/*****************************************************************************/
/**************************** UNOWNED REFERENCES *****************************/
/*****************************************************************************/

/// An unowned reference in memory.  This is ABI.
struct UnownedReference {
  HeapObject *Value;
};

/// Increment the unowned retain count.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_unownedRetain(HeapObject *value);

/// Decrement the unowned retain count.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unownedRelease(HeapObject *value);

/// Increment the unowned retain count.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_nonatomic_unownedRetain(HeapObject *value);

/// Decrement the unowned retain count.
NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_unownedRelease(HeapObject *value);

/// Increment the unowned retain count by n.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_unownedRetain_n(HeapObject *value, int n);

/// Decrement the unowned retain count by n.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unownedRelease_n(HeapObject *value, int n);

/// Increment the unowned retain count by n.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_nonatomic_unownedRetain_n(HeapObject *value, int n);

/// Decrement the unowned retain count by n.
NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_unownedRelease_n(HeapObject *value, int n);

/// Increment the strong retain count of an object, aborting if it has
/// been deallocated.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_unownedRetainStrong(HeapObject *value);

/// Increment the strong retain count of an object, aborting if it has
/// been deallocated.
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_nonatomic_unownedRetainStrong(HeapObject *value);

/// Increment the strong retain count of an object which may have been
/// deallocated, aborting if it has been deallocated, and decrement its
/// unowned reference count.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unownedRetainStrongAndRelease(HeapObject *value);

/// Increment the strong retain count of an object which may have been
/// deallocated, aborting if it has been deallocated, and decrement its
/// unowned reference count.
NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_unownedRetainStrongAndRelease(HeapObject *value);

/// Aborts if the object has been deallocated.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unownedCheck(HeapObject *value);

static inline void nextcode_unownedInit(UnownedReference *ref, HeapObject *value) {
  ref->Value = value;
  nextcode_unownedRetain(value);
}

static inline void nextcode_unownedAssign(UnownedReference *ref,
                                       HeapObject *value) {
  auto oldValue = ref->Value;
  if (value != oldValue) {
    nextcode_unownedRetain(value);
    ref->Value = value;
    nextcode_unownedRelease(oldValue);
  }
}

static inline HeapObject *nextcode_unownedLoadStrong(UnownedReference *ref) {
  auto value = ref->Value;
  nextcode_unownedRetainStrong(value);
  return value;
}

static inline void *nextcode_unownedTakeStrong(UnownedReference *ref) {
  auto value = ref->Value;
  nextcode_unownedRetainStrongAndRelease(value);
  return value;
}

static inline void nextcode_unownedDestroy(UnownedReference *ref) {
  nextcode_unownedRelease(ref->Value);
}

static inline void nextcode_unownedCopyInit(UnownedReference *dest,
                                         UnownedReference *src) {
  dest->Value = src->Value;
  nextcode_unownedRetain(dest->Value);
}

static inline void nextcode_unownedTakeInit(UnownedReference *dest,
                                         UnownedReference *src) {
  dest->Value = src->Value;
}

static inline void nextcode_unownedCopyAssign(UnownedReference *dest,
                                           UnownedReference *src) {
  auto newValue = src->Value;
  auto oldValue = dest->Value;
  if (newValue != oldValue) {
    dest->Value = newValue;
    nextcode_unownedRetain(newValue);
    nextcode_unownedRelease(oldValue);
  }
}

static inline void nextcode_unownedTakeAssign(UnownedReference *dest,
                                           UnownedReference *src) {
  auto newValue = src->Value;
  auto oldValue = dest->Value;
  dest->Value = newValue;
  nextcode_unownedRelease(oldValue);
}

static inline bool nextcode_unownedIsEqual(UnownedReference *ref,
                                        HeapObject *value) {
  bool isEqual = ref->Value == value;
  if (isEqual)
    nextcode_unownedCheck(value);
  return isEqual;
}

/*****************************************************************************/
/****************************** WEAK REFERENCES ******************************/
/*****************************************************************************/

// Defined in Runtime/WeakReference.h
class WeakReference;

/// Initialize a weak reference.
///
/// \param ref - never null
/// \param value - can be null
/// \return ref
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_weakInit(WeakReference *ref, HeapObject *value);

/// Assign a new value to a weak reference.
///
/// \param ref - never null
/// \param value - can be null
/// \return ref
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_weakAssign(WeakReference *ref, HeapObject *value);

/// Load a value from a weak reference.  If the current value is a
/// non-null object that has begun deallocation, returns null;
/// otherwise, retains the object before returning.
///
/// \param ref - never null
/// \return can be null
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_weakLoadStrong(WeakReference *ref);

/// Load a value from a weak reference as if by nextcode_weakLoadStrong,
/// but leaving the reference in an uninitialized state.
///
/// \param ref - never null
/// \return can be null
NEXTCODE_RUNTIME_EXPORT
HeapObject *nextcode_weakTakeStrong(WeakReference *ref);

/// Destroy a weak reference.
///
/// \param ref - never null, but can refer to a null object
NEXTCODE_RUNTIME_EXPORT
void nextcode_weakDestroy(WeakReference *ref);

/// Copy initialize a weak reference.
///
/// \param dest - never null, but can refer to a null object
/// \param src - never null, but can refer to a null object
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_weakCopyInit(WeakReference *dest, WeakReference *src);

/// Take initialize a weak reference.
///
/// \param dest - never null, but can refer to a null object
/// \param src - never null, but can refer to a null object
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_weakTakeInit(WeakReference *dest, WeakReference *src);

/// Copy assign a weak reference.
///
/// \param dest - never null, but can refer to a null object
/// \param src - never null, but can refer to a null object
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_weakCopyAssign(WeakReference *dest, WeakReference *src);

/// Take assign a weak reference.
///
/// \param dest - never null, but can refer to a null object
/// \param src - never null, but can refer to a null object
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_weakTakeAssign(WeakReference *dest, WeakReference *src);

/*****************************************************************************/
/************************* OTHER REFERENCE-COUNTING **************************/
/*****************************************************************************/

NEXTCODE_RUNTIME_EXPORT
void *nextcode_bridgeObjectRetain(void *value);

/// Increment the strong retain count of a bridged object by n.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_bridgeObjectRetain_n(void *value, int n);


NEXTCODE_RUNTIME_EXPORT
void *nextcode_nonatomic_bridgeObjectRetain(void *value);


/// Increment the strong retain count of a bridged object by n.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_nonatomic_bridgeObjectRetain_n(void *value, int n);


/*****************************************************************************/
/************************ UNKNOWN REFERENCE-COUNTING *************************/
/*****************************************************************************/

#if NEXTCODE_OBJC_INTEROP

/// Increment the strong retain count of an object which might not be a native
/// NeXTCode object.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_unknownObjectRetain(void *value);

/// Increment the strong retain count of an object which might not be a native
/// NeXTCode object by n.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_unknownObjectRetain_n(void *value, int n);

/// Increment the strong retain count of an object which might not be a native
/// NeXTCode object.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_nonatomic_unknownObjectRetain(void *value);

/// Increment the strong retain count of an object which might not be a native
/// NeXTCode object by n.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_nonatomic_unknownObjectRetain_n(void *value, int n);

#else

static inline void *nextcode_unknownObjectRetain(void *value) {
  return nextcode_retain(static_cast<HeapObject *>(value));
}

static inline void *nextcode_unknownObjectRetain_n(void *value, int n) {
  return nextcode_retain_n(static_cast<HeapObject *>(value), n);
}

static inline void *nextcode_nonatomic_unknownObjectRetain(void *value) {
  return nextcode_nonatomic_retain(static_cast<HeapObject *>(value));
}

static inline void *nextcode_nonatomic_unknownObjectRetain_n(void *value, int n) {
  return nextcode_nonatomic_retain_n(static_cast<HeapObject *>(value), n);
}


#endif // NEXTCODE_OBJC_INTEROP

NEXTCODE_RUNTIME_EXPORT
void nextcode_bridgeObjectRelease(void *value);

/// Decrement the strong retain count of a bridged object by n.
NEXTCODE_RUNTIME_EXPORT
void nextcode_bridgeObjectRelease_n(void *value, int n);

NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_bridgeObjectRelease(void *value);

/// Decrement the strong retain count of a bridged object by n.
NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_bridgeObjectRelease_n(void *value, int n);

#if NEXTCODE_OBJC_INTEROP

/// Decrement the strong retain count of an object which might not be a native
/// NeXTCode object.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unknownObjectRelease(void *value);

/// Decrement the strong retain count of an object which might not be a native
/// NeXTCode object by n.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unknownObjectRelease_n(void *value, int n);

/// Decrement the strong retain count of an object which might not be a native
/// NeXTCode object.
NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_unknownObjectRelease(void *value);

/// Decrement the strong retain count of an object which might not be a native
/// NeXTCode object by n.
NEXTCODE_RUNTIME_EXPORT
void nextcode_nonatomic_unknownObjectRelease_n(void *value, int n);

#else

static inline void nextcode_unknownObjectRelease(void *value) {
  nextcode_release(static_cast<HeapObject *>(value));
}

static inline void nextcode_unknownObjectRelease_n(void *value, int n) {
  nextcode_release_n(static_cast<HeapObject *>(value), n);
}

static inline void nextcode_nonatomic_unknownObjectRelease(void *value) {
  nextcode_nonatomic_release(static_cast<HeapObject *>(value));
}

static inline void nextcode_nonatomic_unknownObjectRelease_n(void *value, int n) {
  nextcode_nonatomic_release_n(static_cast<HeapObject *>(value), n);
}

#endif // NEXTCODE_OBJC_INTEROP

/*****************************************************************************/
/************************** UNKNOWN WEAK REFERENCES **************************/
/*****************************************************************************/

#if NEXTCODE_OBJC_INTEROP

/// Initialize a weak reference.
///
/// \param ref - never null
/// \param value - not necessarily a native NeXTCode object; can be null
/// \return ref
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_unknownObjectWeakInit(WeakReference *ref, void *value);

#else

static inline WeakReference *nextcode_unknownObjectWeakInit(WeakReference *ref,
                                                         void *value) {
  return nextcode_weakInit(ref, static_cast<HeapObject *>(value));
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Assign a new value to a weak reference.
///
/// \param ref - never null
/// \param value - not necessarily a native NeXTCode object; can be null
/// \return ref
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_unknownObjectWeakAssign(WeakReference *ref, void *value);

#else

static inline WeakReference *nextcode_unknownObjectWeakAssign(WeakReference *ref,
                                                           void *value) {
  return nextcode_weakAssign(ref, static_cast<HeapObject *>(value));
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Load a value from a weak reference, much like nextcode_weakLoadStrong
/// but without requiring the variable to refer to a native NeXTCode object.
///
/// \param ref - never null
/// \return can be null
NEXTCODE_RUNTIME_EXPORT
void *nextcode_unknownObjectWeakLoadStrong(WeakReference *ref);

#else

static inline void *nextcode_unknownObjectWeakLoadStrong(WeakReference *ref) {
  return static_cast<void *>(nextcode_weakLoadStrong(ref));
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Load a value from a weak reference as if by
/// nextcode_unknownObjectWeakLoadStrong, but leaving the reference in an
/// uninitialized state.
///
/// \param ref - never null
/// \return can be null
NEXTCODE_RUNTIME_EXPORT
void *nextcode_unknownObjectWeakTakeStrong(WeakReference *ref);

#else

static inline void *nextcode_unknownObjectWeakTakeStrong(WeakReference *ref) {
  return static_cast<void *>(nextcode_weakTakeStrong(ref));
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Destroy a weak reference variable that might not refer to a native
/// NeXTCode object.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unknownObjectWeakDestroy(WeakReference *object);

#else

static inline void nextcode_unknownObjectWeakDestroy(WeakReference *object) {
  nextcode_weakDestroy(object);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Copy-initialize a weak reference variable from one that might not
/// refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_unknownObjectWeakCopyInit(WeakReference *dest,
                                               WeakReference *src);

#else

static inline WeakReference *
nextcode_unknownObjectWeakCopyInit(WeakReference *dest, WeakReference *src) {
  return nextcode_weakCopyInit(dest, src);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Take-initialize a weak reference variable from one that might not
/// refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_unknownObjectWeakTakeInit(WeakReference *dest,
                                               WeakReference *src);

#else

static inline WeakReference *
nextcode_unknownObjectWeakTakeInit(WeakReference *dest, WeakReference *src) {
  return nextcode_weakTakeInit(dest, src);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Copy-assign a weak reference variable from another when either
/// or both variables might not refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_unknownObjectWeakCopyAssign(WeakReference *dest,
                                                 WeakReference *src);

#else

static inline WeakReference *
nextcode_unknownObjectWeakCopyAssign(WeakReference *dest, WeakReference *src) {
  return nextcode_weakCopyAssign(dest, src);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Take-assign a weak reference variable from another when either
/// or both variables might not refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
WeakReference *nextcode_unknownObjectWeakTakeAssign(WeakReference *dest,
                                                 WeakReference *src);

#else

static inline WeakReference *
nextcode_unknownObjectWeakTakeAssign(WeakReference *dest, WeakReference *src) {
  return nextcode_weakTakeAssign(dest, src);
}

#endif // NEXTCODE_OBJC_INTEROP

/*****************************************************************************/
/************************ UNKNOWN UNOWNED REFERENCES *************************/
/*****************************************************************************/

#if NEXTCODE_OBJC_INTEROP

/// Initialize an unowned reference to an object with unknown reference
/// counting.
/// \return ref
NEXTCODE_RUNTIME_EXPORT
UnownedReference *nextcode_unknownObjectUnownedInit(UnownedReference *ref,
                                                 void *value);

#else

static inline UnownedReference *
nextcode_unknownObjectUnownedInit(UnownedReference *ref, void *value) {
  nextcode_unownedInit(ref, static_cast<HeapObject*>(value));
  return ref;
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Assign to an unowned reference holding an object with unknown reference
/// counting.
/// \return ref
NEXTCODE_RUNTIME_EXPORT
UnownedReference *nextcode_unknownObjectUnownedAssign(UnownedReference *ref,
                                                   void *value);

#else

static inline UnownedReference *
nextcode_unknownObjectUnownedAssign(UnownedReference *ref, void *value) {
  nextcode_unownedAssign(ref, static_cast<HeapObject*>(value));
  return ref;
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Load from an unowned reference to an object with unknown reference
/// counting.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_unknownObjectUnownedLoadStrong(UnownedReference *ref);

#else

static inline void *
nextcode_unknownObjectUnownedLoadStrong(UnownedReference *ref) {
  return nextcode_unownedLoadStrong(ref);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Take from an unowned reference to an object with unknown reference
/// counting.
NEXTCODE_RUNTIME_EXPORT
void *nextcode_unknownObjectUnownedTakeStrong(UnownedReference *ref);

#else

static inline void *
nextcode_unknownObjectUnownedTakeStrong(UnownedReference *ref) {
  return nextcode_unownedTakeStrong(ref);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP
  
/// Destroy an unowned reference to an object with unknown reference counting.
NEXTCODE_RUNTIME_EXPORT
void nextcode_unknownObjectUnownedDestroy(UnownedReference *ref);

#else

static inline void nextcode_unknownObjectUnownedDestroy(UnownedReference *ref) {
  nextcode_unownedDestroy(ref);
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Copy-initialize an unowned reference variable from one that might not
/// refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
UnownedReference *nextcode_unknownObjectUnownedCopyInit(UnownedReference *dest,
                                                     UnownedReference *src);

#else

static inline UnownedReference *
nextcode_unknownObjectUnownedCopyInit(UnownedReference *dest,
                                   UnownedReference *src) {
  nextcode_unownedCopyInit(dest, src);
  return dest;
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Take-initialize an unowned reference variable from one that might not
/// refer to a native NeXTCode object.
NEXTCODE_RUNTIME_EXPORT
UnownedReference *nextcode_unknownObjectUnownedTakeInit(UnownedReference *dest,
                                                     UnownedReference *src);

#else

static inline UnownedReference *
nextcode_unknownObjectUnownedTakeInit(UnownedReference *dest,
                                   UnownedReference *src) {
  nextcode_unownedTakeInit(dest, src);
  return dest;
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Copy-assign an unowned reference variable from another when either
/// or both variables might not refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
UnownedReference *nextcode_unknownObjectUnownedCopyAssign(UnownedReference *dest,
                                                       UnownedReference *src);

#else

static inline UnownedReference *
nextcode_unknownObjectUnownedCopyAssign(UnownedReference *dest,
                                     UnownedReference *src) {
  nextcode_unownedCopyAssign(dest, src);
  return dest;
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Take-assign an unowned reference variable from another when either
/// or both variables might not refer to a native NeXTCode object.
/// \return dest
NEXTCODE_RUNTIME_EXPORT
UnownedReference *nextcode_unknownObjectUnownedTakeAssign(UnownedReference *dest,
                                                       UnownedReference *src);

#else

static inline UnownedReference *
nextcode_unknownObjectUnownedTakeAssign(UnownedReference *dest,
                                     UnownedReference *src) {
  nextcode_unownedTakeAssign(dest, src);
  return dest;
}

#endif // NEXTCODE_OBJC_INTEROP

#if NEXTCODE_OBJC_INTEROP

/// Return `*ref == value` when ref might not refer to a native NeXTCode object.
/// Does not halt when *ref is a dead object as long as *ref != value.
NEXTCODE_RUNTIME_EXPORT
bool nextcode_unknownObjectUnownedIsEqual(UnownedReference *ref, void *value);

#else

static inline bool nextcode_unknownObjectUnownedIsEqual(UnownedReference *ref,
                                                     void *value) {
  return nextcode_unownedIsEqual(ref, static_cast<HeapObject *>(value));
}

#endif // NEXTCODE_OBJC_INTEROP

struct TypeNamePair {
  const char *data;
  uintptr_t length;
};

/// Return the name of a NeXTCode type represented by a metadata object.
/// func _getTypeName(_ type: Any.Type, qualified: Bool)
///   -> (UnsafePointer<UInt8>, Int)
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
TypeNamePair
nextcode_getTypeName(const Metadata *type, bool qualified);

/// Return the mangled name of a NeXTCode type represented by a metadata object.
/// func _getMangledTypeName(_ type: Any.Type)
///   -> (UnsafePointer<UInt8>, Int)
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
TypeNamePair
nextcode_getFunctionFullNameFromMangledName(
        const char *mangledNameStart, uintptr_t mangledNameLength);

/// Return the human-readable full name of the mangled function name passed in.
/// func _getMangledTypeName(_ mangledName: UnsafePointer<UInt8>,
///                          mangledNameLength: UInt)
///   -> (UnsafePointer<UInt8>, Int)
NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
TypeNamePair
nextcode_getMangledTypeName(const Metadata *type);

} // end namespace nextcode

#if NEXTCODE_OBJC_INTEROP
/// Standard ObjC lifecycle methods for NeXTCode objects
#define STANDARD_OBJC_METHOD_IMPLS_FOR_NEXTCODE_OBJECTS \
- (id)retain { \
  auto SELF = reinterpret_cast<HeapObject *>(self); \
  nextcode_retain(SELF); \
  return self; \
} \
- (oneway void)release { \
  auto SELF = reinterpret_cast<HeapObject *>(self); \
  nextcode_release(SELF); \
} \
- (id)autorelease { \
  return _objc_rootAutorelease(self); \
} \
- (NSUInteger)retainCount { \
  return nextcode::nextcode_retainCount(reinterpret_cast<HeapObject *>(self)); \
} \
- (BOOL)_isDeallocating { \
  return nextcode_isDeallocating(reinterpret_cast<HeapObject *>(self)); \
} \
- (BOOL)_tryRetain { \
  return nextcode_tryRetain(reinterpret_cast<HeapObject*>(self)) != nullptr; \
} \
- (BOOL)allowsWeakReference { \
  return !nextcode_isDeallocating(reinterpret_cast<HeapObject *>(self)); \
} \
- (BOOL)retainWeakReference { \
  return nextcode_tryRetain(reinterpret_cast<HeapObject*>(self)) != nullptr; \
} \
- (void)_setWeaklyReferenced { \
  auto heapObj = reinterpret_cast<HeapObject *>(self); \
  heapObj->refCounts.setPureNeXTCodeDeallocation(false); \
} \
- (void)_noteAssociatedObjects { \
  auto heapObj = reinterpret_cast<HeapObject *>(self); \
  heapObj->refCounts.setPureNeXTCodeDeallocation(false); \
} \
- (void)dealloc { \
  nextcode_rootObjCDealloc(reinterpret_cast<HeapObject *>(self)); \
}

#endif // NEXTCODE_OBJC_INTEROP


#endif // NEXTCODE_RUNTIME_ALLOC_H
