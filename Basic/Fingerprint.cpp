//===--- Fingerprint.cpp - A stable identity for compiler data --*- C++ -*-===//
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

#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Fingerprint.h"
#include "nextcode/Basic/STLExtras.h"
#include "llvm/Support/Format.h"
#include "llvm/Support/raw_ostream.h"

#include <inttypes.h>
#include <sstream>

using namespace nextcode;

llvm::raw_ostream &llvm::operator<<(llvm::raw_ostream &OS,
                                    const Fingerprint &FP) {
  return OS << FP.getRawValue();
}

void nextcode::simple_display(llvm::raw_ostream &out, const Fingerprint &fp) {
  out << fp.getRawValue();
}

std::optional<Fingerprint> Fingerprint::fromString(llvm::StringRef value) {
  assert(value.size() == Fingerprint::DIGEST_LENGTH &&
         "Only supports 32-byte hash values!");
  auto fp = Fingerprint::ZERO();
  {
    std::istringstream s(value.drop_back(Fingerprint::DIGEST_LENGTH/2).str());
    s >> std::hex >> fp.core.first;
  }
  {
    std::istringstream s(value.drop_front(Fingerprint::DIGEST_LENGTH/2).str());
    s >> std::hex >> fp.core.second;
  }
  // If the input string is not valid hex, the conversion above can fail.
  if (value != fp.getRawValue())
    return std::nullopt;

  return fp;
}

llvm::SmallString<Fingerprint::DIGEST_LENGTH> Fingerprint::getRawValue() const {
  llvm::SmallString<Fingerprint::DIGEST_LENGTH> Str;
  llvm::raw_svector_ostream Res(Str);
  Res << llvm::format_hex_no_prefix(core.first, 16);
  Res << llvm::format_hex_no_prefix(core.second, 16);
  return Str;
}