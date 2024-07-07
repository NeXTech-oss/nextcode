//===--- ExponentialGrowthAppendingBinaryByteStream.h -----------*- C++ -*-===//
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
/// Defines an \c llvm::WritableBinaryStream whose internal buffer grows
/// exponentially in size as data is written to it. It is thus more efficient
/// than llvm::AppendingBinaryByteStream if a lot of small data gets appended to
/// it.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_EXPONENTIALGROWTHAPPENDINGBINARYBYTESTREAM_H
#define NEXTCODE_BASIC_EXPONENTIALGROWTHAPPENDINGBINARYBYTESTREAM_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/BinaryByteStream.h"

namespace nextcode {

/// An implementation of WritableBinaryStream which can write at its end
/// causing the underlying data to grow.  This class owns the underlying data.
class ExponentialGrowthAppendingBinaryByteStream
    : public llvm::WritableBinaryStream {
  /// The buffer holding the data.
  SmallVector<uint8_t, 0> Data;

  /// Data in the stream is always encoded in little-endian byte order.
  const llvm::support::endianness Endian = llvm::support::endianness::little;
public:
  ExponentialGrowthAppendingBinaryByteStream() = default;

  void reserve(size_t Size);

  llvm::support::endianness getEndian() const override { return Endian; }

  llvm::Error readBytes(uint64_t Offset, uint64_t Size,
                        ArrayRef<uint8_t> &Buffer) override;

  llvm::Error readLongestContiguousChunk(uint64_t Offset,
                                         ArrayRef<uint8_t> &Buffer) override;

  MutableArrayRef<uint8_t> data() { return Data; }

  uint64_t getLength() override { return Data.size(); }

  llvm::Error writeBytes(uint64_t Offset, ArrayRef<uint8_t> Buffer) override;

  /// This is an optimized version of \c writeBytes specifically for integers.
  /// Integers are written in little-endian byte order.
  template<typename T>
  llvm::Error writeInteger(uint64_t Offset, T Value) {
    static_assert(std::is_integral<T>::value, "Integer required.");
    if (auto Error = checkOffsetForWrite(Offset, sizeof(T))) {
      return Error;
    }

    // Resize the internal buffer if needed.
    uint64_t RequiredSize = Offset + sizeof(T);
    if (RequiredSize > Data.size()) {
      Data.resize(RequiredSize);
    }

    llvm::support::endian::write<T, llvm::support::unaligned>(
      Data.data() + Offset, Value, Endian);

    return llvm::Error::success();
  }

  llvm::Error commit() override { return llvm::Error::success(); }

  virtual llvm::BinaryStreamFlags getFlags() const override {
    return llvm::BSF_Write | llvm::BSF_Append;
  }
};

} // end namespace nextcode

#endif // NEXTCODE_BASIC_EXPONENTIALGROWTHAPPENDINGBINARYBYTESTREAM_H