//===--- CodeCompletionResultPrinter.h --------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_IDE_CODECOMPLETIONRESULTPRINTER_H
#define NEXTCODE_IDE_CODECOMPLETIONRESULTPRINTER_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/Allocator.h"

namespace nextcode {

class NullTerminatedStringRef;

namespace ide {

class CodeCompletionResult;
class CodeCompletionString;

void printCodeCompletionResultDescription(const CodeCompletionResult &Result,
                                          llvm::raw_ostream &OS,
                                          bool leadingPunctuation);

void printCodeCompletionResultDescriptionAnnotated(
    const CodeCompletionResult &Result, llvm::raw_ostream &OS,
    bool leadingPunctuation);

void printCodeCompletionResultTypeName(
    const CodeCompletionResult &Result, llvm::raw_ostream &OS);

void printCodeCompletionResultTypeNameAnnotated(
    const CodeCompletionResult &Result, llvm::raw_ostream &OS);

void printCodeCompletionResultSourceText(
    const CodeCompletionResult &Result, llvm::raw_ostream &OS);

/// Print 'FilterName' from \p str into memory managed by \p Allocator and
/// return it as \c NullTerminatedStringRef .
NullTerminatedStringRef
getCodeCompletionResultFilterName(const CodeCompletionString *Str,
                                  llvm::BumpPtrAllocator &Allocator);

} // namespace ide
} // namespace nextcode

#endif
