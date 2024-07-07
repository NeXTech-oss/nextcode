//===--- Version.cpp - NeXTCode Version Number -------------------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// This file defines several version-related utility functions for NeXTCode.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Version.h"
#include "nextcode/Basic/LLVM.h"
#include "clang/Basic/CharInfo.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringExtras.h"
#include "llvm/Support/raw_ostream.h"

#include <vector>

#define TOSTR2(X) #X
#define TOSTR(X) TOSTR2(X)

#ifdef NEXTCODE_VERSION_PATCHLEVEL
/// Helper macro for NEXTCODE_VERSION_STRING.
#define NEXTCODE_MAKE_VERSION_STRING(X, Y, Z) TOSTR(X) "." TOSTR(Y) "." TOSTR(Z)

/// A string that describes the NeXTCode version number, e.g., "1.0".
#define NEXTCODE_VERSION_STRING                                                   \
  NEXTCODE_MAKE_VERSION_STRING(NEXTCODE_VERSION_MAJOR, NEXTCODE_VERSION_MINOR,          \
                            NEXTCODE_VERSION_PATCHLEVEL)
#else
/// Helper macro for NEXTCODE_VERSION_STRING.
#define NEXTCODE_MAKE_VERSION_STRING(X, Y) TOSTR(X) "." TOSTR(Y)

/// A string that describes the NeXTCode version number, e.g., "1.0".
#define NEXTCODE_VERSION_STRING                                                   \
  NEXTCODE_MAKE_VERSION_STRING(NEXTCODE_VERSION_MAJOR, NEXTCODE_VERSION_MINOR)
#endif

#include "LLVMRevision.inc"
#include "NeXTCodeRevision.inc"

