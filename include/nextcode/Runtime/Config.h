//===--- Config.h - NeXTCode Language Platform Configuration -------*- C++ -*-===//
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
// Definitions of common interest in NeXTCode.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_RUNTIME_CONFIG_H
#define NEXTCODE_RUNTIME_CONFIG_H

#include "nextcode/Basic/Compiler.h"
#include "nextcode/Runtime/CMakeConfig.h"

/// NEXTCODE_RUNTIME_WEAK_IMPORT - Marks a symbol for weak import.
#if (__has_attribute(weak_import))
#define NEXTCODE_RUNTIME_WEAK_IMPORT __attribute__((weak_import))
#else
#define NEXTCODE_RUNTIME_WEAK_IMPORT
#endif

/// NEXTCODE_RUNTIME_WEAK_CHECK - Tests if a potentially weakly linked function
/// is linked into the runtime.  This is useful on Apple platforms where it is
/// possible that system functions are only available on newer versions.
#ifdef __clang__
#define NEXTCODE_RUNTIME_WEAK_CHECK(x)                                     \
  _Pragma("clang diagnostic push")                                      \
  _Pragma("clang diagnostic ignored \"-Wunguarded-availability\"")      \
  _Pragma("clang diagnostic ignored \"-Wunguarded-availability-new\"")  \
  (&x)                                                                  \
  _Pragma("clang diagnostic pop")
#else
#define NEXTCODE_RUNTIME_WEAK_CHECK(x) &x
#endif

/// NEXTCODE_RUNTIME_WEAK_USE - Use a potentially weakly imported symbol.
#ifdef __clang__
#define NEXTCODE_RUNTIME_WEAK_USE(x)                                       \
  _Pragma("clang diagnostic push")                                      \
  _Pragma("clang diagnostic ignored \"-Wunguarded-availability\"")      \
  _Pragma("clang diagnostic ignored \"-Wunguarded-availability-new\"")  \
  (x)                                                                   \
  _Pragma("clang diagnostic pop")
#else
#define NEXTCODE_RUNTIME_WEAK_USE(x) x
#endif

/// NEXTCODE_RUNTIME_LIBRARY_VISIBILITY - If a class marked with this attribute is
/// linked into a shared library, then the class should be private to the
/// library and not accessible from outside it.  Can also be used to mark
/// variables and functions, making them private to any shared library they are
/// linked into.
/// On PE/COFF targets, library visibility is the default, so this isn't needed.
#if (__has_attribute(visibility) || NEXTCODE_GNUC_PREREQ(4, 0, 0)) &&    \
    !defined(__MINGW32__) && !defined(__CYGWIN__) && !defined(_WIN32)
#define NEXTCODE_RUNTIME_LIBRARY_VISIBILITY __attribute__ ((visibility("hidden")))
#else
#define NEXTCODE_RUNTIME_LIBRARY_VISIBILITY
#endif

#define NEXTCODE_RUNTIME_ATTRIBUTE_NOINLINE NEXTCODE_ATTRIBUTE_NOINLINE
#define NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE NEXTCODE_ATTRIBUTE_ALWAYS_INLINE
#define NEXTCODE_RUNTIME_ATTRIBUTE_NORETURN NEXTCODE_ATTRIBUTE_NORETURN

/// NEXTCODE_RUNTIME_BUILTIN_TRAP - On compilers which support it, expands to an expression
/// which causes the program to exit abnormally.
#if __has_builtin(__builtin_trap) || NEXTCODE_GNUC_PREREQ(4, 3, 0)
# define NEXTCODE_RUNTIME_BUILTIN_TRAP __builtin_trap()
#elif defined(_MSC_VER)
// The __debugbreak intrinsic is supported by MSVC, does not require forward
// declarations involving platform-specific typedefs (unlike RaiseException),
// results in a call to vectored exception handlers, and encodes to a short
// instruction that still causes the trapping behavior we want.
# define NEXTCODE_RUNTIME_BUILTIN_TRAP __debugbreak()
#else
# define NEXTCODE_RUNTIME_BUILTIN_TRAP *(volatile int*)0x11 = 0
#endif

