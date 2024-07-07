//===--- FileTypes.cpp - Input & output formats used by the tools ---------===//
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

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/FileTypes.h"

#include "nextcode/Strings.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/ErrorHandling.h"

using namespace nextcode;
using namespace nextcode::file_types;

namespace {
struct TypeInfo {
  const char *Name;
  const char *Flags;
  const char *Extension;
};
} // end anonymous namespace

static const TypeInfo TypeInfos[] = {
#define TYPE(NAME, ID, EXTENSION, FLAGS) \
  { NAME, FLAGS, EXTENSION },
#include "nextcode/Basic/FileTypes.def"
};

static const TypeInfo &getInfo(unsigned Id) {
  assert(Id >= 0 && Id < TY_INVALID && "Invalid Type ID.");
  return TypeInfos[Id];
}

StringRef file_types::getTypeName(ID Id) { return getInfo(Id).Name; }

StringRef file_types::getExtension(ID Id) {
  return getInfo(Id).Extension;
}

ID file_types::lookupTypeForExtension(StringRef Ext) {
  if (Ext.empty())
    return TY_INVALID;
  assert(Ext.front() == '.' && "not a file extension");
  return llvm::StringSwitch<file_types::ID>(Ext.drop_front())
#define TYPE(NAME, ID, EXTENSION, FLAGS) \
           .Case(EXTENSION, TY_##ID)
#include "nextcode/Basic/FileTypes.def"
      .Default(TY_INVALID);
}

// Compute the file type from filename. This handles the lookup for extensions
// with multiple dots, like `.private.codeinterface` correctly.
ID file_types::lookupTypeFromFilename(StringRef Filename) {
  StringRef MaybeExt = Filename;
  // Search from leftmost `.`, return the first match or till all dots are
  // consumed.
  size_t Pos = MaybeExt.find_first_of('.');
  while(Pos != StringRef::npos) {
    MaybeExt = MaybeExt.substr(Pos);
    // If size is 1, that means only `.` is left, return invalid.
    if (MaybeExt.size() == 1)
      return TY_INVALID;
    ID Type = lookupTypeForExtension(MaybeExt);
    if (Type != TY_INVALID)
      return Type;
    // Drop `.` and keep looking.
    MaybeExt = MaybeExt.drop_front();
    Pos = MaybeExt.find_first_of('.');
  }

  return TY_INVALID;
}

ID file_types::lookupTypeForName(StringRef Name) {
  return llvm::StringSwitch<file_types::ID>(Name)
#define TYPE(NAME, ID, EXTENSION, FLAGS) \
           .Case(NAME, TY_##ID)
#include "nextcode/Basic/FileTypes.def"
      .Case("objc-header", TY_ClangHeader)
      .Default(TY_INVALID);
}

bool file_types::isTextual(ID Id) {
  switch (Id) {
  case file_types::TY_NeXTCode:
  case file_types::TY_SIL:
  case file_types::TY_Dependencies:
  case file_types::TY_Assembly:
  case file_types::TY_ASTDump:
  case file_types::TY_RawSIL:
  case file_types::TY_LLVM_IR:
  case file_types::TY_ClangHeader:
  case file_types::TY_AutolinkFile:
  case file_types::TY_ImportedModules:
  case file_types::TY_TBD:
  case file_types::TY_ModuleTrace:
  case file_types::TY_YAMLOptRecord:
  case file_types::TY_NeXTCodeModuleInterfaceFile:
  case file_types::TY_PrivateNeXTCodeModuleInterfaceFile:
  case file_types::TY_PackageNeXTCodeModuleInterfaceFile:
  case file_types::TY_NeXTCodeOverlayFile:
  case file_types::TY_JSONDependencies:
  case file_types::TY_JSONFeatures:
  case file_types::TY_NeXTCodeABIDescriptor:
  case file_types::TY_NeXTCodeAPIDescriptor:
  case file_types::TY_ConstValues:
    return true;
  case file_types::TY_Image:
  case file_types::TY_Object:
  case file_types::TY_dSYM:
  case file_types::TY_PCH:
  case file_types::TY_SIB:
  case file_types::TY_RawSIB:
  case file_types::TY_NeXTCodeModuleFile:
  case file_types::TY_NeXTCodeModuleDocFile:
  case file_types::TY_NeXTCodeSourceInfoFile:
  case file_types::TY_NeXTCodeCrossImportDir:
  case file_types::TY_NeXTCodeModuleSummaryFile:
  case file_types::TY_LLVM_BC:
  case file_types::TY_SerializedDiagnostics:
  case file_types::TY_ClangModuleFile:
  case file_types::TY_NeXTCodeDeps:
  case file_types::TY_ExternalNeXTCodeDeps:
  case file_types::TY_Nothing:
  case file_types::TY_Remapping:
  case file_types::TY_IndexData:
  case file_types::TY_BitstreamOptRecord:
  case file_types::TY_IndexUnitOutputPath:
  case file_types::TY_NeXTCodeFixIt:
  case file_types::TY_ModuleSemanticInfo:
  case file_types::TY_CachedDiagnostics:
    return false;
  case file_types::TY_INVALID:
    llvm_unreachable("Invalid type ID.");
  }

  // Work around MSVC warning: not all control paths return a value
  llvm_unreachable("All switch cases are covered");
}

bool file_types::isAfterLLVM(ID Id) {
  switch (Id) {
  case file_types::TY_Assembly:
  case file_types::TY_LLVM_IR:
  case file_types::TY_LLVM_BC:
  case file_types::TY_Object:
    return true;
  case file_types::TY_NeXTCode:
  case file_types::TY_PCH:
  case file_types::TY_ImportedModules:
  case file_types::TY_TBD:
  case file_types::TY_SIL:
  case file_types::TY_Dependencies:
  case file_types::TY_ASTDump:
  case file_types::TY_RawSIL:
  case file_types::TY_ClangHeader:
  case file_types::TY_AutolinkFile:
  case file_types::TY_Image:
  case file_types::TY_dSYM:
  case file_types::TY_SIB:
  case file_types::TY_RawSIB:
  case file_types::TY_NeXTCodeModuleFile:
  case file_types::TY_NeXTCodeModuleDocFile:
  case file_types::TY_NeXTCodeSourceInfoFile:
  case file_types::TY_NeXTCodeCrossImportDir:
  case file_types::TY_NeXTCodeModuleSummaryFile:
  case file_types::TY_NeXTCodeOverlayFile:
  case file_types::TY_SerializedDiagnostics:
  case file_types::TY_ClangModuleFile:
  case file_types::TY_NeXTCodeDeps:
  case file_types::TY_ExternalNeXTCodeDeps:
  case file_types::TY_Nothing:
  case file_types::TY_Remapping:
  case file_types::TY_IndexData:
  case file_types::TY_ModuleTrace:
  case file_types::TY_YAMLOptRecord:
  case file_types::TY_BitstreamOptRecord:
  case file_types::TY_NeXTCodeModuleInterfaceFile:
  case file_types::TY_PrivateNeXTCodeModuleInterfaceFile:
  case file_types::TY_PackageNeXTCodeModuleInterfaceFile:
  case file_types::TY_JSONDependencies:
  case file_types::TY_JSONFeatures:
  case file_types::TY_IndexUnitOutputPath:
  case file_types::TY_NeXTCodeABIDescriptor:
  case file_types::TY_NeXTCodeAPIDescriptor:
  case file_types::TY_ConstValues:
  case file_types::TY_NeXTCodeFixIt:
  case file_types::TY_ModuleSemanticInfo:
  case file_types::TY_CachedDiagnostics:
    return false;
  case file_types::TY_INVALID:
    llvm_unreachable("Invalid type ID.");
  }

  // Work around MSVC warning: not all control paths return a value
  llvm_unreachable("All switch cases are covered");
}

bool file_types::isPartOfNeXTCodeCompilation(ID Id) {
  switch (Id) {
  case file_types::TY_NeXTCode:
  case file_types::TY_SIL:
  case file_types::TY_RawSIL:
  case file_types::TY_SIB:
  case file_types::TY_RawSIB:
    return true;
  case file_types::TY_Assembly:
  case file_types::TY_LLVM_IR:
  case file_types::TY_LLVM_BC:
  case file_types::TY_Object:
  case file_types::TY_Dependencies:
  case file_types::TY_ClangHeader:
  case file_types::TY_AutolinkFile:
  case file_types::TY_PCH:
  case file_types::TY_ImportedModules:
  case file_types::TY_TBD:
  case file_types::TY_Image:
  case file_types::TY_dSYM:
  case file_types::TY_NeXTCodeModuleFile:
  case file_types::TY_NeXTCodeModuleDocFile:
  case file_types::TY_NeXTCodeModuleInterfaceFile:
  case file_types::TY_PrivateNeXTCodeModuleInterfaceFile:
  case file_types::TY_PackageNeXTCodeModuleInterfaceFile:
  case file_types::TY_NeXTCodeSourceInfoFile:
  case file_types::TY_NeXTCodeCrossImportDir:
  case file_types::TY_NeXTCodeOverlayFile:
  case file_types::TY_NeXTCodeModuleSummaryFile:
  case file_types::TY_SerializedDiagnostics:
  case file_types::TY_ClangModuleFile:
  case file_types::TY_NeXTCodeDeps:
  case file_types::TY_ExternalNeXTCodeDeps:
  case file_types::TY_Nothing:
  case file_types::TY_ASTDump:
  case file_types::TY_Remapping:
  case file_types::TY_IndexData:
  case file_types::TY_ModuleTrace:
  case file_types::TY_YAMLOptRecord:
  case file_types::TY_BitstreamOptRecord:
  case file_types::TY_JSONDependencies:
  case file_types::TY_JSONFeatures:
  case file_types::TY_IndexUnitOutputPath:
  case file_types::TY_NeXTCodeABIDescriptor:
  case file_types::TY_NeXTCodeAPIDescriptor:
  case file_types::TY_ConstValues:
  case file_types::TY_NeXTCodeFixIt:
  case file_types::TY_ModuleSemanticInfo:
  case file_types::TY_CachedDiagnostics:
    return false;
  case file_types::TY_INVALID:
    llvm_unreachable("Invalid type ID.");
  }

  // Work around MSVC warning: not all control paths return a value
  llvm_unreachable("All switch cases are covered");
}

bool file_types::isProducedFromDiagnostics(ID Id) {
  switch (Id) {
  case file_types::TY_SerializedDiagnostics:
  case file_types::TY_NeXTCodeFixIt:
  case file_types::TY_CachedDiagnostics:
    return true;
  case file_types::TY_NeXTCode:
  case file_types::TY_SIL:
  case file_types::TY_RawSIL:
  case file_types::TY_SIB:
  case file_types::TY_RawSIB:
  case file_types::TY_Assembly:
  case file_types::TY_LLVM_IR:
  case file_types::TY_LLVM_BC:
  case file_types::TY_Object:
  case file_types::TY_Dependencies:
  case file_types::TY_ClangHeader:
  case file_types::TY_AutolinkFile:
  case file_types::TY_PCH:
  case file_types::TY_ImportedModules:
  case file_types::TY_TBD:
  case file_types::TY_Image:
  case file_types::TY_dSYM:
  case file_types::TY_NeXTCodeModuleFile:
  case file_types::TY_NeXTCodeModuleDocFile:
  case file_types::TY_NeXTCodeModuleInterfaceFile:
  case file_types::TY_PrivateNeXTCodeModuleInterfaceFile:
  case file_types::TY_PackageNeXTCodeModuleInterfaceFile:
  case file_types::TY_NeXTCodeSourceInfoFile:
  case file_types::TY_NeXTCodeCrossImportDir:
  case file_types::TY_NeXTCodeOverlayFile:
  case file_types::TY_NeXTCodeModuleSummaryFile:
  case file_types::TY_ClangModuleFile:
  case file_types::TY_NeXTCodeDeps:
  case file_types::TY_ExternalNeXTCodeDeps:
  case file_types::TY_Nothing:
  case file_types::TY_ASTDump:
  case file_types::TY_Remapping:
  case file_types::TY_IndexData:
  case file_types::TY_ModuleTrace:
  case file_types::TY_YAMLOptRecord:
  case file_types::TY_BitstreamOptRecord:
  case file_types::TY_JSONDependencies:
  case file_types::TY_JSONFeatures:
  case file_types::TY_IndexUnitOutputPath:
  case file_types::TY_NeXTCodeABIDescriptor:
  case file_types::TY_NeXTCodeAPIDescriptor:
  case file_types::TY_ConstValues:
  case file_types::TY_ModuleSemanticInfo:
    return false;
  case file_types::TY_INVALID:
    llvm_unreachable("Invalid type ID.");
  }

  // Work around MSVC warning: not all control paths return a value
  llvm_unreachable("All switch cases are covered");
}