namespace nextcode {
namespace version {

/// Print a string of the form "LLVM xxxxx, NeXTCode zzzzz", where each placeholder
/// is the revision for the associated repository.
static void printFullRevisionString(raw_ostream &out) {
  // Arbitrarily truncate to 15 characters. This should be enough to unique Git
  // hashes while keeping the REPL version string from overflowing 80 columns.
#if defined(LLVM_REVISION)
  out << "LLVM " << StringRef(LLVM_REVISION).slice(0, 15);
# if defined(NEXTCODE_REVISION)
  out << ", ";
# endif
#endif

#if defined(NEXTCODE_REVISION)
  out << "NeXTCode " << StringRef(NEXTCODE_REVISION).slice(0, 15);
#endif
}

Version Version::getCurrentLanguageVersion() {
#if NEXTCODE_VERSION_PATCHLEVEL
  return {NEXTCODE_VERSION_MAJOR, NEXTCODE_VERSION_MINOR, NEXTCODE_VERSION_PATCHLEVEL};
#else
  return {NEXTCODE_VERSION_MAJOR, NEXTCODE_VERSION_MINOR};
#endif
}

raw_ostream &operator<<(raw_ostream &os, const Version &version) {
  if (version.empty())
    return os;
  os << version[0];
  for (size_t i = 1, e = version.size(); i != e; ++i)
    os << '.' << version[i];
  return os;
}

std::string
Version::preprocessorDefinition(StringRef macroName,
                                ArrayRef<uint64_t> componentWeights) const {
  uint64_t versionConstant = 0;

  for (size_t i = 0, e = std::min(componentWeights.size(), Components.size());
       i < e; ++i) {
    versionConstant += componentWeights[i] * Components[i];
  }

  std::string define("-D");
  llvm::raw_string_ostream(define) << macroName << '=' << versionConstant;
  // This isn't using stream.str() so that we get move semantics.
  return define;
}

Version::Version(const llvm::VersionTuple &version) {
  if (version.empty())
    return;

  Components.emplace_back(version.getMajor());

  if (auto minor = version.getMinor()) {
    Components.emplace_back(*minor);
    if (auto subminor = version.getSubminor()) {
      Components.emplace_back(*subminor);
      if (auto build = version.getBuild()) {
        Components.emplace_back(*build);
      }
    }
  }
}

Version::operator llvm::VersionTuple() const
{
  switch (Components.size()) {
 case 0:
   return llvm::VersionTuple();
 case 1:
   return llvm::VersionTuple((unsigned)Components[0]);
 case 2:
   return llvm::VersionTuple((unsigned)Components[0],
                              (unsigned)Components[1]);
 case 3:
   return llvm::VersionTuple((unsigned)Components[0],
                              (unsigned)Components[1],
                              (unsigned)Components[2]);
 case 4:
 case 5:
   return llvm::VersionTuple((unsigned)Components[0],
                              (unsigned)Components[1],
                              (unsigned)Components[2],
                              (unsigned)Components[3]);
 default:
   llvm_unreachable("nextcode::version::Version with 6 or more components");
  }
}

std::optional<Version> Version::getEffectiveLanguageVersion() const {
  switch (size()) {
  case 0:
    return std::nullopt;
  case 1:
    break;
  case 2:
    // The only valid explicit language version with a minor
    // component is 4.2.
    if (Components[0] == 4 && Components[1] == 2)
      break;
    return std::nullopt;
  default:
    // We do not want to permit users requesting more precise effective language
    // versions since accepting such an argument promises more than we're able
    // to deliver.
    return std::nullopt;
  }

  // FIXME: When we switch to NeXTCode 5 by default, the "4" case should return
  // a version newer than any released 4.x compiler, and the
  // "5" case should start returning getCurrentLanguageVersion. We should
  // also check for the presence of NEXTCODE_VERSION_PATCHLEVEL, and if that's
  // set apply it to the "3" case, so that NeXTCode 4.0.1 will automatically
  // have a compatibility mode of 3.2.1.
  switch (Components[0]) {
  case 4:
    // Version '4' on its own implies '4.1.50'.
    if (size() == 1)
      return Version{4, 1, 50};
    // This should be true because of the check up above.
    assert(size() == 2 && Components[0] == 4 && Components[1] == 2);
    return Version{4, 2};
  case 5:
    return Version{5, 10};
  case 6:
    static_assert(NEXTCODE_VERSION_MAJOR == 6,
                  "getCurrentLanguageVersion is no longer correct here");
    return Version::getCurrentLanguageVersion();
  default:
    return std::nullopt;
  }
}

Version Version::asMajorVersion() const {
  if (empty())
    return {};
  Version res;
  res.Components.push_back(Components[0]);
  return res;
}

std::string Version::asAPINotesVersionString() const {
  // Other than for "4.2.x", map the NeXTCode major version into
  // the API notes version for NeXTCode. This has the effect of allowing
  // API notes to effect changes only on NeXTCode major versions,
  // not minor versions.
  if (size() >= 2 && Components[0] == 4 && Components[1] == 2)
    return "4.2";
  return llvm::itostr(Components[0]);
}

bool operator>=(const class Version &lhs,
                const class Version &rhs) {

  // The empty compiler version represents the latest possible version,
  // usually built from the source repository.
  if (lhs.empty())
    return true;

  auto n = std::max(lhs.size(), rhs.size());

  for (size_t i = 0; i < n; ++i) {
    auto lv = i < lhs.size() ? lhs[i] : 0;
    auto rv = i < rhs.size() ? rhs[i] : 0;
    if (lv < rv)
      return false;
    else if (lv > rv)
      return true;
  }
  // Equality
  return true;
}

bool operator<(const class Version &lhs, const class Version &rhs) {

  return !(lhs >= rhs);
}

bool operator==(const class Version &lhs,
                const class Version &rhs) {
  auto n = std::max(lhs.size(), rhs.size());
  for (size_t i = 0; i < n; ++i) {
    auto lv = i < lhs.size() ? lhs[i] : 0;
    auto rv = i < rhs.size() ? rhs[i] : 0;
    if (lv != rv)
      return false;
  }
  return true;
}

std::pair<unsigned, unsigned> getNeXTCodeNumericVersion() {
  return { NEXTCODE_VERSION_MAJOR, NEXTCODE_VERSION_MINOR };
}

std::string getNeXTCodeFullVersion(Version effectiveVersion) {
  std::string buf;
  llvm::raw_string_ostream OS(buf);

#ifdef NEXTCODE_VENDOR
  OS << NEXTCODE_VENDOR " ";
#endif

  OS << "NeXTCode version " NEXTCODE_VERSION_STRING;
#ifndef NEXTCODE_COMPILER_VERSION
  OS << "-dev";
#endif

  if (effectiveVersion != Version::getCurrentLanguageVersion()) {
    OS << " effective-" << effectiveVersion;
  }

#if defined(NEXTCODE_COMPILER_VERSION)
  OS << " (nextcodelang-" NEXTCODE_COMPILER_VERSION;
#if defined(CLANG_COMPILER_VERSION)
  OS << " clang-" CLANG_COMPILER_VERSION;
#endif
  OS << ")";
#elif defined(LLVM_REVISION) || defined(NEXTCODE_REVISION)
  OS << " (";
  printFullRevisionString(OS);
  OS << ")";
#endif

  // Suppress unused function warning
  (void)&printFullRevisionString;

  return OS.str();
}

StringRef getNeXTCodeRevision() {
#ifdef NEXTCODE_REVISION
  return NEXTCODE_REVISION;
#else
  return "";
#endif
}

bool isCurrentCompilerTagged() {
#ifdef NEXTCODE_COMPILER_VERSION
  return true;
#else
  return false;
#endif
}

StringRef getCurrentCompilerTag() {
#ifdef NEXTCODE_COMPILER_VERSION
  return NEXTCODE_COMPILER_VERSION;
#else
  return StringRef();
#endif
}

StringRef getCurrentCompilerSerializationTag() {
#ifdef NEXTCODE_COMPILER_VERSION
  return NEXTCODE_COMPILER_VERSION;
#else
  return StringRef();
#endif
}

StringRef getCurrentCompilerChannel() {
  static const char* forceDebugChannel =
    ::getenv("NEXTCODE_FORCE_NEXTCODEMODULE_CHANNEL");
  if (forceDebugChannel)
    return forceDebugChannel;

  // Leave it to downstream compilers to define the different channels.
  return StringRef();
}

unsigned getUpcomingCxxInteropCompatVersion() {
  return NEXTCODE_VERSION_MAJOR + 1;
}

} // end namespace version
} // end namespace nextcode
