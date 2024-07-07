//=== SerializedModuleDependencyCacheFormat.h - serialized format -*- C++-*-=//
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

#ifndef NEXTCODE_DEPENDENCY_SERIALIZEDCACHEFORMAT_H
#define NEXTCODE_DEPENDENCY_SERIALIZEDCACHEFORMAT_H

#include "llvm/Bitcode/BitcodeConvenience.h"
#include "llvm/Bitstream/BitCodes.h"

namespace llvm {
class MemoryBuffer;
namespace vfs{
class OutputBackend;
}
}

namespace nextcode {

class DiagnosticEngine;
class NeXTCodeDependencyScanningService;

namespace dependencies {
namespace module_dependency_cache_serialization {

using llvm::BCArray;
using llvm::BCBlob;
using llvm::BCFixed;
using llvm::BCRecordLayout;
using llvm::BCVBR;

/// Every .moddepcache file begins with these 4 bytes, for easy identification.
const unsigned char MODULE_DEPENDENCY_CACHE_FORMAT_SIGNATURE[] = {'I', 'M', 'D','C'};
const unsigned MODULE_DEPENDENCY_CACHE_FORMAT_VERSION_MAJOR =
    7; // isSystem
/// Increment this on every change.
const unsigned MODULE_DEPENDENCY_CACHE_FORMAT_VERSION_MINOR = 1;

/// Various identifiers in this format will rely on having their strings mapped
/// using this ID.
using IdentifierIDField = BCVBR<13>;
using FileIDField = IdentifierIDField;
using ModuleIDField = IdentifierIDField;
using ContextHashIDField = IdentifierIDField;

/// A bit that indicates whether or not a module is a framework
using IsFrameworkField = BCFixed<1>;
/// A bit that indicates whether or not a module is a system module
using IsSystemField = BCFixed<1>;
/// A bit that indicates whether or not a module is that of a static archive
using IsStaticField = BCFixed<1>;

/// Arrays of various identifiers, distinguished for readability
using IdentifierIDArryField = llvm::BCArray<IdentifierIDField>;
using ModuleIDArryField = llvm::BCArray<IdentifierIDField>;

/// Identifiers used to refer to the above arrays
using FileIDArrayIDField = IdentifierIDField;
using ContextHashIDField = IdentifierIDField;
using ImportArrayIDField = IdentifierIDField;
using FlagIDArrayIDField = IdentifierIDField;
using DependencyIDArrayIDField = IdentifierIDField;

/// The ID of the top-level block containing the dependency graph
const unsigned GRAPH_BLOCK_ID = llvm::bitc::FIRST_APPLICATION_BLOCKID;

/// The .moddepcache file format consists of a METADATA record, followed by
/// zero or more IDENTIFIER records that contain various strings seen in the graph
/// (e.g. file names or compiler flags), followed by zero or more IDENTIFIER_ARRAY records
/// which are arrays of identifiers seen in the graph (e.g. list of source files or list of compile flags),
/// followed by zero or more MODULE_NODE, *_DETAILS_NODE pairs of records.
namespace graph_block {
enum {
  METADATA = 1,
  MODULE_NODE,
  NEXTCODE_INTERFACE_MODULE_DETAILS_NODE,
  NEXTCODE_SOURCE_MODULE_DETAILS_NODE,
  NEXTCODE_PLACEHOLDER_MODULE_DETAILS_NODE,
  NEXTCODE_BINARY_MODULE_DETAILS_NODE,
  CLANG_MODULE_DETAILS_NODE,
  IDENTIFIER_NODE,
  IDENTIFIER_ARRAY_NODE
};

// Always the first record in the file.
using MetadataLayout = BCRecordLayout<
    METADATA,    // ID
    BCFixed<16>, // Inter-Module Dependency graph format major version
    BCFixed<16>, // Inter-Module Dependency graph format minor version
    BCBlob       // Compiler version string
    >;

// After the metadata record, we have zero or more identifier records,
// for each unique string that is referenced in the graph.
//
// Identifiers are referenced by their sequence number, starting from 1.
// The identifier value 0 is special; it always represents the empty string.
// There is no IDENTIFIER_NODE serialized that corresponds to it, instead
// the first IDENTIFIER_NODE always has a sequence number of 1.
using IdentifierNodeLayout = BCRecordLayout<IDENTIFIER_NODE, BCBlob>;

// After the identifier records we have zero or more identifier array records.
//
// These arrays are also referenced by their sequence number,
// starting from 1, similar to identifiers above. Value 0 indicates an
// empty array. This record is used because individual array fields must
// appear as the last field of whatever record they belong to, and several of
// the below record layouts contain multiple arrays.
using IdentifierArrayLayout =
    BCRecordLayout<IDENTIFIER_ARRAY_NODE, IdentifierIDArryField>;

// After the array records, we have a sequence of Module info
// records, each of which is followed by one of:
// - NeXTCodeInterfaceModuleDetails
// - NeXTCodeSourceModuleDetails
// - NeXTCodeBinaryModuleDetails
// - NeXTCodePlaceholderModuleDetails
// - ClangModuleDetails
using ModuleInfoLayout =
    BCRecordLayout<MODULE_NODE,                  // ID
                   IdentifierIDField,            // moduleName
                   ContextHashIDField,           // contextHash
                   ImportArrayIDField,           // moduleImports
                   ImportArrayIDField,           // optionalModuleImports
                   DependencyIDArrayIDField      // resolvedDirectModuleDependencies
                   >;

using NeXTCodeInterfaceModuleDetailsLayout =
    BCRecordLayout<NEXTCODE_INTERFACE_MODULE_DETAILS_NODE, // ID
                   FileIDField,                         // outputFilePath
                   FileIDField,                         // nextcodeInterfaceFile
                   FileIDArrayIDField,                  // compiledModuleCandidates
                   FlagIDArrayIDField,                  // buildCommandLine
                   FlagIDArrayIDField,                  // extraPCMArgs
                   ContextHashIDField,                  // contextHash
                   IsFrameworkField,                    // isFramework
                   IsStaticField,                       // isStatic
                   FileIDField,                         // bridgingHeaderFile
                   FileIDArrayIDField,                  // sourceFiles
                   FileIDArrayIDField,                  // bridgingSourceFiles
                   IdentifierIDField,                   // bridgingModuleDependencies
                   DependencyIDArrayIDField,            // nextcodeOverlayDependencies
                   IdentifierIDField,                   // CASFileSystemRootID
                   IdentifierIDField,                   // bridgingHeaderIncludeTree
                   IdentifierIDField                    // moduleCacheKey
                   >;

using NeXTCodeSourceModuleDetailsLayout =
    BCRecordLayout<NEXTCODE_SOURCE_MODULE_DETAILS_NODE, // ID
                   FlagIDArrayIDField,               // extraPCMArgs
                   FileIDField,                      // bridgingHeaderFile
                   FileIDArrayIDField,               // sourceFiles
                   FileIDArrayIDField,               // bridgingSourceFiles
                   FileIDArrayIDField,               // bridgingModuleDependencies
                   DependencyIDArrayIDField,         // nextcodeOverlayDependencies
                   IdentifierIDField,                // CASFileSystemRootID
                   IdentifierIDField,                // bridgingHeaderIncludeTree
                   FlagIDArrayIDField,               // buildCommandLine
                   FlagIDArrayIDField                // bridgingHeaderBuildCommandLine
                   >;

using NeXTCodeBinaryModuleDetailsLayout =
    BCRecordLayout<NEXTCODE_BINARY_MODULE_DETAILS_NODE, // ID
                   FileIDField,                      // compiledModulePath
                   FileIDField,                      // moduleDocPath
                   FileIDField,                      // moduleSourceInfoPath
                   DependencyIDArrayIDField,         // nextcodeOverlayDependencies
                   FileIDField,                      // headerImport
                   IdentifierIDField,                // headerModuleDependencies
                   FileIDArrayIDField,               // headerSourceFiles
                   IsFrameworkField,                 // isFramework
                   IsStaticField,                    // isStatic
                   IdentifierIDField                 // moduleCacheKey
                   >;

using NeXTCodePlaceholderModuleDetailsLayout =
    BCRecordLayout<NEXTCODE_PLACEHOLDER_MODULE_DETAILS_NODE, // ID
                   FileIDField,                           // compiledModulePath
                   FileIDField,                           // moduleDocPath
                   FileIDField                            // moduleSourceInfoPath
                   >;

using ClangModuleDetailsLayout =
    BCRecordLayout<CLANG_MODULE_DETAILS_NODE, // ID
                   FileIDField,               // pcmOutputPath
                   FileIDField,               // mappedPCMPath
                   FileIDField,               // moduleMapPath
                   ContextHashIDField,        // contextHash
                   FlagIDArrayIDField,        // commandLine
                   FileIDArrayIDField,        // fileDependencies
                   FlagIDArrayIDField,        // capturedPCMArgs
                   IdentifierIDField,         // CASFileSystemRootID
                   IdentifierIDField,         // clangIncludeTreeRoot
                   IdentifierIDField,         // moduleCacheKey
                   IsSystemField              // isSystem
                   >;
} // namespace graph_block

/// Tries to read the dependency graph from the given buffer.
/// Returns \c true if there was an error.
bool readInterModuleDependenciesCache(llvm::MemoryBuffer &buffer,
                                      NeXTCodeDependencyScanningService &cache);

/// Tries to read the dependency graph from the given path name.
/// Returns true if there was an error.
bool readInterModuleDependenciesCache(llvm::StringRef path,
                                      NeXTCodeDependencyScanningService &cache);

/// Tries to write the dependency graph to the given path name.
/// Returns true if there was an error.
bool writeInterModuleDependenciesCache(DiagnosticEngine &diags,
                                       llvm::vfs::OutputBackend &backend,
                                       llvm::StringRef path,
                                       const NeXTCodeDependencyScanningService &cache);

/// Tries to write out the given dependency cache with the given
/// bitstream writer.
void writeInterModuleDependenciesCache(llvm::BitstreamWriter &Out,
                                       const NeXTCodeDependencyScanningService &cache);

} // end namespace module_dependency_cache_serialization
} // end namespace dependencies
} // end namespace nextcode

#endif
