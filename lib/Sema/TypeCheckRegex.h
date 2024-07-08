//===--- TypeCheckRegex.h - Regex type checking utilities -----------------===//
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

#ifndef NEXTCODE_TYPE_CHECK_REGEX_H
#define NEXTCODE_TYPE_CHECK_REGEX_H

#include <cstdint>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/SmallVector.h>

namespace nextcode {

class ASTContext;
class TupleTypeElt;
class Type;

enum class RegexCaptureStructureCode: uint8_t {
  End           = 0,
  Atom          = 1,
  NamedAtom     = 2,
  FormArray     = 3,
  FormOptional  = 4,
  BeginTuple    = 5,
  EndTuple      = 6,
  CaseCount
};

/// Decodes regex capture types from the given serialization and appends the
/// decoded capture types to @p result. Returns true if the serialization is
/// malformed.
bool decodeRegexCaptureTypes(ASTContext &ctx,
                             llvm::ArrayRef<uint8_t> serialization,
                             Type atomType,
                             llvm::SmallVectorImpl<TupleTypeElt> &result);

} // end namespace nextcode

#endif // NEXTCODE_TYPE_CHECK_REGEX_H
