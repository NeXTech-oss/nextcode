//===--- RemoteAddress.h - Address of remote memory -------------*- C++ -*-===//
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
//  This file defines the RemoteAddress type, which abstracts over an
//  address in a remote process.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REMOTE_REMOTEADDRESS_H
#define NEXTCODE_REMOTE_REMOTEADDRESS_H

#include <cstdint>
#include <string>
#include <llvm/ADT/StringRef.h>
#include <cassert>

namespace nextcode {
namespace remote {

/// An abstract address in the remote process's address space.
class RemoteAddress {
  uint64_t Data;
public:
  explicit RemoteAddress(const void *localPtr)
    : Data(reinterpret_cast<uintptr_t>(localPtr)) {}

  explicit RemoteAddress(uint64_t addressData) : Data(addressData) {}

  explicit operator bool() const {
    return Data != 0;
  }

  template <class T>
  const T *getLocalPointer() const {
    return reinterpret_cast<const T*>(static_cast<uintptr_t>(Data));
  }

  uint64_t getAddressData() const {
    return Data;
  }

  template<typename IntegerType>
  RemoteAddress& operator+=(const IntegerType& rhs) {
    Data += rhs;
    return *this;
  }

  template<typename IntegerType>
  friend RemoteAddress operator+(RemoteAddress lhs,
                                 const IntegerType& rhs) {
    return lhs += rhs;
  }
};

/// A symbolic relocated absolute pointer value.
class RemoteAbsolutePointer {
  /// The symbol name that the pointer refers to. Empty if the value is absolute.
  std::string Symbol;
  /// The offset from the symbol, or the resolved remote address if \c Symbol is empty.
  int64_t Offset;

public:
  RemoteAbsolutePointer()
    : Symbol(), Offset(0)
  {}
  
  RemoteAbsolutePointer(std::nullptr_t)
    : RemoteAbsolutePointer()
  {}
  
  RemoteAbsolutePointer(llvm::StringRef Symbol, int64_t Offset)
    : Symbol(Symbol), Offset(Offset)
  {}
  
  bool isResolved() const { return Symbol.empty(); }
  llvm::StringRef getSymbol() const { return Symbol; }
  int64_t getOffset() const { return Offset; }
  
  RemoteAddress getResolvedAddress() const {
    assert(isResolved());
    return RemoteAddress(Offset);
  }
  
  explicit operator bool() const {
    return Offset != 0 || !Symbol.empty();
  }
};

} // end namespace remote
} // end namespace nextcode

#endif // NEXTCODE_REMOTE_REMOTEADDRESS_H

