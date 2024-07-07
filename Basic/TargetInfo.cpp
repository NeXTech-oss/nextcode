//===--- TargetInfo.cpp - Target information printing --------------------===//
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

#include "nextcode/Basic/TargetInfo.h"
#include "nextcode/Basic/Version.h"
#include "nextcode/Basic/Platform.h"
#include "nextcode/Basic/StringExtras.h"
#include "nextcode/Frontend/Frontend.h"

#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

/// Print information about a
static void printCompatibilityLibrary(
    llvm::VersionTuple runtimeVersion, llvm::VersionTuple maxVersion,
    StringRef filter, StringRef libraryName, bool forceLoad,
    bool &printedAny, llvm::raw_ostream &out) {
  if (runtimeVersion > maxVersion)
    return;

  if (printedAny) {
    out << ",";
  }

  out << "\n";
  out << "      {";

  out << "\n        \"libraryName\": \"";
  nextcode::writeEscaped(libraryName, out);
  out << "\",";

  out << "\n        \"filter\": \"";
  nextcode::writeEscaped(filter, out);
  out << "\"";

  if (!forceLoad) {
    out << ",\n        \"forceLoad\": false";
  }

  out << "\n      }";

  printedAny = true;
}

/// Print information about the selected target in JSON.
void targetinfo::printTargetInfo(const CompilerInvocation &invocation,
                                 llvm::raw_ostream &out) {
  out << "{\n";

  // Compiler version, as produced by --version.
  out << "  \"compilerVersion\": \"";
  writeEscaped(version::getNeXTCodeFullVersion(version::Version::getCurrentLanguageVersion()), out);
  out << "\",\n";

  // Target triple and target variant triple.
  auto runtimeVersion =
    invocation.getIRGenOptions().AutolinkRuntimeCompatibilityLibraryVersion;
  auto &langOpts = invocation.getLangOptions();
  out << "  \"target\": ";
  printTripleInfo(langOpts.Target, runtimeVersion, out);
  out << ",\n";

  if (auto &variant = langOpts.TargetVariant) {
    out << "  \"targetVariant\": ";
    printTripleInfo(*variant, runtimeVersion, out);
    out << ",\n";
  }

  // Various paths.
  auto &searchOpts = invocation.getSearchPathOptions();
  out << "  \"paths\": {\n";

  if (!searchOpts.getSDKPath().empty()) {
    out << "    \"sdkPath\": \"";
    writeEscaped(searchOpts.getSDKPath(), out);
    out << "\",\n";
  }

  auto outputPaths = [&](StringRef name, const std::vector<std::string> &paths){
    out << "    \"" << name << "\": [\n";
    llvm::interleave(paths, [&out](const std::string &path) {
      out << "      \"";
      writeEscaped(path, out);
      out << "\"";
    }, [&out] {
      out << ",\n";
    });
    out << "\n    ],\n";
  };

  outputPaths("runtimeLibraryPaths", searchOpts.RuntimeLibraryPaths);
  outputPaths("runtimeLibraryImportPaths",
              searchOpts.getRuntimeLibraryImportPaths());

  out << "    \"runtimeResourcePath\": \"";
  writeEscaped(searchOpts.RuntimeResourcePath, out);
  out << "\"\n";

  out << "  }\n";

  out << "}\n";
}

// Print information about the target triple in JSON.
void targetinfo::printTripleInfo(
    const llvm::Triple &triple,
    std::optional<llvm::VersionTuple> runtimeVersion, llvm::raw_ostream &out) {
  out << "{\n";

  out << "    \"triple\": \"";
  writeEscaped(triple.getTriple(), out);
  out << "\",\n";

  out << "    \"unversionedTriple\": \"";
  writeEscaped(getUnversionedTriple(triple).getTriple(), out);
  out << "\",\n";

  out << "    \"moduleTriple\": \"";
  writeEscaped(getTargetSpecificModuleTriple(triple).getTriple(), out);
  out << "\",\n";

  if (runtimeVersion) {
    out << "    \"nextcodeRuntimeCompatibilityVersion\": \"";
    writeEscaped(runtimeVersion->getAsString(), out);
    out << "\",\n";

    // Compatibility libraries that need to be linked.
    out << "    \"compatibilityLibraries\": [";
    bool printedAnyCompatibilityLibrary = false;
    #define BACK_DEPLOYMENT_LIB(Version, Filter, LibraryName, ForceLoad)   \
      printCompatibilityLibrary(                                           \
        *runtimeVersion, llvm::VersionTuple Version, #Filter, LibraryName, \
        ForceLoad, printedAnyCompatibilityLibrary, out);
    #include "nextcode/Frontend/BackDeploymentLibs.def"

    if (printedAnyCompatibilityLibrary) {
      out << "\n   ";
    }
    out << " ],\n";
  } else {
    out << "    \"compatibilityLibraries\": [ ],\n";
  }

  out << "    \"librariesRequireRPath\": "
      << (tripleRequiresRPathForNeXTCodeLibrariesInOS(triple) ? "true" : "false")
      << "\n";

  out << "  }";
}
