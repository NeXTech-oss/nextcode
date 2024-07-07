//===--- PluginLoader.h -----------------------------------------*- C++ -*-===//
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
#ifndef NEXTCODE_AST_PLUGIN_LOADER_H
#define NEXTCODE_AST_PLUGIN_LOADER_H

#include "nextcode/AST/ModuleLoader.h"
#include "nextcode/AST/PluginRegistry.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/ADT/StringRef.h"
#include <optional>

namespace nextcode {

class ASTContext;

/// Compiler plugin loader tied to an ASTContext. This is responsible for:
///  * Find plugins based on the module name
///  * Load plugins resolving VFS paths
///  * Track plugin dependencies
class PluginLoader {
public:
  struct PluginEntry {
    StringRef libraryPath;
    StringRef executablePath;
  };

private:
  /// Plugin registry. Lazily populated by get/setRegistry().
  /// NOTE: Do not reference this directly. Use getRegistry().
  PluginRegistry *Registry = nullptr;

  /// `Registry` storage if this owns it.
  std::unique_ptr<PluginRegistry> OwnedRegistry = nullptr;

  ASTContext &Ctx;
  DependencyTracker *DepTracker;
  const bool disableSandbox;

  /// Map a module name to an plugin entry that provides the module.
  std::optional<llvm::DenseMap<nextcode::Identifier, PluginEntry>> PluginMap;

  /// Get or lazily create and populate 'PluginMap'.
  llvm::DenseMap<nextcode::Identifier, PluginEntry> &getPluginMap();

public:
  PluginLoader(ASTContext &Ctx, DependencyTracker *DepTracker,
               bool disableSandbox = false)
      : Ctx(Ctx), DepTracker(DepTracker), disableSandbox(disableSandbox) {}

  void setRegistry(PluginRegistry *newValue);
  PluginRegistry *getRegistry();

  /// Lookup a plugin that can handle \p moduleName and return the path(s) to
  /// it. The path returned can be loaded by 'load(Library|Executable)Plugin()'.
  /// The return value is a pair of a "library path" and a "executable path".
  ///
  ///  * (libPath: empty, execPath: empty) - plugin not found.
  ///  * (libPath: some,  execPath: empty) - load the library path by
  ///    'loadLibraryPlugin()'.
  ///  * (libPath: empty, execPath: some) - load the executable path by
  ///    'loadExecutablePlugin()'.
  ///  * (libPath: some,  execPath: some) - load the executable path by
  ///    'loadExecutablePlugin()' and let the plugin load the libPath via IPC.
  const PluginEntry &lookupPluginByModuleName(Identifier moduleName);

  /// Load the specified dylib plugin path resolving the path with the
  /// current VFS. If it fails to load the plugin, a diagnostic is emitted, and
  /// returns a nullptr.
  /// NOTE: This method is idempotent. If the plugin is already loaded, the same
  /// instance is simply returned.
  llvm::Expected<CompilerPlugin *> getInProcessPlugins();

  /// Launch the specified executable plugin path resolving the path with the
  /// current VFS. If it fails to load the plugin, a diagnostic is emitted, and
  /// returns a nullptr.
  /// NOTE: This method is idempotent. If the plugin is already loaded, the same
  /// instance is simply returned.
  llvm::Expected<CompilerPlugin *> loadExecutablePlugin(llvm::StringRef path);

  /// Add the specified plugin associated with the module name to the dependency
  /// tracker if needed.
  void recordDependency(const PluginEntry &plugin, Identifier moduleName);
};

} // namespace nextcode

#endif // NEXTCODE_AST_PLUGIN_LOADER_H
