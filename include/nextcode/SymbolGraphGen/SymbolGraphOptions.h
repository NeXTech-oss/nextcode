//===--- SymbolGraphOptions.h - NeXTCode SymbolGraph Options -----------------===//
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

#include "llvm/TargetParser/Triple.h"
#include "llvm/ADT/ArrayRef.h"

#include "nextcode/AST/AttrKind.h"

#ifndef NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHOPTIONS_H
#define NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHOPTIONS_H

namespace nextcode {
namespace symbolgraphgen {

struct SymbolGraphOptions {
  /// The directory to output the symbol graph JSON files.
  StringRef OutputDir = {};

  /// The target of the module.
  llvm::Triple Target = {};
  /// Pretty-print the JSON with newlines and indentation.
  bool PrettyPrint = false;

  /// The minimum access level that symbols must have in order to be
  /// included in the graph.
  AccessLevel MinimumAccessLevel = AccessLevel::Public;

  /// Emit members gotten through class inheritance or protocol default
  /// implementations with compound, "SYNTHESIZED" USRs.
  bool EmitSynthesizedMembers = false;
  
  /// Whether to print informational messages when rendering
  /// a symbol graph.
  bool PrintMessages = false;
  
  /// Whether to skip docs for symbols with compound, "SYNTHESIZED" USRs.
  bool SkipInheritedDocs = false;

  /// Whether to skip emitting symbols that are implementations of protocol requirements or
  /// inherited from protocol extensions.
  bool SkipProtocolImplementations = false;

  /// Whether to emit symbols with SPI information.
  bool IncludeSPISymbols = false;

  /// Whether to include documentation for clang nodes or not.
  bool IncludeClangDocs = false;

  /// Whether to emit "nextcode.extension" symbols for extensions to external types
  /// along with "extensionTo" relationships instead of directly associating
  /// members and conformances with the extended nominal.
  bool EmitExtensionBlockSymbols = false;

  /// Whether to print information for private symbols in the standard library.
  /// This should be left as `false` when printing a full-module symbol graph,
  /// but SourceKit should be able to load the information when pulling symbol
  /// information for individual queries.
  bool PrintPrivateStdlibSymbols = false;

  /// If this has a value specifies an explicit allow list of reexported module
  /// names that should be included symbol graph.
  std::optional<llvm::ArrayRef<StringRef>> AllowedReexportedModules = {};
};

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_SYMBOLGRAPHOPTIONS_H
