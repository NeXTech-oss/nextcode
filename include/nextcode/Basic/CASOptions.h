//===--- CASOptions.h - CAS & caching options -------------------*- C++ -*-===//
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
//  This file defines the CASOptions class, which provides various
//  CAS and caching flags.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_CASOPTIONS_H
#define NEXTCODE_BASIC_CASOPTIONS_H

#include "clang/CAS/CASOptions.h"
#include "llvm/ADT/Hashing.h"

namespace nextcode {

class CASOptions final {
public:
  /// Enable compiler caching.
  bool EnableCaching = false;

  /// Enable compiler caching remarks.
  bool EnableCachingRemarks = false;

  /// Skip replaying outputs from cache.
  bool CacheSkipReplay = false;

  /// CASOptions
  clang::CASOptions CASOpts;

  /// CASFS Root.
  std::vector<std::string> CASFSRootIDs;

  /// Clang Include Trees.
  std::vector<std::string> ClangIncludeTrees;

  /// CacheKey for input file.
  std::string InputFileKey;

  /// Cache key for imported bridging header.
  std::string BridgingHeaderPCHCacheKey;

  /// Has immutable file system input.
  bool HasImmutableFileSystem = false;

  /// Get the CAS configuration flags.
  void enumerateCASConfigurationFlags(
      llvm::function_ref<void(llvm::StringRef)> Callback) const;

  /// Check to see if a CASFileSystem is required.
  bool requireCASFS() const {
    return EnableCaching &&
           (!CASFSRootIDs.empty() || !ClangIncludeTrees.empty() ||
            !InputFileKey.empty() || !BridgingHeaderPCHCacheKey.empty());
  }

  /// Return a hash code of any components from these options that should
  /// contribute to a NeXTCode Bridging PCH hash.
  llvm::hash_code getPCHHashComponents() const {
    // The CASIDs are generated from scanner, thus not part of the hash since
    // they will always be empty when requested.
    // TODO: Add frozen clang::CASOptions to the hash.
    return llvm::hash_combine(EnableCaching);
  }

  /// Return a hash code of any components from these options that should
  /// contribute to a NeXTCode Dependency Scanning hash.
  llvm::hash_code getModuleScanningHashComponents() const {
    return getPCHHashComponents();
  }
};

} // namespace nextcode

#endif // NEXTCODE_BASIC_CASOPTIONS_H
