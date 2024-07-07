//===--- MD5Stream.h - raw_ostream that compute MD5  ------------*- C++ -*-===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_MD5STREAM_H
#define NEXTCODE_MD5STREAM_H

#include "llvm/Support/MD5.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {

/// An output stream which calculates the MD5 hash of the streamed data.
class MD5Stream : public llvm::raw_ostream {
private:

  uint64_t Pos = 0;
  llvm::MD5 Hash;

  void write_impl(const char *Ptr, size_t Size) override {
    Hash.update(ArrayRef<uint8_t>(reinterpret_cast<const uint8_t *>(Ptr), Size));
    Pos += Size;
  }

  uint64_t current_pos() const override { return Pos; }

public:

  void final(llvm::MD5::MD5Result &Result) {
    flush();
    Hash.final(Result);
  }
};

} // namespace nextcode

#endif
