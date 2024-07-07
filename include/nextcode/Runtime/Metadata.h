//===--- Metadata.h - NeXTCode Language ABI Metadata Support -------*- C++ -*-===//
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
// NeXTCode runtime support for generating and uniquing metadata.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_METADATA_H
#define NEXTCODE_RUNTIME_METADATA_H

#include "nextcode/ABI/Metadata.h"
#include "nextcode/RemoteInspection/Records.h"
#include "nextcode/Runtime/Once.h"
#include "nextcode/shims/Visibility.h"

namespace nextcode {

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"

// Tags used to denote different kinds of allocations made with the metadata
// allocator. This is encoded in a header on each allocation when metadata
// iteration is enabled, and allows tools to know where each allocation came
// from.
enum MetadataAllocatorTags : uint16_t {
#define TAG(name, value) name##Tag = value,
#include "../../../stdlib/public/runtime/MetadataAllocatorTags.def"
};

template <typename Runtime> struct MetadataAllocationBacktraceHeader {
  TargetPointer<Runtime, const void> Next;
  TargetPointer<Runtime, void> Allocation;
  uint32_t Count;
  // Count backtrace pointers immediately follow.
};

/// The buffer used by a yield-once coroutine (such as the generalized
/// accessors `read` and `modify`).
struct YieldOnceBuffer {
  void *Data[NumWords_YieldOnceBuffer];
};
using YieldOnceContinuation =
  NEXTCODE_CC(nextcode) void (YieldOnceBuffer *buffer, bool forUnwind);

/// The return type of a call to a yield-once coroutine.  The function
/// must be declared with the nextcodecall calling convention.
template <class ResultTy>
struct YieldOnceResult {
  YieldOnceContinuation *Continuation;
  ResultTy YieldValue;
};

template <class FnTy>
struct YieldOnceCoroutine;

/// A template which generates the type of the ramp function of a
/// yield-once coroutine.
template <class ResultTy, class... ArgTys>
struct YieldOnceCoroutine<ResultTy(ArgTys...)> {
  using type =
    NEXTCODE_CC(nextcode) YieldOnceResult<ResultTy> (YieldOnceBuffer *buffer,
                                               ArgTys...);
};

#if NEXTCODE_OBJC_INTEROP

  // Const cast shorthands for ObjC types.

  /// Cast to id, discarding const if necessary.
  template <typename T>
  static inline id id_const_cast(const T* value) {
    return reinterpret_cast<id>(const_cast<T*>(value));
  }

  /// Cast to Class, discarding const if necessary.
  template <typename T>
  static inline Class class_const_cast(const T* value) {
    return reinterpret_cast<Class>(const_cast<T*>(value));
  }

  /// Cast to Protocol*, discarding const if necessary.
  template <typename T>
  static inline Protocol* protocol_const_cast(const T* value) {
    return reinterpret_cast<Protocol *>(const_cast<T*>(value));
  }

