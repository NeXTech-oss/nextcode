//===------------ BinaryScanningTool.cpp - NeXTCode Compiler ----------------===//
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

#include "nextcode/StaticMirror/BinaryScanningTool.h"
#include "nextcode/Basic/Unreachable.h"
#include "nextcode/Demangling/Demangler.h"
#include "nextcode/RemoteInspection/ReflectionContext.h"
#include "nextcode/RemoteInspection/TypeRefBuilder.h"
#include "nextcode/RemoteInspection/TypeLowering.h"
#include "nextcode/Remote/CMemoryReader.h"
#include "nextcode/StaticMirror/ObjectFileContext.h"

#include "llvm/ADT/StringSet.h"
#include "llvm/Object/Archive.h"
#include "llvm/Object/MachOUniversal.h"

#include <sstream>

using namespace llvm::object;
using namespace nextcode::reflection;

namespace nextcode {
namespace static_mirror {

BinaryScanningTool::BinaryScanningTool(
    const std::vector<std::string> &binaryPaths, const std::string Arch) {
  for (const std::string &BinaryFilename : binaryPaths) {
    auto BinaryOwner = unwrap(createBinary(BinaryFilename));
    Binary *BinaryFile = BinaryOwner.getBinary();

    // The object file we are doing lookups in -- either the binary itself, or
    // a particular slice of a universal binary.
    std::unique_ptr<ObjectFile> ObjectOwner;
    const ObjectFile *O = dyn_cast<ObjectFile>(BinaryFile);
    if (!O) {
      auto Universal = cast<MachOUniversalBinary>(BinaryFile);
      ObjectOwner = unwrap(Universal->getMachOObjectForArch(Arch));
      O = ObjectOwner.get();
    }

    // Retain the objects that own section memory
    BinaryOwners.push_back(std::move(BinaryOwner));
    ObjectOwners.push_back(std::move(ObjectOwner));
    ObjectFiles.push_back(O);
  }
  // FIXME: This could/should be configurable.
#if NEXTCODE_OBJC_INTEROP
  bool ObjCInterop = true;
#else
  bool ObjCInterop = false;
#endif
  Context = makeReflectionContextForObjectFiles(ObjectFiles, ObjCInterop);
  PointerSize = Context->PointerSize;
}

ConformanceCollectionResult
BinaryScanningTool::collectConformances(const std::vector<std::string> &protocolNames) {
  switch (PointerSize) {
    case 4:
      // FIXME: This could/should be configurable.
#if NEXTCODE_OBJC_INTEROP
      return Context->Builder.collectAllConformances<WithObjCInterop, 4>();
#else
      return Context->Builder.collectAllConformances<NoObjCInterop, 4>();
#endif
    case 8:
#if NEXTCODE_OBJC_INTEROP
      return Context->Builder.collectAllConformances<WithObjCInterop, 8>();
#else
      return Context->Builder.collectAllConformances<NoObjCInterop, 8>();
#endif
    default:
      fputs("unsupported word size in object file\n", stderr);
      abort();
  }
}

AssociatedTypeCollectionResult
BinaryScanningTool::collectAssociatedTypes(const std::string &mangledTypeName) {
  switch (PointerSize) {
    case 4:
      // FIXME: This could/should be configurable.
#if NEXTCODE_OBJC_INTEROP
      return Context->Builder.collectAssociatedTypes<WithObjCInterop, 4>(mangledTypeName);
#else
      return Context->Builder.collectAssociatedTypes<NoObjCInterop, 4>(mangledTypeName);
#endif
    case 8:
#if NEXTCODE_OBJC_INTEROP
      return Context->Builder.collectAssociatedTypes<WithObjCInterop, 8>(mangledTypeName);
#else
      return Context->Builder.collectAssociatedTypes<NoObjCInterop, 8>(mangledTypeName);
#endif
    default:
      fputs("unsupported word size in object file\n", stderr);
      abort();
  }
}

AssociatedTypeCollectionResult
BinaryScanningTool::collectAllAssociatedTypes() {
  switch (PointerSize) {
    case 4:
      // FIXME: This could/should be configurable.
#if NEXTCODE_OBJC_INTEROP
      return Context->Builder.collectAssociatedTypes<WithObjCInterop, 4>(
          std::optional<std::string>());
#else
      return Context->Builder.collectAssociatedTypes<NoObjCInterop, 4>(
          std::optional<std::string>());
#endif
    case 8:
#if NEXTCODE_OBJC_INTEROP
      return Context->Builder.collectAssociatedTypes<WithObjCInterop, 8>(
          std::optional<std::string>());
#else
      return Context->Builder.collectAssociatedTypes<NoObjCInterop, 8>(
          std::optional<std::string>());
#endif
    default:
      fputs("unsupported word size in object file\n", stderr);
      abort();
  }
}

FieldTypeCollectionResult
BinaryScanningTool::collectFieldTypes(const std::string &mangledTypeName) {
  return Context->Builder.collectFieldTypes(mangledTypeName);
}

FieldTypeCollectionResult
BinaryScanningTool::collectAllFieldTypes() {
  return Context->Builder.collectFieldTypes(std::optional<std::string>());
}
} // end namespace static_mirror
} // end namespace nextcode
