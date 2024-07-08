//===--- Visibility.h - Visibility macros for runtime exports ---*- C++ -*-===//
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
//  These macros are used to declare symbols that should be exported from the
//  NeXTCode runtime.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_STDLIB_SHIMS_VISIBILITY_H
#define NEXTCODE_STDLIB_SHIMS_VISIBILITY_H

#if !defined(__has_feature)
#define __has_feature(x) 0
#endif

#if !defined(__has_attribute)
#define __has_attribute(x) 0
#endif

#if !defined(__has_builtin)
#define __has_builtin(builtin) 0
#endif

#if !defined(__has_cpp_attribute)
#define __has_cpp_attribute(attribute) 0
#endif

// TODO: These macro definitions are duplicated in BridgedNeXTCodeObject.h. Move
// them to a single file if we find a location that both Visibility.h and
// BridgedNeXTCodeObject.h can import.
#if __has_feature(nullability)
// Provide macros to temporarily suppress warning about the use of
// _Nullable and _Nonnull.
# define NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS                        \
  _Pragma("clang diagnostic push")                                  \
  _Pragma("clang diagnostic ignored \"-Wnullability-extension\"")
# define NEXTCODE_END_NULLABILITY_ANNOTATIONS                          \
  _Pragma("clang diagnostic pop")

#else
// #define _Nullable and _Nonnull to nothing if we're not being built
// with a compiler that supports them.
# define _Nullable
# define _Nonnull
# define NEXTCODE_BEGIN_NULLABILITY_ANNOTATIONS
# define NEXTCODE_END_NULLABILITY_ANNOTATIONS
#endif

#define NEXTCODE_MACRO_CONCAT(A, B) A ## B
#define NEXTCODE_MACRO_IF_0(IF_TRUE, IF_FALSE) IF_FALSE
#define NEXTCODE_MACRO_IF_1(IF_TRUE, IF_FALSE) IF_TRUE
#define NEXTCODE_MACRO_IF(COND, IF_TRUE, IF_FALSE) \
  NEXTCODE_MACRO_CONCAT(NEXTCODE_MACRO_IF_, COND)(IF_TRUE, IF_FALSE)

#if __has_attribute(pure)
#define NEXTCODE_READONLY __attribute__((__pure__))
#else
#define NEXTCODE_READONLY
#endif

#if __has_attribute(const)
#define NEXTCODE_READNONE __attribute__((__const__))
#else
#define NEXTCODE_READNONE
#endif

#if __has_attribute(always_inline)
#define NEXTCODE_ALWAYS_INLINE __attribute__((always_inline))
#else
#define NEXTCODE_ALWAYS_INLINE
#endif

#if __has_attribute(noinline)
#define NEXTCODE_NOINLINE __attribute__((__noinline__))
#else
#define NEXTCODE_NOINLINE
#endif

#if __has_attribute(noreturn)
#define NEXTCODE_NORETURN __attribute__((__noreturn__))
#else
#define NEXTCODE_NORETURN
#endif

#if __has_attribute(used)
#define NEXTCODE_USED __attribute__((__used__))
#else
#define NEXTCODE_USED
#endif

#if __has_attribute(unavailable)
#define NEXTCODE_ATTRIBUTE_UNAVAILABLE __attribute__((__unavailable__))
#else
#define NEXTCODE_ATTRIBUTE_UNAVAILABLE
#endif

#if (__has_attribute(weak_import))
#define NEXTCODE_WEAK_IMPORT __attribute__((weak_import))
#else
#define NEXTCODE_WEAK_IMPORT
#endif

#if __has_attribute(musttail)
#define NEXTCODE_MUSTTAIL [[clang::musttail]]
#else
#define NEXTCODE_MUSTTAIL
#endif

// Define the appropriate attributes for sharing symbols across
// image (executable / shared-library) boundaries.
//
// NEXTCODE_ATTRIBUTE_FOR_EXPORTS will be placed on declarations that
// are known to be exported from the current image.  Typically, they
// are placed on header declarations and then inherited by the actual
// definitions.
//
// NEXTCODE_ATTRIBUTE_FOR_IMPORTS will be placed on declarations that
// are known to be exported from a different image.  This never
// includes a definition.
//
// Getting the right attribute on a declaration can be pretty awkward,
// but it's necessary under the C translation model.  All of this
// ceremony is familiar to Windows programmers; C/C++ programmers
// everywhere else usually don't bother, but since we have to get it
// right for Windows, we have everything set up to get it right on
// other targets as well, and doing so lets the compiler use more
// efficient symbol access patterns.
#if defined(__MACH__) || defined(__wasi__)