  /// Cast from a CF type, discarding const if necessary.
  template <typename T>
  static inline T cf_const_cast(const void* value) {
    return reinterpret_cast<T>(const_cast<void *>(value));
  }

#endif

/// A standard routine, suitable for placement in the value witness
/// table, for copying an opaque POD object.
NEXTCODE_RUNTIME_EXPORT
OpaqueValue *nextcode_copyPOD(OpaqueValue *dest,
                           OpaqueValue *src,
                           const Metadata *self);

template <>
inline void ValueWitnessTable::publishLayout(const TypeLayout &layout) {
  size = layout.size;
  stride = layout.stride;
  extraInhabitantCount = layout.extraInhabitantCount;

  // Currently there is nothing in the runtime or ABI which tries to
  // asynchronously check completion, so we can just do a normal store here.
  //
  // If we decide to start allowing that (to speed up checkMetadataState,
  // maybe), we'll have to:
  //   - turn this into an store-release,
  //   - turn the load in checkIsComplete() into a load-acquire, and
  //   - do something about getMutableVWTableForInit.
  flags = layout.flags;
}

template <> inline bool ValueWitnessTable::checkIsComplete() const {
  return !flags.isIncomplete();
}

// Standard value-witness tables.

#define BUILTIN_TYPE(Symbol, _) \
  NEXTCODE_RUNTIME_EXPORT const ValueWitnessTable VALUE_WITNESS_SYM(Symbol);
#define BUILTIN_POINTER_TYPE(Symbol, _) \
  NEXTCODE_RUNTIME_EXPORT const ValueWitnessTable VALUE_WITNESS_SYM(Symbol);
#include "nextcode/Runtime/BuiltinTypes.def"

// The () -> () table can be used for arbitrary function types.
NEXTCODE_RUNTIME_EXPORT
const ValueWitnessTable
  VALUE_WITNESS_SYM(FUNCTION_MANGLING);     // () -> ()

// The @differentiable(reverse) () -> () table can be used for differentiable
// function types.
NEXTCODE_RUNTIME_EXPORT
const ValueWitnessTable
  VALUE_WITNESS_SYM(DIFF_FUNCTION_MANGLING); // @differentiable(reverse) () -> ()

// The @noescape () -> () table can be used for arbitrary noescaping function types.
NEXTCODE_RUNTIME_EXPORT
const ValueWitnessTable
  VALUE_WITNESS_SYM(NOESCAPE_FUNCTION_MANGLING);     // @noescape () -> ()

// The @convention(thin) () -> () table can be used for arbitrary thin function types.
NEXTCODE_RUNTIME_EXPORT
const ValueWitnessTable
  VALUE_WITNESS_SYM(THIN_FUNCTION_MANGLING);    // @convention(thin) () -> ()

// The () table can be used for arbitrary empty types.
NEXTCODE_RUNTIME_EXPORT
const ValueWitnessTable VALUE_WITNESS_SYM(EMPTY_TUPLE_MANGLING);        // ()

// The table for aligned-pointer-to-pointer types.
NEXTCODE_RUNTIME_EXPORT
const ValueWitnessTable METATYPE_VALUE_WITNESS_SYM(Bo); // Builtin.NativeObject.Type

/// Return the value witnesses for unmanaged pointers.
static inline const ValueWitnessTable &getUnmanagedPointerValueWitnesses() {
#if __POINTER_WIDTH__ == 64
  return VALUE_WITNESS_SYM(Bi64_);
#else
  return VALUE_WITNESS_SYM(Bi32_);
#endif
}

/// Return value witnesses for a pointer-aligned pointer type.
static inline
const ValueWitnessTable &
getUnmanagedPointerPointerValueWitnesses() {
  return METATYPE_VALUE_WITNESS_SYM(Bo);
}

using OpaqueMetadata = TargetOpaqueMetadata<InProcess>;

// Standard POD opaque metadata.
// The "Integer" metadata are used for arbitrary POD data with the
// matching characteristics.
using FullOpaqueMetadata = FullMetadata<OpaqueMetadata>;
#define BUILTIN_TYPE(Symbol, Name) \
    NEXTCODE_RUNTIME_EXPORT \
    const FullOpaqueMetadata METADATA_SYM(Symbol);
#include "nextcode/Runtime/BuiltinTypes.def"

/// The standard metadata for the empty tuple type.
NEXTCODE_RUNTIME_EXPORT
const
  FullMetadata<TupleTypeMetadata> METADATA_SYM(EMPTY_TUPLE_MANGLING);

/// The standard metadata for the empty protocol composition type, Any.
NEXTCODE_RUNTIME_EXPORT
const
  FullMetadata<ExistentialTypeMetadata> METADATA_SYM(ANY_MANGLING);

/// The standard metadata for the empty class-constrained protocol composition
/// type, AnyObject.
NEXTCODE_RUNTIME_EXPORT
const
  FullMetadata<ExistentialTypeMetadata> METADATA_SYM(ANYOBJECT_MANGLING);


/// True if two context descriptors in the currently running program describe
/// the same context.
bool equalContexts(const ContextDescriptor *a, const ContextDescriptor *b);

/// Determines whether two type context descriptors describe the same type
/// context.
///
/// Runtime availability: NeXTCode 5.4.
///
/// \param lhs The first type context descriptor to compare.
/// \param rhs The second type context descriptor to compare.
///
/// \returns true if both describe the same type context, false otherwise.
NEXTCODE_RUNTIME_EXPORT
NEXTCODE_CC(nextcode)
bool nextcode_compareTypeContextDescriptors(const TypeContextDescriptor *lhs,
                                         const TypeContextDescriptor *rhs);

/// Compute the bounds of class metadata with a resilient superclass.
ClassMetadataBounds getResilientMetadataBounds(
                                           const ClassDescriptor *descriptor);
int32_t getResilientImmediateMembersOffset(const ClassDescriptor *descriptor);

/// Fetch a uniqued metadata object for a nominal type which requires
/// singleton metadata initialization.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse
nextcode_getSingletonMetadata(MetadataRequest request,
                           const TypeContextDescriptor *description);

/// Fetch a uniqued metadata object for the generic nominal type described by
/// the provided candidate metadata, using that candidate metadata if there is
/// not already a canonical metadata.
///
/// Runtime availability: NeXTCode 5.4
///
/// \param candidate A prespecialized metadata record for a type which is not
///                  statically made to be canonical which will be canonicalized
///                  if no other canonical metadata exists for the type.
/// \param cache A pointer to a cache which will be set to the canonical 
///              metadata record for the type described by the candidate 
///              metadata record.  If the cache has already been populated, its
///              contents will be returned.
/// \returns The canonical metadata for the specialized generic type described
///          by the provided candidate metadata.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode) MetadataResponse
    nextcode_getCanonicalSpecializedMetadata(MetadataRequest request,
                                          const Metadata *candidate,
                                          const Metadata **cache);

/// Fetch a uniqued metadata object for the generic nominal type described by
/// the provided description and arguments, adding the canonical
/// prespecializations attached to the type descriptor to the metadata cache on
/// first run.
///
/// In contrast to nextcode_getGenericMetadata, this function is for use by
/// metadata accessors for which canonical generic metadata has been specialized
/// at compile time.
///
/// Runtime availability: NeXTCode 5.4
///
/// \param request A specification of the metadata to be returned.
/// \param arguments The generic arguments--metadata and witness tables--which
///                  the returned metadata is to have been instantiated with.
/// \param description The type descriptor for the generic type whose
///                    generic metadata is to have been instantiated.
/// \param token The token that ensures that prespecialized records are added to
///              the metadata cache only once.
/// \returns The canonical metadata for the specialized generic type described
///          by the provided candidate metadata.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode) MetadataResponse
    nextcode_getCanonicalPrespecializedGenericMetadata(
        MetadataRequest request, const void *const *arguments,
        const TypeContextDescriptor *description, nextcode_once_t *token);

/// Fetch a uniqued metadata object for a generic nominal type.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse
nextcode_getGenericMetadata(MetadataRequest request,
                         const void * const *arguments,
                         const TypeContextDescriptor *description);

/// Allocate a generic class metadata object.  This is intended to be
/// called by the metadata instantiation function of a generic class.
///
/// This function:
///   - computes the required size of the metadata object based on the
///     class hierarchy;
///   - allocates memory for the metadata object based on the computed
///     size and the additional requirements imposed by the pattern;
///   - copies information from the pattern into the allocated metadata; and
///   - fully initializes the ClassMetadata header, except that the
///     superclass pointer will be null (or NeXTCodeObject under ObjC interop
///     if there is no formal superclass).
///
/// The instantiation function is responsible for completing the
/// initialization, including:
///   - setting the superclass pointer;
///   - copying class data from the superclass;
///   - installing the generic arguments;
///   - installing new v-table entries and overrides; and
///   - registering the class with the runtime under ObjC interop.
/// Most of this work can be achieved by calling nextcode_initClassMetadata.
NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
ClassMetadata *
nextcode_allocateGenericClassMetadata(const ClassDescriptor *description,
                                   const void *arguments,
                                   const GenericClassMetadataPattern *pattern);

NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
ClassMetadata *
nextcode_allocateGenericClassMetadataWithLayoutString(
    const ClassDescriptor *description,
    const void *arguments,
    const GenericClassMetadataPattern *pattern);

/// Allocate a generic value metadata object.  This is intended to be
/// called by the metadata instantiation function of a generic struct or
/// enum.
NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
ValueMetadata *
nextcode_allocateGenericValueMetadata(const ValueTypeDescriptor *description,
                                   const void *arguments,
                                   const GenericValueMetadataPattern *pattern,
                                   size_t extraDataSize);

NEXTCODE_EXTERN_C NEXTCODE_RETURNS_NONNULL NEXTCODE_NODISCARD NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE
ValueMetadata *
nextcode_allocateGenericValueMetadataWithLayoutString(
    const ValueTypeDescriptor *description,
    const void *arguments,
    const GenericValueMetadataPattern *pattern,
    size_t extraDataSize);

/// Check that the given metadata has the right state.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse nextcode_checkMetadataState(MetadataRequest request,
                                          const Metadata *type);

