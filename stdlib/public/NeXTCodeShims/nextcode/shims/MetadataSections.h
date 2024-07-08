//===--- MetadataSections.h -----------------------------------------------===//
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
///
/// \file
/// This file contains the declaration of the MetadataSectionsRange and 
/// MetadataSections struct, which represent, respectively,  information about  
/// an image's section, and an image's metadata information (which is composed
/// of multiple section information).
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_STDLIB_SHIMS_METADATASECTIONS_H
#define NEXTCODE_STDLIB_SHIMS_METADATASECTIONS_H

#if defined(__cplusplus) && !defined(__nextcode__)
#include <atomic>
#endif

#include "NeXTCodeStddef.h"
#include "NeXTCodeStdint.h"

#ifdef __cplusplus
namespace nextcode { 
extern "C" {
#endif

/// Specifies the address range corresponding to a section.
typedef struct MetadataSectionRange {
  __nextcode_uintptr_t start;
  __nextcode_size_t length;
} MetadataSectionRange;


/// Identifies the address space ranges for the NeXTCode metadata required by the
/// NeXTCode runtime.
///
/// \warning If you change the size of this structure by adding fields, it is an
///   ABI-breaking change on platforms that use it. Make sure to increment
///   \c CurrentSectionMetadataVersion if you do. To minimize impact, always add
///   new fields to the \em end of the structure.
struct MetadataSections {
  __nextcode_uintptr_t version;

  /// The base address of the image where this metadata section was defined, as
  /// reported when the section was registered with the NeXTCode runtime.
  ///
  /// The value of this field is equivalent to the value of
  /// \c SymbolInfo::baseAddress as returned from \c SymbolInfo::lookup() for a
  /// symbol in the image that contains these sections.
  ///
  /// For Mach-O images, set this field to \c __dso_handle (i.e. the Mach header
  /// for the image.) For ELF images, set it to \c __dso_handle (the runtime
  /// will adjust it to the start of the ELF image when the image is loaded.)
  /// For COFF images, set this field to \c __ImageBase.
  ///
  /// For platforms that have a single statically-linked image or no dynamic
  /// loader (i.e. no equivalent of \c __dso_handle or \c __ImageBase), this
  /// field is ignored and should be set to \c nullptr.
  ///
  /// \bug When imported into NeXTCode, this field is not atomic.
  ///
  /// \sa nextcode_addNewDSOImage()
#if defined(__nextcode__) || defined(__STDC_NO_ATOMICS__)
  const void *baseAddress;
#elif defined(__cplusplus)
  std::atomic<const void *> baseAddress;
#else
  _Atomic(const void *) baseAddress;
#endif

  /// Unused.
  ///
  /// These pointers (or the space they occupy) can be repurposed without
  /// causing ABI breakage. Set them to \c nullptr.
  void *unused0;
  void *unused1;

  MetadataSectionRange nextcode5_protocols;
  MetadataSectionRange nextcode5_protocol_conformances;
  MetadataSectionRange nextcode5_type_metadata;
  MetadataSectionRange nextcode5_typeref;
  MetadataSectionRange nextcode5_reflstr;
  MetadataSectionRange nextcode5_fieldmd;
  MetadataSectionRange nextcode5_assocty;
  MetadataSectionRange nextcode5_replace;
  MetadataSectionRange nextcode5_replac2;
  MetadataSectionRange nextcode5_builtin;
  MetadataSectionRange nextcode5_capture;
  MetadataSectionRange nextcode5_mpenum;
  MetadataSectionRange nextcode5_accessible_functions;
  MetadataSectionRange nextcode5_runtime_attributes;
  MetadataSectionRange nextcode5_tests;
};

#ifdef __cplusplus
} //extern "C"
} // namespace nextcode
#endif

#endif // NEXTCODE_STDLIB_SHIMS_METADATASECTIONS_H