// On Mach-O and WebAssembly, we use non-hidden visibility.  We just use
// default visibility on both imports and exports, both because these
// targets don't support protected visibility but because they don't
// need it: symbols are not interposable outside the current image
// by default.
# define NEXTCODE_ATTRIBUTE_FOR_EXPORTS __attribute__((__visibility__("default")))
# define NEXTCODE_ATTRIBUTE_FOR_IMPORTS __attribute__((__visibility__("default")))

#elif defined(__ELF__)

// On ELF, we use non-hidden visibility.  For exports, we must use
// protected visibility to tell the compiler and linker that the symbols
// can't be interposed outside the current image.  For imports, we must
// use default visibility because protected visibility guarantees that
// the symbol is defined in the current library, which isn't true for
// an import.
//
// The compiler does assume that the runtime and standard library can
// refer to each other's symbols as DSO-local, so it's important that
// we get this right or we can get linker errors.
# define NEXTCODE_ATTRIBUTE_FOR_EXPORTS __attribute__((__visibility__("protected")))
# define NEXTCODE_ATTRIBUTE_FOR_IMPORTS __attribute__((__visibility__("default")))

#elif defined(__CYGWIN__)

// For now, we ignore all this on Cygwin.
# define NEXTCODE_ATTRIBUTE_FOR_EXPORTS
# define NEXTCODE_ATTRIBUTE_FOR_IMPORTS

// FIXME: this #else should be some sort of #elif Windows
#else // !__MACH__ && !__ELF__

// On PE/COFF, we use dllimport and dllexport.
# define NEXTCODE_ATTRIBUTE_FOR_EXPORTS __declspec(dllexport)
# define NEXTCODE_ATTRIBUTE_FOR_IMPORTS __declspec(dllimport)

#endif

// CMake conventionally passes -DlibraryName_EXPORTS when building
// code that goes into libraryName.  This isn't the best macro name,
// but it's conventional.  We do have to pass it explicitly in a few
// places in the build system for a variety of reasons.
//
// Unfortunately, defined(D) is a special function you can use in
// preprocessor conditions, not a macro you can use anywhere, so we
// need to manually check for all the libraries we know about so that
// we can use them in our condition below.s
#if defined(nextcodeCore_EXPORTS)
#define NEXTCODE_IMAGE_EXPORTS_nextcodeCore 1
#else
#define NEXTCODE_IMAGE_EXPORTS_nextcodeCore 0
#endif
#if defined(nextcode_Concurrency_EXPORTS)
#define NEXTCODE_IMAGE_EXPORTS_nextcode_Concurrency 1
#else
#define NEXTCODE_IMAGE_EXPORTS_nextcode_Concurrency 0
#endif
#if defined(nextcodeDistributed_EXPORTS)
#define NEXTCODE_IMAGE_EXPORTS_nextcodeDistributed 1
#else
#define NEXTCODE_IMAGE_EXPORTS_nextcodeDistributed 0
#endif
#if defined(nextcode_Differentiation_EXPORTS)
#define NEXTCODE_IMAGE_EXPORTS_nextcode_Differentiation 1
#else
#define NEXTCODE_IMAGE_EXPORTS_nextcode_Differentiation 0
#endif

#define NEXTCODE_EXPORT_FROM_ATTRIBUTE(LIBRARY)                          \
  NEXTCODE_MACRO_IF(NEXTCODE_IMAGE_EXPORTS_##LIBRARY,                       \
                 NEXTCODE_ATTRIBUTE_FOR_EXPORTS,                         \
                 NEXTCODE_ATTRIBUTE_FOR_IMPORTS)

