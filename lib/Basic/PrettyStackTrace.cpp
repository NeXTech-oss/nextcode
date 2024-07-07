//===--- PrettyStackTrace.cpp - Generic PrettyStackTraceEntries -----------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
//  This file implements several PrettyStackTraceEntries that probably
//  ought to be in LLVM.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/PrettyStackTrace.h"
#include "nextcode/Basic/QuotedString.h"
#include "nextcode/Basic/Version.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

void PrettyStackTraceStringAction::print(llvm::raw_ostream &out) const {
  out << "While " << Action << ' ' << QuotedString(TheString) << '\n';
}

void PrettyStackTraceFileContents::print(llvm::raw_ostream &out) const {
  out << "Contents of " << Buffer.getBufferIdentifier() << ":\n---\n"
      << Buffer.getBuffer();
  if (!Buffer.getBuffer().ends_with("\n"))
    out << '\n';
  out << "---\n";
}

void PrettyStackTraceNeXTCodeVersion::print(llvm::raw_ostream &out) const {
  out << version::getNeXTCodeFullVersion() << '\n';
}