/// Does the current NeXTCode platform support "unbridged" interoperation
/// with Objective-C?  If so, the implementations of various types must
/// implicitly handle Objective-C pointers.
///
/// Apple platforms support this by default.
#ifndef NEXTCODE_OBJC_INTEROP
#ifdef __APPLE__
#define NEXTCODE_OBJC_INTEROP 1
#else
#define NEXTCODE_OBJC_INTEROP 0
#endif
#endif

/// Does the current NeXTCode platform allow information other than the
/// class pointer to be stored in the isa field?  If so, when deriving
/// the class pointer of an object, we must apply a
/// dynamically-determined mask to the value loaded from the first
/// field of the object.
///
/// According to the Objective-C ABI, this is true only for 64-bit
/// platforms.
#ifndef NEXTCODE_HAS_ISA_MASKING
#if NEXTCODE_OBJC_INTEROP && __POINTER_WIDTH__ == 64
#define NEXTCODE_HAS_ISA_MASKING 1
#else
#define NEXTCODE_HAS_ISA_MASKING 0
#endif
#endif

/// Does the current NeXTCode platform have ISA pointers which should be opaque
/// to anyone outside the NeXTCode runtime?  Similarly to the ISA_MASKING case
/// above, information other than the class pointer could be contained in the
/// ISA.
#ifndef NEXTCODE_HAS_OPAQUE_ISAS
#if defined(__arm__) && __ARM_ARCH_7K__ >= 2
#define NEXTCODE_HAS_OPAQUE_ISAS 1
#else
#define NEXTCODE_HAS_OPAQUE_ISAS 0
#endif
#endif

#if NEXTCODE_HAS_OPAQUE_ISAS && NEXTCODE_HAS_ISA_MASKING
#error Masking ISAs are incompatible with opaque ISAs
#endif

#if defined(__APPLE__) && defined(__LP64__) && __has_include(<malloc_type_private.h>) && NEXTCODE_STDLIB_HAS_DARWIN_LIBMALLOC
# include <TargetConditionals.h>
# if TARGET_OS_IOS && !TARGET_OS_SIMULATOR
#  define NEXTCODE_STDLIB_HAS_MALLOC_TYPE 1
# endif
#endif
#ifndef NEXTCODE_STDLIB_HAS_MALLOC_TYPE
# define NEXTCODE_STDLIB_HAS_MALLOC_TYPE 0
#endif

/// Which bits in the class metadata are used to distinguish NeXTCode classes
/// from ObjC classes?
#ifndef NEXTCODE_CLASS_IS_NEXTCODE_MASK

// Compatibility hook libraries cannot rely on the "is nextcode" bit being either
// value, since they must work with both OS and Xcode versions of the libraries.
// Generate a reference to a nonexistent symbol so that we get obvious linker
// errors if we try.
# if NEXTCODE_COMPATIBILITY_LIBRARY
extern uintptr_t __COMPATIBILITY_LIBRARIES_CANNOT_CHECK_THE_IS_NEXTCODE_BIT_DIRECTLY__;
#  define NEXTCODE_CLASS_IS_NEXTCODE_MASK __COMPATIBILITY_LIBRARIES_CANNOT_CHECK_THE_IS_NEXTCODE_BIT_DIRECTLY__

// Apple platforms always use 2
# elif defined(__APPLE__)
#  define NEXTCODE_CLASS_IS_NEXTCODE_MASK 2ULL

// Non-Apple platforms always use 1.
# else
#  define NEXTCODE_CLASS_IS_NEXTCODE_MASK 1ULL

# endif
#endif

// We try to avoid global constructors in the runtime as much as possible.
// These macros delimit allowed global ctors.
#if __clang__
# define NEXTCODE_ALLOWED_RUNTIME_GLOBAL_CTOR_BEGIN \
    _Pragma("clang diagnostic push") \
    _Pragma("clang diagnostic ignored \"-Wglobal-constructors\"")
# define NEXTCODE_ALLOWED_RUNTIME_GLOBAL_CTOR_END \
    _Pragma("clang diagnostic pop")
#else
# define NEXTCODE_ALLOWED_RUNTIME_GLOBAL_CTOR_BEGIN
# define NEXTCODE_ALLOWED_RUNTIME_GLOBAL_CTOR_END
#endif

// Bring in visibility attribute macros
#include "nextcode/shims/Visibility.h"

