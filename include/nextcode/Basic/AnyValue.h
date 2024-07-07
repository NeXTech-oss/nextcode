//===--- AnyValue.h ---------------------------------------------*- C++ -*-===//
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
// This file defines some miscellaneous overloads of hash_value() and
// simple_display().
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_ANYVALUE_H
#define NEXTCODE_BASIC_ANYVALUE_H

#include "nextcode/Basic/SimpleDisplay.h"
#include "nextcode/Basic/TypeID.h"
#include "llvm/ADT/PointerUnion.h"  // to define hash_value
#include "llvm/ADT/TinyPtrVector.h"

namespace llvm {
  // FIXME: Belongs in LLVM itself
  template<typename PT1, typename PT2>
  hash_code hash_value(const llvm::PointerUnion<PT1, PT2> &ptr) {
    return hash_value(ptr.getOpaqueValue());
  }

  template<typename T>
  bool operator==(const TinyPtrVector<T> &lhs, const TinyPtrVector<T> &rhs) {
    if (lhs.size() != rhs.size())
      return false;
    
    for (unsigned i = 0, n = lhs.size(); i != n; ++i) {
      if (lhs[i] != rhs[i])
        return false;
    }
    
    return true;
  }
  
  template<typename T>
  bool operator!=(const TinyPtrVector<T> &lhs, const TinyPtrVector<T> &rhs) {
    return !(lhs == rhs);
  }

  template <typename T>
  void simple_display(raw_ostream &out, const std::optional<T> &opt) {
    if (opt) {
      simple_display(out, *opt);
    } else {
      out << "None";
    }
  }
} // end namespace llvm

#endif //