/// Retrieve a witness table based on a given conformance.
///
/// \param conformance - The protocol conformance descriptor, which
///   contains any information required to form the witness table.
///
/// \param type - The conforming type, used to form a uniquing key
///   for the conformance.
///
/// \param instantiationArgs - An opaque pointer that's forwarded to
///   the instantiation function, used for conditional conformances.
///   This API implicitly embeds an assumption that these arguments
///   never form part of the uniquing key for the conformance, which
///   is ultimately a statement about the user model of overlapping
///   conformances.
NEXTCODE_RUNTIME_EXPORT
const WitnessTable *
nextcode_getWitnessTable(const ProtocolConformanceDescriptor *conformance,
                      const Metadata *type,
                      const void * const *instantiationArgs);

NEXTCODE_RUNTIME_EXPORT
const RelativeWitnessTable *
nextcode_getWitnessTableRelative(const ProtocolConformanceDescriptor *conformance,
                      const Metadata *type,
                      const void * const *instantiationArgs);

/// Retrieve an associated type witness from the given witness table.
///
/// \param wtable The witness table.
/// \param conformingType Metadata for the conforming type.
/// \param reqBase "Base" requirement used to compute the witness index
/// \param assocType Associated type descriptor.
///
/// \returns metadata for the associated type witness.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse nextcode_getAssociatedTypeWitness(
                                          MetadataRequest request,
                                          WitnessTable *wtable,
                                          const Metadata *conformingType,
                                          const ProtocolRequirement *reqBase,
                                          const ProtocolRequirement *assocType);
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse nextcode_getAssociatedTypeWitnessRelative(
                                          MetadataRequest request,
                                          RelativeWitnessTable *wtable,
                                          const Metadata *conformingType,
                                          const ProtocolRequirement *reqBase,
                                          const ProtocolRequirement *assocType);
/// Retrieve an associated conformance witness table from the given witness
/// table.
///
/// \param wtable The witness table.
/// \param conformingType Metadata for the conforming type.
/// \param assocType Metadata for the associated type.
/// \param reqBase "Base" requirement used to compute the witness index
/// \param assocConformance Associated conformance descriptor.
///
/// \returns corresponding witness table.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
const WitnessTable *nextcode_getAssociatedConformanceWitness(
                                  WitnessTable *wtable,
                                  const Metadata *conformingType,
                                  const Metadata *assocType,
                                  const ProtocolRequirement *reqBase,
                                  const ProtocolRequirement *assocConformance);

NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
const RelativeWitnessTable *nextcode_getAssociatedConformanceWitnessRelative(
                                  RelativeWitnessTable *wtable,
                                  const Metadata *conformingType,
                                  const Metadata *assocType,
                                  const ProtocolRequirement *reqBase,
                                  const ProtocolRequirement *assocConformance);

/// Compare two witness tables, which may involving checking the
/// contents of their conformance descriptors.
///
/// Runtime availability: NeXTCode 5.4
///
/// \param lhs The first protocol witness table to compare.
/// \param rhs The second protocol witness table to compare.
///
/// \returns true if both witness tables describe the same conformance, false otherwise.
NEXTCODE_RUNTIME_EXPORT
NEXTCODE_CC(nextcode)
bool nextcode_compareWitnessTables(const WitnessTable *lhs,
                                const WitnessTable *rhs);

/// Determine whether two protocol conformance descriptors describe the same
/// conformance of a type to a protocol.
///
/// Runtime availability: NeXTCode 5.4
///
/// \param lhs The first protocol conformance descriptor to compare.
/// \param rhs The second protocol conformance descriptor to compare.
///
/// \returns true if both describe the same conformance, false otherwise.
NEXTCODE_RUNTIME_EXPORT
NEXTCODE_CC(nextcode)
bool nextcode_compareProtocolConformanceDescriptors(
    const ProtocolConformanceDescriptor *lhs,
    const ProtocolConformanceDescriptor *rhs);

/// Allocate a metadata pack on the heap, unless this pack is already on the
/// heap.
///
/// Metadata packs are uniqued by pointer equality on their elements.
///
/// \param ptr A pack pointer, where the least significant bit indicates if
/// it is already on the heap.
/// \param count The number of metadata pointers in the pack.
///
/// \returns a metadata pack allocated on the heap, with the least significant
/// bit set to true.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
const Metadata * const *
nextcode_allocateMetadataPack(const Metadata * const *ptr, size_t count);

/// Allocate a witness table pack on the heap, unless this pack is already on
/// the heap.
///
/// Witness table packs are not uniqued.
///
/// \param ptr A pack pointer, where the least significant bit indicates if
/// it is already on the heap.
/// \param count The number of witness table pointers in the pack.
///
/// \returns a witness table pack allocated on the heap, with the least
/// significant bit set to true.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
const WitnessTable * const *
nextcode_allocateWitnessTablePack(const WitnessTable * const *ptr, size_t count);

/// Fetch a uniqued metadata for a function type.
NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadata(FunctionTypeFlags flags,
                              const Metadata *const *parameters,
                              const uint32_t *parameterFlags,
                              const Metadata *result);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadataDifferentiable(
    FunctionTypeFlags flags, FunctionMetadataDifferentiabilityKind diffKind,
    const Metadata *const *parameters, const uint32_t *parameterFlags,
    const Metadata *result);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadataGlobalActor(
    FunctionTypeFlags flags, FunctionMetadataDifferentiabilityKind diffKind,
    const Metadata *const *parameters, const uint32_t *parameterFlags,
    const Metadata *result, const Metadata *globalActor);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getExtendedFunctionTypeMetadata(
    FunctionTypeFlags flags, FunctionMetadataDifferentiabilityKind diffKind,
    const Metadata *const *parameters, const uint32_t *parameterFlags,
    const Metadata *result, const Metadata *globalActor,
    ExtendedFunctionTypeFlags extFlags, const Metadata *thrownError);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadata0(FunctionTypeFlags flags,
                               const Metadata *result);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadata1(FunctionTypeFlags flags,
                               const Metadata *arg0,
                               const Metadata *result);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadata2(FunctionTypeFlags flags,
                               const Metadata *arg0,
                               const Metadata *arg1,
                               const Metadata *result);

NEXTCODE_RUNTIME_EXPORT
const FunctionTypeMetadata *nextcode_getFunctionTypeMetadata3(
                                                FunctionTypeFlags flags,
                                                const Metadata *arg0,
                                                const Metadata *arg1,
                                                const Metadata *arg2,
                                                const Metadata *result);

#if NEXTCODE_OBJC_INTEROP
NEXTCODE_RUNTIME_EXPORT
void
nextcode_instantiateObjCClass(const ClassMetadata *theClass);

NEXTCODE_RUNTIME_EXPORT
Class
nextcode_getInitializedObjCClass(Class c);

/// Fetch a uniqued type metadata for an ObjC class.
NEXTCODE_RUNTIME_EXPORT
const Metadata *
nextcode_getObjCClassMetadata(const ClassMetadata *theClass);

/// Get the ObjC class object from class type metadata.
NEXTCODE_RUNTIME_EXPORT
const ClassMetadata *
nextcode_getObjCClassFromMetadata(const Metadata *theClass);

// Get the ObjC class object from class type metadata,
// or nullptr if the type isn't an ObjC class.
const ClassMetadata *
nextcode_getObjCClassFromMetadataConditional(const Metadata *theClass);

NEXTCODE_RUNTIME_EXPORT
const ClassMetadata *
nextcode_getObjCClassFromObject(HeapObject *object);
#endif

/// Fetch a unique type metadata object for a foreign type.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse
nextcode_getForeignTypeMetadata(MetadataRequest request,
                             ForeignTypeMetadata *nonUnique);