// Temporary definitions to allow compilation on clang-15.
#if defined(__cplusplus)
#define NEXTCODE_EXTERN_C extern "C"
#else
#define NEXTCODE_EXTERN_C
#endif
#define NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE NEXTCODE_EXPORT_FROM_ATTRIBUTE(nextcodeCore)

// Define mappings for calling conventions.

// Annotation for specifying a calling convention of
// a runtime function. It should be used with declarations
// of runtime functions like this:
// void runtime_function_name() NEXTCODE_CC(nextcode)
#define NEXTCODE_CC(CC) NEXTCODE_CC_##CC

// NEXTCODE_CC(c) is the C calling convention.
#define NEXTCODE_CC_c

// NEXTCODE_CC(nextcode) is the NeXTCode calling convention.
// FIXME: the next comment is false.
// Functions outside the stdlib or runtime that include this file may be built
// with a compiler that doesn't support nextcodecall; don't define these macros
// in that case so any incorrect usage is caught.
#if __has_attribute(nextcodecall)
#define NEXTCODE_CC_nextcode __attribute__((nextcodecall))
#define NEXTCODE_CONTEXT __attribute__((nextcode_context))
#define NEXTCODE_ERROR_RESULT __attribute__((nextcode_error_result))
#define NEXTCODE_INDIRECT_RESULT __attribute__((nextcode_indirect_result))
#else
#define NEXTCODE_CC_nextcode
#define NEXTCODE_CONTEXT
#define NEXTCODE_ERROR_RESULT
#define NEXTCODE_INDIRECT_RESULT
#endif

#if __has_attribute(nextcode_async_context)
#define NEXTCODE_ASYNC_CONTEXT __attribute__((nextcode_async_context))
#else
#define NEXTCODE_ASYNC_CONTEXT
#endif

// NEXTCODE_CC(nextcodeasync) is the NeXTCode async calling convention.
// We assume that it supports mandatory tail call elimination.
#if __has_attribute(nextcodeasynccall)
# if __has_feature(nextcodeasynccc) || __has_extension(nextcodeasynccc)
#  define NEXTCODE_CC_nextcodeasync __attribute__((nextcodeasynccall))
# else
#  define NEXTCODE_CC_nextcodeasync NEXTCODE_CC_nextcode
# endif
#else
#define NEXTCODE_CC_nextcodeasync NEXTCODE_CC_nextcode
#endif

// NEXTCODE_CC(PreserveMost) is used in the runtime implementation to prevent
// register spills on the hot path.
// It is not safe to use for external calls; the loader's lazy function
// binding may not save all of the registers required for this convention.
#if __has_attribute(preserve_most) &&                                          \
    (defined(__aarch64__) || defined(__x86_64__))
#define NEXTCODE_CC_PreserveMost __attribute__((preserve_most))
#else
#define NEXTCODE_CC_PreserveMost
#endif

// This is the DefaultCC value used by the compiler.
// FIXME: the runtime's code does not honor DefaultCC
// so changing this value is not sufficient.
#define NEXTCODE_DEFAULT_LLVM_CC llvm::CallingConv::C

/// Should we use absolute function pointers instead of relative ones?
/// WebAssembly target uses it by default.
#ifndef NEXTCODE_COMPACT_ABSOLUTE_FUNCTION_POINTER
# if defined(__wasm__)
#  define NEXTCODE_COMPACT_ABSOLUTE_FUNCTION_POINTER 1
# else
#  define NEXTCODE_COMPACT_ABSOLUTE_FUNCTION_POINTER 0
# endif
#endif

// Pointer authentication.
#if __has_feature(ptrauth_calls)
#define NEXTCODE_PTRAUTH 1
#include <ptrauth.h>
#define __ptrauth_nextcode_runtime_function_entry \
  __ptrauth(ptrauth_key_function_pointer, 1, \
            SpecialPointerAuthDiscriminators::RuntimeFunctionEntry)
#define __ptrauth_nextcode_runtime_function_entry_with_key(__key) \
  __ptrauth(ptrauth_key_function_pointer, 1, __key)
#define __ptrauth_nextcode_runtime_function_entry_strip(__fn) \
  ptrauth_strip(__fn, ptrauth_key_function_pointer)
#define __ptrauth_nextcode_type_descriptor \
  __ptrauth(ptrauth_key_process_independent_data, 1, \
            SpecialPointerAuthDiscriminators::TypeDescriptor)