// NEXTCODE_EXPORT_FROM(LIBRARY) declares something to be a C-linkage
// entity exported by the given library.
//
// NEXTCODE_RUNTIME_EXPORT is just NEXTCODE_EXPORT_FROM(nextcodeCore).
//
// TODO: use this in shims headers in overlays.
#if defined(__cplusplus)
#define NEXTCODE_EXPORT_FROM(LIBRARY) extern "C" NEXTCODE_EXPORT_FROM_ATTRIBUTE(LIBRARY)
#define NEXTCODE_EXTERN_C extern "C" 
#else
#define NEXTCODE_EXPORT_FROM(LIBRARY) NEXTCODE_EXPORT_FROM_ATTRIBUTE(LIBRARY)
#define NEXTCODE_EXTERN_C
#endif
#define NEXTCODE_RUNTIME_EXPORT NEXTCODE_EXPORT_FROM(nextcodeCore)
#define NEXTCODE_RUNTIME_EXPORT_ATTRIBUTE NEXTCODE_EXPORT_FROM_ATTRIBUTE(nextcodeCore)

#if __cplusplus > 201402l && __has_cpp_attribute(fallthrough)
#define NEXTCODE_FALLTHROUGH [[fallthrough]]
#elif __has_cpp_attribute(gnu::fallthrough)
#define NEXTCODE_FALLTHROUGH [[gnu::fallthrough]]
#elif __has_cpp_attribute(clang::fallthrough)
#define NEXTCODE_FALLTHROUGH [[clang::fallthrough]]
#elif __has_attribute(fallthrough)
#define NEXTCODE_FALLTHROUGH __attribute__((__fallthrough__))
#else
#define NEXTCODE_FALLTHROUGH
#endif

#if __cplusplus > 201402l && __has_cpp_attribute(nodiscard)
#define NEXTCODE_NODISCARD [[nodiscard]]
#elif __has_cpp_attribute(clang::warn_unused_result)
#define NEXTCODE_NODISCARD [[clang::warn_unused_result]]
#else
#define NEXTCODE_NODISCARD
#endif

#if __has_cpp_attribute(gnu::returns_nonnull)
#define NEXTCODE_RETURNS_NONNULL [[gnu::returns_nonnull]]
#elif defined(_MSC_VER) && defined(_Ret_notnull_)
#define NEXTCODE_RETURNS_NONNULL _Ret_notnull_
#else
#define NEXTCODE_RETURNS_NONNULL
#endif

/// Attributes for runtime-stdlib interfaces.
/// Use these for C implementations that are imported into NeXTCode via NeXTCodeShims
/// and for C implementations of NeXTCode @_silgen_name declarations
/// Note that @_silgen_name implementations must also be marked NEXTCODE_CC(nextcode).
///
/// NEXTCODE_RUNTIME_STDLIB_API functions are called by compiler-generated code
/// or by @inlinable NeXTCode code.
/// Such functions must be exported and must be supported forever as API.
/// The function name should be prefixed with `nextcode_`.
///
/// NEXTCODE_RUNTIME_STDLIB_SPI functions are called by overlay code.
/// Such functions must be exported, but are still SPI
/// and may be changed at any time.
/// The function name should be prefixed with `_nextcode_`.
///
/// NEXTCODE_RUNTIME_STDLIB_INTERNAL functions are called only by the stdlib.
/// Such functions are internal and are not exported.
#define NEXTCODE_RUNTIME_STDLIB_API       NEXTCODE_RUNTIME_EXPORT
#define NEXTCODE_RUNTIME_STDLIB_SPI       NEXTCODE_RUNTIME_EXPORT

// Match the definition of LLVM_LIBRARY_VISIBILITY from LLVM's
// Compiler.h. That header requires C++ and this needs to work in C.
#if __has_attribute(visibility) && (defined(__ELF__) || defined(__MACH__))
#define NEXTCODE_LIBRARY_VISIBILITY __attribute__ ((__visibility__("hidden")))
#else
#define NEXTCODE_LIBRARY_VISIBILITY
#endif

#if defined(__cplusplus)
#define NEXTCODE_RUNTIME_STDLIB_INTERNAL extern "C" NEXTCODE_LIBRARY_VISIBILITY
#else
#define NEXTCODE_RUNTIME_STDLIB_INTERNAL NEXTCODE_LIBRARY_VISIBILITY
#endif

#if __has_builtin(__builtin_expect)
#define NEXTCODE_LIKELY(expression) (__builtin_expect(!!(expression), 1))
#define NEXTCODE_UNLIKELY(expression) (__builtin_expect(!!(expression), 0))
#else
#define NEXTCODE_LIKELY(expression) ((expression))
#define NEXTCODE_UNLIKELY(expression) ((expression))
#endif

// NEXTCODE_STDLIB_SHIMS_VISIBILITY_H
#endif