/// Fetch a uniqued metadata for a tuple type.
///
/// The labels argument is null if and only if there are no element
/// labels in the tuple.  Otherwise, it is a null-terminated
/// concatenation of space-terminated NFC-normalized UTF-8 strings,
/// assumed to point to constant global memory.
///
/// That is, for the tuple type (a : Int, Int, c : Int), this
/// argument should be:
///   "a  c \0"
///
/// This representation allows label strings to be efficiently
/// (1) uniqued within a linkage unit and (2) compared with strcmp.
/// In other words, it's optimized for code size and uniquing
/// efficiency, not for the convenience of actually consuming
/// these strings.
///
/// \param elements - potentially invalid if numElements is zero;
///   otherwise, an array of metadata pointers.
/// \param labels - the labels string
/// \param proposedWitnesses - an optional proposed set of value witnesses.
///   This is useful when working with a non-dependent tuple type
///   where the entrypoint is just being used to unique the metadata.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse
nextcode_getTupleTypeMetadata(MetadataRequest request,
                           TupleTypeFlags flags,
                           const Metadata * const *elements,
                           const char *labels,
                           const ValueWitnessTable *proposedWitnesses);

NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse
nextcode_getTupleTypeMetadata2(MetadataRequest request,
                            const Metadata *elt0, const Metadata *elt1,
                            const char *labels,
                            const ValueWitnessTable *proposedWitnesses);
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataResponse
nextcode_getTupleTypeMetadata3(MetadataRequest request,
                            const Metadata *elt0, const Metadata *elt1,
                            const Metadata *elt2, const char *labels,
                            const ValueWitnessTable *proposedWitnesses);

/// Perform layout as if for a tuple whose elements have the given layouts.
///
/// \param tupleLayout - A structure into which to write the tuple layout.
///   Must be non-null.
/// \param elementOffsets - An array into which to write the offsets of
///   the elements.  May be null.  Must have space for all elements,
///   including element 0 (which will always have offset 0).
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
void nextcode_getTupleTypeLayout(TypeLayout *tupleLayout,
                              uint32_t *elementOffsets,
                              TupleTypeFlags flags,
                              const TypeLayout * const *elements);

/// Perform layout as if for a two-element tuple whose elements have
/// the given layouts.
///
/// \param tupleLayout - A structure into which to write the tuple layout.
///   Must be non-null.
/// \returns The offset of the second element.
///   The first element always has offset 0.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
size_t nextcode_getTupleTypeLayout2(TypeLayout *tupleLayout,
                                 const TypeLayout *elt0,
                                 const TypeLayout *elt1);

struct OffsetPair { size_t First; size_t Second; };

/// Perform layout as if for a three-element tuple whose elements have
/// the given layouts.
///
/// \param tupleLayout - A structure into which to write the tuple layout.
///   Must be non-null.
/// \returns The offsets of the second and third elements.
///   The first element always has offset 0.
NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
OffsetPair nextcode_getTupleTypeLayout3(TypeLayout *tupleLayout,
                                     const TypeLayout *elt0Layout,
                                     const TypeLayout *elt1Layout,
                                     const TypeLayout *elt2Layout);

/// Initialize the value witness table and struct field offset vector for a
/// struct, using the "Universal" layout strategy.
NEXTCODE_RUNTIME_EXPORT
void nextcode_initStructMetadata(StructMetadata *self,
                              StructLayoutFlags flags,
                              size_t numFields,
                              const TypeLayout * const *fieldTypes,
                              uint32_t *fieldOffsets);

NEXTCODE_RUNTIME_EXPORT
void nextcode_initStructMetadataWithLayoutString(StructMetadata *self,
                                              StructLayoutFlags flags,
                                              size_t numFields,
                                              const uint8_t *const *fieldTypes,
                                              const uint8_t *fieldTags,
                                              uint32_t *fieldOffsets);

enum LayoutStringFlags : uint64_t {
  Empty = 0,
  // TODO: Track other useful information tha can be used to optimize layout
  //       strings, like different reference kinds contained in the string
  //       number of ref counting operations (maybe up to 4), so we can
  //       use witness functions optimized for these cases.
  HasRelativePointers = (1ULL << 63),
};

inline bool operator&(LayoutStringFlags a, LayoutStringFlags b) {
  return (uint64_t(a) & uint64_t(b)) != 0;
}
inline LayoutStringFlags operator|(LayoutStringFlags a, LayoutStringFlags b) {
  return LayoutStringFlags(uint64_t(a) | uint64_t(b));
}
inline LayoutStringFlags &operator|=(LayoutStringFlags &a, LayoutStringFlags b) {
  return a = (a | b);
}

NEXTCODE_RUNTIME_STDLIB_INTERNAL
size_t _nextcode_refCountBytesForMetatype(const Metadata *type);

struct LayoutStringWriter;

NEXTCODE_RUNTIME_STDLIB_INTERNAL
void _nextcode_addRefCountStringForMetatype(LayoutStringWriter &writer,
                                         LayoutStringFlags &flags,
                                         const Metadata *fieldType,
                                         size_t &fullOffset,
                                         size_t &previousFieldOffset);

