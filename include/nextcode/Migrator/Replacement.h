//===--- Replacement.h - Migrator Replacements ------------------*- C++ -*-===//
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

#ifndef NEXTCODE_MIGRATOR_REPLACEMENT_H
#define NEXTCODE_MIGRATOR_REPLACEMENT_H
namespace nextcode {
namespace migrator {

struct Replacement {
  size_t Offset;
  size_t Remove;
  std::string Text;

  bool isRemove() const {
    return Remove > 0;
  }

  bool isInsert() const { return Remove == 0 && !Text.empty(); }

  bool isReplace() const { return Remove > 0 && !Text.empty(); }

  size_t endOffset() const {
    if (isInsert()) {
      return Offset + Text.size();
    } else {
      return Offset + Remove;
    }
  }

  bool operator<(const Replacement &Other) const {
    return Offset < Other.Offset;
  }

  bool operator==(const Replacement &Other) const {
    return Offset == Other.Offset && Remove == Other.Remove &&
      Text == Other.Text;
  }
};

} // end namespace migrator
} // end namespace nextcode

#endif // NEXTCODE_MIGRATOR_REPLACEMENT_H