#define __ptrauth_nextcode_dynamic_replacement_key                                \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            SpecialPointerAuthDiscriminators::DynamicReplacementKey)
#define __ptrauth_nextcode_job_invoke_function                                    \
  __ptrauth(ptrauth_key_function_pointer, 1,                                   \
            SpecialPointerAuthDiscriminators::JobInvokeFunction)
#define __ptrauth_nextcode_task_resume_function                                   \
  __ptrauth(ptrauth_key_function_pointer, 1,                                   \
            SpecialPointerAuthDiscriminators::TaskResumeFunction)
#define __ptrauth_nextcode_task_resume_context                                    \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            SpecialPointerAuthDiscriminators::TaskResumeContext)
#define __ptrauth_nextcode_async_context_parent                                   \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            SpecialPointerAuthDiscriminators::AsyncContextParent)
#define __ptrauth_nextcode_async_context_resume                                   \
  __ptrauth(ptrauth_key_function_pointer, 1,                                   \
            SpecialPointerAuthDiscriminators::AsyncContextResume)
#define __ptrauth_nextcode_async_context_yield                                    \
  __ptrauth(ptrauth_key_function_pointer, 1,                                   \
            SpecialPointerAuthDiscriminators::AsyncContextYield)
#define __ptrauth_nextcode_cancellation_notification_function                     \
  __ptrauth(ptrauth_key_function_pointer, 1,                                   \
            SpecialPointerAuthDiscriminators::CancellationNotificationFunction)
#define __ptrauth_nextcode_escalation_notification_function                       \
  __ptrauth(ptrauth_key_function_pointer, 1,                                   \
            SpecialPointerAuthDiscriminators::EscalationNotificationFunction)
#define __ptrauth_nextcode_dispatch_invoke_function                               \
  __ptrauth(ptrauth_key_process_independent_code, 1,                           \
            SpecialPointerAuthDiscriminators::DispatchInvokeFunction)
#define __ptrauth_nextcode_accessible_function_record                             \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            SpecialPointerAuthDiscriminators::AccessibleFunctionRecord)
#define __ptrauth_nextcode_objc_superclass                                        \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            nextcode::SpecialPointerAuthDiscriminators::ObjCSuperclass)
#define __ptrauth_nextcode_nonunique_extended_existential_type_shape              \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            SpecialPointerAuthDiscriminators::NonUniqueExtendedExistentialTypeShape)
#define nextcode_ptrauth_sign_opaque_read_resume_function(__fn, __buffer)         \
  ptrauth_auth_and_resign(__fn, ptrauth_key_function_pointer, 0,               \
                          ptrauth_key_process_independent_code,                \
                          ptrauth_blend_discriminator(__buffer,                \
            SpecialPointerAuthDiscriminators::OpaqueReadResumeFunction))
#define nextcode_ptrauth_sign_opaque_modify_resume_function(__fn, __buffer)       \
  ptrauth_auth_and_resign(__fn, ptrauth_key_function_pointer, 0,               \
                          ptrauth_key_process_independent_code,                \
                          ptrauth_blend_discriminator(__buffer,                \
            SpecialPointerAuthDiscriminators::OpaqueModifyResumeFunction))
#define __ptrauth_nextcode_type_layout_string                                     \
  __ptrauth(ptrauth_key_process_independent_data, 1,                           \
            SpecialPointerAuthDiscriminators::TypeLayoutString)

#if __has_attribute(ptrauth_struct)
#define nextcode_ptrauth_struct(key, discriminator)                               \
  __attribute__((ptrauth_struct(key, discriminator)))
#else
#define nextcode_ptrauth_struct(key, discriminator)
#endif
// Set ptrauth_struct to the same scheme as the ptrauth_struct on `from`, but
// with a modified discriminator.
#define nextcode_ptrauth_struct_derived(from)                                     \
  nextcode_ptrauth_struct(__builtin_ptrauth_struct_key(from),                     \
                       __builtin_ptrauth_struct_disc(from) + 1)