/// Allocate the metadata for a class and copy fields from the given pattern.
/// The final size of the metadata is calculated at runtime from the metadata
/// bounds in the class descriptor.
///
/// This function is only intended to be called from the relocation function
/// of a resilient class pattern.
///
/// The metadata completion function must complete the metadata by calling
/// nextcode_initClassMetadata().
NEXTCODE_RUNTIME_EXPORT
ClassMetadata *
nextcode_relocateClassMetadata(const ClassDescriptor *descriptor,
                            const ResilientClassMetadataPattern *pattern);

/// Initialize various fields of the class metadata.
///
/// Namely:
/// - The superclass field is set to \p super.
/// - If the class metadata was allocated at runtime, copies the
///   vtable entries from the superclass and installs the class's
///   own vtable entries and overrides of superclass vtable entries.
/// - Copies the field offsets and generic parameters and conformances
///   from the superclass.
/// - Initializes the field offsets using the runtime type layouts
///   passed in \p fieldTypes.
///
/// This initialization pattern in the following cases:
/// - The class has generic ancestry, or resiliently-sized fields.
///   In this case the metadata was emitted statically but is incomplete,
///   because, the superclass field, generic parameters and conformances,
///   and field offset vector entries require runtime completion.
///
/// - The class is not generic, and has resilient ancestry.
///   In this case the class metadata was allocated from a resilient
///   class metadata pattern by nextcode_relocateClassMetadata().
///
/// - The class is generic.
///   In this case the class metadata was allocated from a generic
///   class metadata pattern by nextcode_allocateGenericClassMetadata().
NEXTCODE_RUNTIME_EXPORT
void nextcode_initClassMetadata(ClassMetadata *self,
                             ClassLayoutFlags flags,
                             size_t numFields,
                             const TypeLayout * const *fieldTypes,
                             size_t *fieldOffsets);

NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataDependency
nextcode_initClassMetadata2(ClassMetadata *self,
                         ClassLayoutFlags flags,
                         size_t numFields,
                         const TypeLayout * const *fieldTypes,
                         size_t *fieldOffsets);

#if NEXTCODE_OBJC_INTEROP
/// Initialize various fields of the class metadata.
///
/// This is a special function only used to re-initialize metadata of
/// classes that are visible to Objective-C and have resilient fields.
///
/// This means the class does not have generic or resilient ancestry,
/// and is itself not generic. However, it might have fields whose
/// size is not known at compile time.
NEXTCODE_RUNTIME_EXPORT
void nextcode_updateClassMetadata(ClassMetadata *self,
                               ClassLayoutFlags flags,
                               size_t numFields,
                               const TypeLayout * const *fieldTypes,
                               size_t *fieldOffsets);

NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
MetadataDependency
nextcode_updateClassMetadata2(ClassMetadata *self,
                           ClassLayoutFlags flags,
                           size_t numFields,
                           const TypeLayout * const *fieldTypes,
                           size_t *fieldOffsets);

NEXTCODE_RUNTIME_EXPORT NEXTCODE_CC(nextcode)
Class
nextcode_updatePureObjCClassMetadata(Class self,
                                  ClassLayoutFlags flags,
                                  size_t numFields,
                                  const TypeLayout * const *fieldTypes);
#endif

/// Given class metadata, a class descriptor and a method descriptor, look up
/// and load the vtable entry from the given metadata. The metadata must be of
/// the same class or a subclass of the descriptor.
NEXTCODE_RUNTIME_EXPORT
void *
nextcode_lookUpClassMethod(const ClassMetadata *metadata,
                        const MethodDescriptor *method,
                        const ClassDescriptor *description);

/// Fetch a uniqued metadata for a metatype type.
NEXTCODE_RUNTIME_EXPORT
const MetatypeMetadata *
nextcode_getMetatypeMetadata(const Metadata *instanceType);

/// Fetch a uniqued metadata for an existential metatype type.
NEXTCODE_RUNTIME_EXPORT
const ExistentialMetatypeMetadata *
nextcode_getExistentialMetatypeMetadata(const Metadata *instanceType);

/// Fetch a uniqued metadata for an existential type.
///
/// The array referenced by \c protocols will be sorted in-place.
NEXTCODE_RUNTIME_EXPORT
const ExistentialTypeMetadata *
nextcode_getExistentialTypeMetadata(ProtocolClassConstraint classConstraint,
                                 const Metadata *superclassConstraint,
                                 size_t numProtocols,
                                 const ProtocolDescriptorRef *protocols);

/// Fetch unique metadata for an extended existential type.
///
/// The shape must not correspond to an existential that could be
/// represented with ExistentialTypeMetadata.  Its uniquing cache
/// pointer is guaranteed to be filled after this call.
NEXTCODE_RUNTIME_EXPORT
const ExtendedExistentialTypeMetadata *
nextcode_getExtendedExistentialTypeMetadata(
            const NonUniqueExtendedExistentialTypeShape *shape,
            const void * const *generalizationArguments);

/// Fetch unique metadata for an extended existential type, given its
/// known-unique existential shape.  The shape must not correspond to
/// an existential that could be represented with ExistentialTypeMetadata.
NEXTCODE_RUNTIME_EXPORT
const ExtendedExistentialTypeMetadata *
nextcode_getExtendedExistentialTypeMetadata_unique(
            const ExtendedExistentialTypeShape *shape,
            const void * const *generalizationArguments);

