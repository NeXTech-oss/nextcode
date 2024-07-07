//===--- FuzzyStringMatcher.h - ---------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_IDE_FUZZYSTRINGMATCHER_H
#define NEXTCODE_IDE_FUZZYSTRINGMATCHER_H

#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/BitVector.h"
#include <string>

namespace nextcode {
namespace ide {

/// FuzzyStringMatcher compares candidate strings against a pattern
/// string using a fuzzy matching algorithm and provides a numerical
/// score for the match quality.
///
/// The inputs should be UTF8 strings, but the implementation is not currently
/// unicode-correct in that no normalization or non-ASCII upper/lower casing is
/// supported.  Non-ASCII bytes in the input are treated as opaque.
class FuzzyStringMatcher {
  std::string pattern;
  std::string lowercasePattern;
  double maxScore; ///< The maximum possible raw score for this pattern.
  /// If (and only if) c is in pattern, charactersInPattern[c] == 1
  llvm::BitVector charactersInPattern;

public:
  bool normalize = false; ///< Whether to normalize scores to [0, 1].

public:
  FuzzyStringMatcher(StringRef pattern);

  /// Whether \p candidate matches the pattern.
  ///
  /// This operation is much simpler/faster than calculating
  /// the candidate's score.
  bool matchesCandidate(StringRef candidate) const;

  /// Calculates the numerical score for \p candidate.
  double scoreCandidate(StringRef candidate) const;
};

} // namespace ide
} // namespace nextcode

#endif // NEXTCODE_IDE_FUZZYSTRINGMATCHER_H
