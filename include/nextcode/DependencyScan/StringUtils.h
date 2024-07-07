//===----- StringUtils.h - Managed C String Utility Functions -----*- C -*-===//
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

#include "nextcode-c/CommonString/CommonString.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSet.h"
#include <string>
#include <vector>

//=== Private Utility Functions--------------------------------------------===//
namespace nextcode {
namespace c_string_utils {

/// Create null string
nextcodescan_string_ref_t create_null();

/// Create a \c nextcodescan_string_ref_t object from a nul-terminated C string.  New
/// \c nextcodescan_string_ref_t will contain a copy of \p string.
nextcodescan_string_ref_t create_clone(const char *string);

/// Create an array of \c nextcodescan_string_ref_t objects from a vector of C++ strings using the
/// create_clone routine.
nextcodescan_string_set_t *create_set(const std::vector<std::string> &strings);

/// Create an array of nextcodescan_string_ref_t objects from an array of C strings using the
/// create_clone routine.
nextcodescan_string_set_t *create_set(int count, const char **strings);

/// Create an empty array of nextcodescan_string_ref_t objects
nextcodescan_string_set_t *create_empty_set();

/// Retrieve the character data associated with the given string.
const char *get_C_string(nextcodescan_string_ref_t string);
}
}