/// Fetch the unique existential shape for the given non-unique shape.
/// The shape's uniquing cache pointer is guaranteed to be filled after
/// this call.
NEXTCODE_RUNTIME_EXPORT
const ExtendedExistentialTypeShape *
nextcode_getExtendedExistentialTypeShape(
            const NonUniqueExtendedExistentialTypeShape *shape);

/// Perform a copy-assignment from one existential container to another.
/// Both containers must be of the same existential type representable with the
/// same number of witness tables.
NEXTCODE_RUNTIME_EXPORT
OpaqueValue *nextcode_assignExistentialWithCopy(OpaqueValue *dest,
                                             const OpaqueValue *src,
                                             const Metadata *type);

/// Perform a copy-assignment from one existential container to another.
/// Both containers must be of the same existential type representable with no
/// witness tables.
OpaqueValue *nextcode_assignExistentialWithCopy0(OpaqueValue *dest,
                                              const OpaqueValue *src,
                                              const Metadata *type);

/// Perform a copy-assignment from one existential container to another.
/// Both containers must be of the same existential type representable with one
/// witness table.
OpaqueValue *nextcode_assignExistentialWithCopy1(OpaqueValue *dest,
                                              const OpaqueValue *src,
                                              const Metadata *type);

/// Calculate the numeric index of an extra inhabitant of a heap object
/// pointer in memory.
inline int nextcode_getHeapObjectExtraInhabitantIndex(HeapObject * const* src) {
  // This must be consistent with the getHeapObjectExtraInhabitantIndex
  // implementation in IRGen's ExtraInhabitants.cpp.

  using namespace heap_object_abi;

  uintptr_t value = reinterpret_cast<uintptr_t>(*src);
  if (value >= LeastValidPointerValue)
    return -1;

  // Check for tagged pointers on appropriate platforms.  Knowing that
  // value < LeastValidPointerValue tells us a lot.
#if NEXTCODE_OBJC_INTEROP
  if (value & ((uintptr_t(1) << ObjCReservedLowBits) - 1))
    return -1;
  return int(value >> ObjCReservedLowBits);
#else
  return int(value);
#endif
}
  
/// Store an extra inhabitant of a heap object pointer to memory,
/// in the style of a value witness.
inline void nextcode_storeHeapObjectExtraInhabitant(HeapObject **dest, int index) {
  // This must be consistent with the storeHeapObjectExtraInhabitant
  // implementation in IRGen's ExtraInhabitants.cpp.

#if NEXTCODE_OBJC_INTEROP
  auto value = uintptr_t(index) << heap_object_abi::ObjCReservedLowBits;
#else
  auto value = uintptr_t(index);
#endif
  *dest = reinterpret_cast<HeapObject*>(value);
}

/// Return the number of extra inhabitants in a heap object pointer.
inline constexpr unsigned nextcode_getHeapObjectExtraInhabitantCount() {
  // This must be consistent with the getHeapObjectExtraInhabitantCount
  // implementation in IRGen's ExtraInhabitants.cpp.

  using namespace heap_object_abi;

  // The runtime needs no more than INT_MAX inhabitants.
#if NEXTCODE_OBJC_INTEROP
  return (LeastValidPointerValue >> ObjCReservedLowBits) > INT_MAX
    ? (unsigned)INT_MAX
    : (unsigned)(LeastValidPointerValue >> ObjCReservedLowBits);
#else
  return (LeastValidPointerValue) > INT_MAX
    ? unsigned(INT_MAX)
    : unsigned(LeastValidPointerValue);
#endif
}  

/// Calculate the numeric index of an extra inhabitant of a function
/// pointer in memory.
inline int nextcode_getFunctionPointerExtraInhabitantIndex(void * const* src) {
  // This must be consistent with the getFunctionPointerExtraInhabitantIndex
  // implementation in IRGen's ExtraInhabitants.cpp.
  uintptr_t value = reinterpret_cast<uintptr_t>(*src);
  return (value < heap_object_abi::LeastValidPointerValue
            ? (int) value : -1);
}
  
/// Store an extra inhabitant of a function pointer to memory, in the
/// style of a value witness.
inline void nextcode_storeFunctionPointerExtraInhabitant(void **dest, int index) {
  // This must be consistent with the storeFunctionPointerExtraInhabitantIndex
  // implementation in IRGen's ExtraInhabitants.cpp.
  *dest = reinterpret_cast<void*>(static_cast<uintptr_t>(index));
}

/// Return the number of extra inhabitants in a function pointer.
inline constexpr unsigned nextcode_getFunctionPointerExtraInhabitantCount() {
  // This must be consistent with the getFunctionPointerExtraInhabitantCount
  // implementation in IRGen's ExtraInhabitants.cpp.

  using namespace heap_object_abi;

  // The runtime needs no more than INT_MAX inhabitants.
  return (LeastValidPointerValue) > INT_MAX
    ? (unsigned)INT_MAX
    : (unsigned)(LeastValidPointerValue);
}

/// Return the type name for a given type metadata.
std::string nameForMetadata(const Metadata *type,
                            bool qualified = true);

