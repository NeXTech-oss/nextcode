//===-------- ExponentialGrowthAppendingBinaryByteStream.cpp --------------===//
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

#include "nextcode/Basic/ExponentialGrowthAppendingBinaryByteStream.h"

using namespace llvm;
using namespace nextcode;

Error ExponentialGrowthAppendingBinaryByteStream::readBytes(
    uint64_t Offset, uint64_t Size, ArrayRef<uint8_t> &Buffer) {
  if (auto Error = checkOffsetForRead(Offset, Size)) {
    return Error;
  }
  
  Buffer = ArrayRef<uint8_t>(Data.data() + Offset, Size);
  return Error::success();
}

Error ExponentialGrowthAppendingBinaryByteStream::readLongestContiguousChunk(
    uint64_t Offset, ArrayRef<uint8_t> &Buffer) {
  if (auto Error = checkOffsetForRead(Offset, 0)) {
    return Error;
  }

  Buffer = ArrayRef<uint8_t>(Data.data() + Offset, Data.size() - Offset);
  return Error::success();
}

void ExponentialGrowthAppendingBinaryByteStream::reserve(size_t Size) {
  Data.reserve(Size);
}

Error ExponentialGrowthAppendingBinaryByteStream::writeBytes(
    uint64_t Offset, ArrayRef<uint8_t> Buffer) {
  if (Buffer.empty())
    return Error::success();

  if (auto Error = checkOffsetForWrite(Offset, Buffer.size())) {
    return Error;
  }
  
  // Resize the internal buffer if needed.
  uint64_t RequiredSize = Offset + Buffer.size();
  if (RequiredSize > Data.size()) {
    Data.resize(RequiredSize);
  }

  ::memcpy(Data.data() + Offset, Buffer.data(), Buffer.size());

  return Error::success();
}