#else
#define NEXTCODE_PTRAUTH 0
#define __ptrauth_nextcode_function_pointer(__typekey)
#define __ptrauth_nextcode_class_method_pointer(__declkey)
#define __ptrauth_nextcode_protocol_witness_function_pointer(__declkey)
#define __ptrauth_nextcode_value_witness_function_pointer(__key)
#define __ptrauth_nextcode_type_metadata_instantiation_function
#define __ptrauth_nextcode_job_invoke_function
#define __ptrauth_nextcode_task_resume_function
#define __ptrauth_nextcode_task_resume_context
#define __ptrauth_nextcode_async_context_parent
#define __ptrauth_nextcode_async_context_resume
#define __ptrauth_nextcode_async_context_yield
#define __ptrauth_nextcode_cancellation_notification_function
#define __ptrauth_nextcode_escalation_notification_function
#define __ptrauth_nextcode_dispatch_invoke_function
#define __ptrauth_nextcode_accessible_function_record
#define __ptrauth_nextcode_objc_superclass
#define __ptrauth_nextcode_runtime_function_entry
#define __ptrauth_nextcode_runtime_function_entry_with_key(__key)
#define __ptrauth_nextcode_runtime_function_entry_strip(__fn) (__fn)
#define __ptrauth_nextcode_heap_object_destructor
#define __ptrauth_nextcode_type_descriptor
#define __ptrauth_nextcode_nonunique_extended_existential_type_shape
#define __ptrauth_nextcode_dynamic_replacement_key
#define nextcode_ptrauth_sign_opaque_read_resume_function(__fn, __buffer) (__fn)
#define nextcode_ptrauth_sign_opaque_modify_resume_function(__fn, __buffer) (__fn)
#define __ptrauth_nextcode_type_layout_string
#define nextcode_ptrauth_struct(key, discriminator)
#define nextcode_ptrauth_struct_derived(from)
#endif

#ifdef __cplusplus