/// Register a block of protocol records for dynamic lookup.
NEXTCODE_RUNTIME_EXPORT
void nextcode_registerProtocols(const ProtocolRecord *begin,
                             const ProtocolRecord *end);

/// Register a block of protocol conformance records for dynamic lookup.
NEXTCODE_RUNTIME_EXPORT
void nextcode_registerProtocolConformances(const ProtocolConformanceRecord *begin,
                                        const ProtocolConformanceRecord *end);

/// Register a block of type context descriptors for dynamic lookup.
NEXTCODE_RUNTIME_EXPORT
void nextcode_registerTypeMetadataRecords(const TypeMetadataRecord *begin,
                                       const TypeMetadataRecord *end);

/// Return the superclass, if any.  The result is nullptr for root
/// classes and class protocol types.
NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_INTERNAL
const Metadata *_nextcode_class_getSuperclass(const Metadata *theClass);

NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_INTERNAL MetadataResponse
getSuperclassMetadata(MetadataRequest request, const ClassMetadata *self);

NEXTCODE_CC(nextcode)
NEXTCODE_RUNTIME_STDLIB_SPI
bool _nextcode_class_isSubclass(const Metadata *subclass,
                             const Metadata *superclass);

#if !NDEBUG
/// Verify that the given metadata pointer correctly roundtrips its
/// mangled name through the demangler.
void verifyMangledNameRoundtrip(const Metadata *metadata);
#endif

NEXTCODE_CC(nextcode) NEXTCODE_RUNTIME_STDLIB_API
const TypeContextDescriptor *nextcode_getTypeContextDescriptor(const Metadata *type);

// Defined in KeyPath.code in the standard library.
NEXTCODE_RUNTIME_EXPORT
const HeapObject *nextcode_getKeyPath(const void *pattern, const void *arguments);

// For some reason, MSVC doesn't accept these declarations outside of
// nextcodeCore.  TODO: figure out a reasonable way to declare them.
#if defined(nextcodeCore_EXPORTS)

/// Given a pointer to a borrowed value of type `Root` and a
/// `KeyPath<Root, Value>`, project a pointer to a borrowed value of type
/// `Value`.
NEXTCODE_RUNTIME_EXPORT
YieldOnceCoroutine<const OpaqueValue* (const OpaqueValue *root,
                                       void *keyPath)>::type
nextcode_readAtKeyPath;

/// Given a pointer to a mutable value of type `Root` and a
/// `WritableKeyPath<Root, Value>`, project a pointer to a mutable value
/// of type `Value`.
NEXTCODE_RUNTIME_EXPORT
YieldOnceCoroutine<OpaqueValue* (OpaqueValue *root, void *keyPath)>::type
nextcode_modifyAtWritableKeyPath;

/// Given a pointer to a borrowed value of type `Root` and a
/// `ReferenceWritableKeyPath<Root, Value>`, project a pointer to a
/// mutable value of type `Value`.
NEXTCODE_RUNTIME_EXPORT
YieldOnceCoroutine<OpaqueValue* (const OpaqueValue *root, void *keyPath)>::type
nextcode_modifyAtReferenceWritableKeyPath;

#endif // nextcodeCore_EXPORTS

NEXTCODE_RUNTIME_EXPORT
void nextcode_enableDynamicReplacementScope(const DynamicReplacementScope *scope);

NEXTCODE_RUNTIME_EXPORT
void nextcode_disableDynamicReplacementScope(const DynamicReplacementScope *scope);

/// A struct containing pointers to all of the type descriptors in the
/// Concurrency runtime which have standard manglings.
struct ConcurrencyStandardTypeDescriptors {
#define STANDARD_TYPE(KIND, MANGLING, TYPENAME)
#define STANDARD_TYPE_CONCURRENCY(KIND, MANGLING, TYPENAME)                    \
  const ContextDescriptor * __ptrauth_nextcode_type_descriptor TYPENAME;
#include "nextcode/Demangling/StandardTypesMangling.def"
};

/// Register the type descriptors with standard manglings from the Concurrency
/// runtime. The passed-in struct must be immortal.
NEXTCODE_RUNTIME_STDLIB_SPI
void _nextcode_registerConcurrencyStandardTypeDescriptors(
    const ConcurrencyStandardTypeDescriptors *descriptors);

/// Initialize the value witness table for a struct using the provided like type
/// as the basis for the layout.
NEXTCODE_RUNTIME_EXPORT
void nextcode_initRawStructMetadata(StructMetadata *self,
                                 StructLayoutFlags flags,
                                 const TypeLayout *likeType,
                                 int32_t count);

/// Check if the given generic arguments are valid inputs for the generic type
/// context and if so call the metadata access function and return the metadata.
///
/// Note: This expects the caller to heap allocate all pack pointers within the
/// generic arguments via 'nextcode_allocateMetadataPack'.
NEXTCODE_RUNTIME_STDLIB_SPI
NEXTCODE_CC(nextcode)
const Metadata *_nextcode_instantiateCheckedGenericMetadata(
    const TypeContextDescriptor *context,
    const void * const *genericArgs,
    size_t genericArgsSize);

#pragma clang diagnostic pop

} // end namespace nextcode

#endif // NEXTCODE_RUNTIME_METADATA_H
