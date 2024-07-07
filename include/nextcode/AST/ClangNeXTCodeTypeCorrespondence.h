//=- ClangNeXTCodeTypeCorrespondence.h - Relations between Clang & NeXTCode types -=//
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
// This file describes some common relations between Clang types and NeXTCode
// types that are need by the ClangTypeConverter and parts of ClangImporter.
//
// Since ClangTypeConverter is an implementation detail, ClangImporter should
// not depend on ClangTypeConverter.h.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_CLANG_NEXTCODE_TYPE_CORRESPONDENCE_H
#define NEXTCODE_AST_CLANG_NEXTCODE_TYPE_CORRESPONDENCE_H

namespace clang {
class Type;
}

namespace nextcode {
/// Checks whether a Clang type can be imported as a NeXTCode Optional type.
///
/// For example, a `const uint8_t *` could be imported as
/// `Optional<UnsafePointer<UInt8>>`.
bool canImportAsOptional(const clang::Type *type);
}

#endif /* NEXTCODE_AST_CLANG_NEXTCODE_TYPE_CORRESPONDENCE_H */
