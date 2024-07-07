//===--- CachingUtils.h -----------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_FRONTEND_CACHINGUTILS_H
#define NEXTCODE_FRONTEND_CACHINGUTILS_H

#include "nextcode/Frontend/CASOutputBackends.h"
#include "nextcode/Frontend/CachedDiagnostics.h"
#include "nextcode/Frontend/FrontendInputsAndOutputs.h"
#include "nextcode/Frontend/FrontendOptions.h"
#include "clang/CAS/CASOptions.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/CAS/ActionCache.h"
#include "llvm/CAS/CASReference.h"
#include "llvm/CAS/ObjectStore.h"
#include "llvm/Support/PrefixMapper.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "llvm/Support/VirtualOutputBackend.h"
#include <memory>

namespace nextcode {

/// Create a nextcode caching output backend that stores the output from
/// compiler into a CAS.
llvm::IntrusiveRefCntPtr<cas::NeXTCodeCASOutputBackend>
createNeXTCodeCachingOutputBackend(
    llvm::cas::ObjectStore &CAS, llvm::cas::ActionCache &Cache,
    llvm::cas::ObjectRef BaseKey,
    const FrontendInputsAndOutputs &InputsAndOutputs,
    FrontendOptions::ActionType Action);

/// Replay the output of the compilation from cache.
/// Return true if outputs are replayed, false otherwise.
bool replayCachedCompilerOutputs(
    llvm::cas::ObjectStore &CAS, llvm::cas::ActionCache &Cache,
    llvm::cas::ObjectRef BaseKey, DiagnosticEngine &Diag,
    const FrontendInputsAndOutputs &InputsAndOutputs,
    CachingDiagnosticsProcessor &CDP, bool CacheRemarks);

/// Load the cached compile result from cache.
std::unique_ptr<llvm::MemoryBuffer> loadCachedCompileResultFromCacheKey(
    llvm::cas::ObjectStore &CAS, llvm::cas::ActionCache &Cache,
    DiagnosticEngine &Diag, llvm::StringRef CacheKey, file_types::ID Type,
    llvm::StringRef Filename = "");

llvm::Expected<llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem>>
createCASFileSystem(llvm::cas::ObjectStore &CAS, ArrayRef<std::string> FSRoots,
                    ArrayRef<std::string> IncludeTreeRoots);

std::vector<std::string> remapPathsFromCommandLine(
    ArrayRef<std::string> Args,
    llvm::function_ref<std::string(StringRef)> RemapCallback);

namespace cas {
class CachedResultLoader {
public:
  CachedResultLoader(llvm::cas::ObjectStore &CAS,
                     llvm::cas::ObjectRef OutputRef)
      : CAS(CAS), OutputRef(OutputRef) {}

  using CallbackTy =
      llvm::function_ref<llvm::Error(file_types::ID, llvm::cas::ObjectRef)>;

  /// Replay the cached result.
  llvm::Error replay(CallbackTy Callback);

private:
  llvm::cas::ObjectStore &CAS;
  llvm::cas::ObjectRef OutputRef;
};
} // end namespace cas
} // end namespace nextcode

#endif
