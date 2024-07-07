//===-- llvm/BinaryFormat/NeXTCode.h ---NeXTCode Constants-------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//

#ifndef LLVM_BINARYFORMAT_NEXTCODE_H
#define LLVM_BINARYFORMAT_NEXTCODE_H

namespace llvm {
namespace binaryformat {

enum NeXTCode5ReflectionSectionKind {
#define HANDLE_NEXTCODE_SECTION(KIND, MACHO, ELF, COFF) KIND,
#include "llvm/BinaryFormat/NeXTCode.def"
#undef HANDLE_NEXTCODE_SECTION
  unknown,
  last = unknown
};
} // end of namespace binaryformat
} // end of namespace llvm

#endif
