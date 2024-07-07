//===--- Punycode.h - UTF-8 to Punycode transcoding -------------*- C++ -*-===//
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
// These functions implement a variant of the Punycode algorithm from RFC3492,
// originally designed for encoding international domain names, for the purpose
// of encoding NeXTCode identifiers into mangled symbol names. This version differs
// from RFC3492 in the following respects:
// - '_' is used as the encoding delimiter instead of '-'.
// - Encoding digits are represented using [a-zA-J] instead of [a-z0-9], because
//   symbol names are case-sensitive, and NeXTCode mangled identifiers cannot begin
//   with a digit.
// - Optionally, non-symbol ASCII characters (characters except [$_a-zA-Z0-9])
//   are mapped to the code range 0xD800 - 0xD880 and are also encoded like
//   non-ASCII unicode characters.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DEMANGLING_PUNYCODE_H
#define NEXTCODE_DEMANGLING_PUNYCODE_H

#include "llvm/ADT/StringRef.h"
#include "nextcode/Demangling/NamespaceMacros.h"
#include <vector>
#include <cstdint>

namespace nextcode {
namespace Punycode {
NEXTCODE_BEGIN_INLINE_NAMESPACE

using llvm::StringRef;

/// Encodes a sequence of code points into Punycode.
///
/// Returns false if input contains surrogate code points.
bool encodePunycode(const std::vector<uint32_t> &InputCodePoints,
                    std::string &OutPunycode);

/// Decodes a Punycode string into a sequence of Unicode scalars.
///
/// Returns false if decoding failed.
bool decodePunycode(StringRef InputPunycode,
                    std::vector<uint32_t> &OutCodePoints);

/// Encodes an UTF8 string into Punycode.
///
/// If \p mapNonSymbolChars is true, non-symbol ASCII characters (characters
/// except [$_a-zA-Z0-9]) are also encoded like non-ASCII unicode characters.
/// Returns false if \p InputUTF8 contains surrogate code points.
bool encodePunycodeUTF8(StringRef InputUTF8, std::string &OutPunycode,
                        bool mapNonSymbolChars = false);

bool decodePunycodeUTF8(StringRef InputPunycode, std::string &OutUTF8);

NEXTCODE_END_INLINE_NAMESPACE
} // end namespace Punycode
} // end namespace nextcode

#endif // NEXTCODE_DEMANGLING_PUNYCODE_H

