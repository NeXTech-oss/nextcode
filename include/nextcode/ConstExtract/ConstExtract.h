//===---- ConstExtract.h -- Gather Compile-Time-Known Values ----*- C++ -*-===//
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

#ifndef NEXTCODE_CONST_EXTRACT_H
#define NEXTCODE_CONST_EXTRACT_H

#include "nextcode/AST/ConstTypeInfo.h"
#include "llvm/ADT/ArrayRef.h"
#include <string>
#include <unordered_set>
#include <vector>

namespace llvm {
class StringRef;
class raw_fd_ostream;
}

namespace nextcode {
class SourceFile;
class DiagnosticEngine;
class ModuleDecl;
} // namespace nextcode

namespace nextcode {
/// Parse a list of string identifiers from a file at the given path,
/// representing names of protocols.
bool
parseProtocolListFromFile(llvm::StringRef protocolListFilePath,
                          DiagnosticEngine &diags,
                          std::unordered_set<std::string> &protocols);

/// Gather compile-time-known values of properties in nominal type declarations
/// in this file, of types which conform to one of the protocols listed in
/// \c Protocols
std::vector<ConstValueTypeInfo>
gatherConstValuesForPrimary(const std::unordered_set<std::string> &Protocols,
                            const SourceFile *File);

/// Gather compile-time-known values of properties in nominal type declarations
/// in this module, of types which conform to one of the protocols listed in
/// \c Protocols
std::vector<ConstValueTypeInfo>
gatherConstValuesForModule(const std::unordered_set<std::string> &Protocols,
                           ModuleDecl *Module);

/// Serialize a collection of \c ConstValueInfos to JSON at the
/// provided output stream.
bool writeAsJSONToFile(const std::vector<ConstValueTypeInfo> &ConstValueInfos,
                       llvm::raw_ostream &OS);
} // namespace nextcode

#endif
