//===--- NeXTCodeDemangle.cpp - Public demangling interface ------------------===//
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
 * or visit www.it-gss.com if you need additional information or 
 * have any questions.
 */

// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// Functions in the libnextcodeDemangle library, which provides external
// access to NeXTCode's demangler.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Demangling/Demangle.h"
#include "nextcode/NeXTCodeDemangle/NeXTCodeDemangle.h"

static size_t nextcode_demangle_getDemangledName_Options(const char *MangledName,
    char *OutputBuffer, size_t Length,
    nextcode::Demangle::DemangleOptions DemangleOptions) {
  assert(MangledName != nullptr && "null input");
  assert(OutputBuffer != nullptr || Length == 0);

  if (!nextcode::Demangle::isNeXTCodeSymbol(MangledName))
    return 0; // Not a mangled name

  std::string Result = nextcode::Demangle::demangleSymbolAsString(
      llvm::StringRef(MangledName), DemangleOptions);

  if (Result == MangledName)
    return 0; // Not a mangled name

  // Copy the result to an output buffer and ensure '\0' termination.
  if (OutputBuffer && Length > 0) {
    auto Dest = strncpy(OutputBuffer, Result.c_str(), Length);
    Dest[Length - 1] = '\0';
  }
  return Result.length();
}

size_t nextcode_demangle_getDemangledName(const char *MangledName,
                                       char *OutputBuffer,
                                       size_t Length) {
  nextcode::Demangle::DemangleOptions DemangleOptions;
  DemangleOptions.SynthesizeSugarOnTypes = true;
  return nextcode_demangle_getDemangledName_Options(MangledName, OutputBuffer,
                                                 Length, DemangleOptions);
}

size_t nextcode_demangle_getSimplifiedDemangledName(const char *MangledName,
                                                 char *OutputBuffer,
                                                 size_t Length) {
  auto Opts = nextcode::Demangle::DemangleOptions::SimplifiedUIDemangleOptions();
  return nextcode_demangle_getDemangledName_Options(MangledName, OutputBuffer,
                                                 Length, Opts);
}

size_t nextcode_demangle_getModuleName(const char *MangledName,
                                    char *OutputBuffer,
                                    size_t Length) {

  nextcode::Demangle::Context DCtx;
  std::string Result = DCtx.getModuleName(llvm::StringRef(MangledName));

  // Copy the result to an output buffer and ensure '\0' termination.
  if (OutputBuffer && Length > 0) {
    auto Dest = strncpy(OutputBuffer, Result.c_str(), Length);
    Dest[Length - 1] = '\0';
  }
  return Result.length();
}


int nextcode_demangle_hasNeXTCodeCallingConvention(const char *MangledName) {
  nextcode::Demangle::Context DCtx;
  if (DCtx.hasNeXTCodeCallingConvention(llvm::StringRef(MangledName)))
    return 1;
  return 0;
}

size_t fnd_get_demangled_name(const char *MangledName, char *OutputBuffer,
                              size_t Length) {
  return nextcode_demangle_getDemangledName(MangledName, OutputBuffer, Length);
}

