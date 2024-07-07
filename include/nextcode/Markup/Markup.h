//===--- Markup.h - Markup --------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_MARKUP_MARKUP_H
#define NEXTCODE_MARKUP_MARKUP_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/Allocator.h"
#include "llvm/Support/raw_ostream.h"
#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/Markup/AST.h"
#include "nextcode/Markup/LineList.h"

namespace nextcode {

struct RawComment;

namespace markup {

class LineList;

class MarkupContext final {
  llvm::BumpPtrAllocator Allocator;

public:
  void *allocate(unsigned long Bytes, unsigned Alignment) {
    return Allocator.Allocate(Bytes, Alignment);
  }

  template <typename T, typename It>
  T *allocateCopy(It Begin, It End) {
    T *Res =
    static_cast<T *>(allocate(sizeof(T) * (End - Begin), alignof(T)));
    for (unsigned i = 0; Begin != End; ++Begin, ++i)
      new (Res + i) T(*Begin);
    return Res;
  }

  template <typename T>
  MutableArrayRef<T> allocateCopy(ArrayRef<T> Array) {
    return MutableArrayRef<T>(allocateCopy<T>(Array.begin(), Array.end()),
                              Array.size());
  }

  StringRef allocateCopy(StringRef Str) {
    ArrayRef<char> Result =
        allocateCopy(llvm::ArrayRef(Str.data(), Str.size()));
    return StringRef(Result.data(), Result.size());
  }

  LineList getLineList(nextcode::RawComment RC);
};

Document *parseDocument(MarkupContext &MC, StringRef String);
Document *parseDocument(MarkupContext &MC, LineList &LL);

} // namespace markup
} // namespace nextcode

#endif // NEXTCODE_MARKUP_MARKUP_H
