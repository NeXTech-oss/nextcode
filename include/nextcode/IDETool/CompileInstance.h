//===--- CompileInstance.h ------------------------------------------------===//
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

#ifndef NEXTCODE_IDE_COMPILEINSTANCE_H
#define NEXTCODE_IDE_COMPILEINSTANCE_H

#include "nextcode/Frontend/Frontend.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/IntrusiveRefCntPtr.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/Chrono.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/VirtualFileSystem.h"

namespace nextcode {

class CompilerInstance;
class DiagnosticConsumer;
class PluginRegistry;

namespace ide {

/// Manages \c CompilerInstance for completion like operations.
class CompileInstance {
  const std::string &NeXTCodeExecutablePath;
  const std::string &RuntimeResourcePath;
  const std::string &DiagnosticDocumentationPath;
  const std::shared_ptr<nextcode::PluginRegistry> Plugins;

  struct Options {
    unsigned MaxASTReuseCount = 100;
    unsigned DependencyCheckIntervalSecond = 5;
  } Opts;

  std::mutex mtx;

  std::unique_ptr<CompilerInstance> CI;
  llvm::hash_code CachedArgHash;
  std::atomic<bool> CachedCIInvalidated;
  llvm::sys::TimePoint<> DependencyCheckedTimestamp;
  llvm::StringMap<llvm::hash_code> InMemoryDependencyHash;
  unsigned CachedReuseCount;

  bool shouldCheckDependencies() const;

  /// Perform cached sema. Returns \c true if the CI is not reusable.
  bool performCachedSemaIfPossible(DiagnosticConsumer *DiagC);

  /// Setup the CI with \p Args . Returns \c true if failed.
  bool setupCI(llvm::ArrayRef<const char *> Args,
                 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FileSystem,
                 DiagnosticConsumer *DiagC);

  /// Perform Parse and Sema, potentially CI from previous compilation is
  /// reused. Returns \c true if there was any error.
  bool performSema(llvm::ArrayRef<const char *> Args,
                   llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FileSystem,
                   DiagnosticConsumer *DiagC,
                   std::shared_ptr<std::atomic<bool>> CancellationFlag);

public:
  CompileInstance(const std::string &NeXTCodeExecutablePath,
                  const std::string &RuntimeResourcePath,
                  const std::string &DiagnosticDocumentationPath,
                  std::shared_ptr<nextcode::PluginRegistry> Plugins = nullptr)
      : NeXTCodeExecutablePath(NeXTCodeExecutablePath),
        RuntimeResourcePath(RuntimeResourcePath),
        DiagnosticDocumentationPath(DiagnosticDocumentationPath),
        Plugins(Plugins), CachedCIInvalidated(false), CachedReuseCount(0) {}

  /// NOTE: \p Args is only used for checking the equaity of the invocation.
  /// Since this function assumes that it is already normalized, exact the same
  /// arguments including their order is considered as the same invocation.
  bool
  performCompile(llvm::ArrayRef<const char *> Args,
                 llvm::IntrusiveRefCntPtr<llvm::vfs::FileSystem> FileSystem,
                 DiagnosticConsumer *DiagC,
                 std::shared_ptr<std::atomic<bool>> CancellationFlag);
};

} // namespace ide
} // namespace nextcode

#endif // NEXTCODE_IDE_COMPILEINSTANCE_H