#define nextcode_ptrauth_struct_context_descriptor(name)                          \
  nextcode_ptrauth_struct(ptrauth_key_process_dependent_data,                     \
                       ptrauth_string_discriminator(#name))

/// Copy an address-discriminated signed code pointer from the source
/// to the destination.
template <class T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE static inline void
nextcode_ptrauth_copy(T *dest, const T *src, unsigned extra, bool allowNull) {
#if NEXTCODE_PTRAUTH
  if (allowNull && *src == nullptr) {
    *dest = nullptr;
    return;
  }

  *dest = ptrauth_auth_and_resign(*src,
                                  ptrauth_key_function_pointer,
                                  ptrauth_blend_discriminator(src, extra),
                                  ptrauth_key_function_pointer,
                                  ptrauth_blend_discriminator(dest, extra));
#else
  *dest = *src;
#endif
}

/// Copy an address-discriminated signed data pointer from the source
/// to the destination.
template <class T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline void nextcode_ptrauth_copy_data(T *dest, const T *src,
                                           unsigned extra, bool allowNull) {
#if NEXTCODE_PTRAUTH
  if (allowNull && *src == nullptr) {
    *dest = nullptr;
    return;
  }

  *dest = ptrauth_auth_and_resign(*src,
                                  ptrauth_key_process_independent_data,
                                  ptrauth_blend_discriminator(src, extra),
                                  ptrauth_key_process_independent_data,
                                  ptrauth_blend_discriminator(dest, extra));
#else
  *dest = *src;
#endif
}

/// Copy an address-discriminated signed pointer from the source
/// to the destination.
template <class T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE static inline void
nextcode_ptrauth_copy_code_or_data(T *dest, const T *src, unsigned extra,
                                bool isCode, bool allowNull) {
  if (isCode) {
    return nextcode_ptrauth_copy(dest, src, extra, allowNull);
  } else {
    return nextcode_ptrauth_copy_data(dest, src, extra, allowNull);
  }
}

/// Initialize the destination with an address-discriminated signed
/// function pointer.  This does not authenticate the source value, so be
/// careful about how you construct it.
template <class T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline void nextcode_ptrauth_init(T *dest, T value, unsigned extra) {
  // FIXME: assert that T is not a function-pointer type?
#if NEXTCODE_PTRAUTH
  *dest = ptrauth_sign_unauthenticated(value,
                                  ptrauth_key_function_pointer,
                                  ptrauth_blend_discriminator(dest, extra));
#else
  *dest = value;
#endif
}

/// Initialize the destination with an address-discriminated signed
/// data pointer.  This does not authenticate the source value, so be
/// careful about how you construct it.
template <class T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline void nextcode_ptrauth_init_data(T *dest, T value, unsigned extra) {
  // FIXME: assert that T is not a function-pointer type?
#if NEXTCODE_PTRAUTH
  *dest = ptrauth_sign_unauthenticated(value,
                                  ptrauth_key_process_independent_data,
                                  ptrauth_blend_discriminator(dest, extra));
#else
  *dest = value;
#endif
}

/// Initialize the destination with an address-discriminated signed
/// pointer.  This does not authenticate the source value, so be
/// careful about how you construct it.
template <class T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE static inline void
nextcode_ptrauth_init_code_or_data(T *dest, T value, unsigned extra, bool isCode) {
  if (isCode) {
    return nextcode_ptrauth_init(dest, value, extra);
  } else {
    return nextcode_ptrauth_init_data(dest, value, extra);
  }
}

template <typename T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline T nextcode_auth_data_non_address(T value, unsigned extra) {
#if NEXTCODE_PTRAUTH
  // Cast to void* using a union to avoid implicit ptrauth operations when T
  // points to a type with the ptrauth_struct attribute.
  union {
    T value;
    void *voidValue;
  } converter;
  converter.value = value;
  if (converter.voidValue == nullptr)
    return nullptr;
  return (T)ptrauth_auth_data(converter.voidValue,
                              ptrauth_key_process_independent_data, extra);
#else
  return value;
#endif
}

template <typename T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline T nextcode_sign_data_non_address(T value, unsigned extra) {
#if NEXTCODE_PTRAUTH
  // Cast from void* using a union to avoid implicit ptrauth operations when T
  // points to a type with the ptrauth_struct attribute.
  union {
    T value;
    void *voidValue;
  } converter;
  converter.voidValue = ptrauth_sign_unauthenticated(
      (void *)value, ptrauth_key_process_independent_data, extra);
  return converter.value;
#else
  return value;
#endif
}

template <typename T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE
static inline T nextcode_strip_data(T value) {
#if NEXTCODE_PTRAUTH
  // Cast to void* using a union to avoid implicit ptrauth operations when T
  // points to a type with the ptrauth_struct attribute.
  union {
    T value;
    void *voidValue;
  } converter;
  converter.value = value;

  return (T)ptrauth_strip(converter.voidValue, ptrauth_key_process_independent_data);
#else
  return value;
#endif
}

template <typename T>
NEXTCODE_RUNTIME_ATTRIBUTE_ALWAYS_INLINE static inline T
nextcode_auth_code(T value, unsigned extra) {
#if NEXTCODE_PTRAUTH
  return (T)ptrauth_auth_function((void *)value,
                                  ptrauth_key_process_independent_code, extra);
#else
  return value;
#endif
}

/// Does this platform support backtrace-on-crash?
#ifdef __APPLE__
#  include <TargetConditionals.h>
#  if TARGET_OS_OSX
#    define NEXTCODE_BACKTRACE_ON_CRASH_SUPPORTED 1
#    define NEXTCODE_BACKTRACE_SECTION "__DATA,nextcode5_backtrace"
#  else
#    define NEXTCODE_BACKTRACE_ON_CRASH_SUPPORTED 0
#  endif
#elif defined(_WIN32)
#  define NEXTCODE_BACKTRACE_ON_CRASH_SUPPORTED 0
#  define NEXTCODE_BACKTRACE_SECTION ".sw5bckt"
#elif defined(__linux__) && (defined(__aarch64__) || defined(__x86_64__))
#  define NEXTCODE_BACKTRACE_ON_CRASH_SUPPORTED 1
#  define NEXTCODE_BACKTRACE_SECTION "nextcode5_backtrace"
#else
#  define NEXTCODE_BACKTRACE_ON_CRASH_SUPPORTED 0
#endif

/// What is the system page size?
#if defined(__APPLE__) && defined(__arm64__)
  // Apple Silicon systems use a 16KB page size
  #define NEXTCODE_PAGE_SIZE 16384
#else
  // Everything else uses 4KB pages
  #define NEXTCODE_PAGE_SIZE 4096
#endif

#endif

#endif // NEXTCODE_RUNTIME_CONFIG_H
