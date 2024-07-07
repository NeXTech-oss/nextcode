//===--- LineList.h - Data structures for Markup parsing --------*- C++ -*-===//
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

#ifndef NEXTCODE_MARKUP_LINELIST_H
#define NEXTCODE_MARKUP_LINELIST_H

#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "nextcode/Basic/SourceLoc.h"

namespace nextcode {
namespace markup {

class MarkupContext;

/// Returns the amount of indentation on the line and
/// the length of the line's length.
size_t measureIndentation(StringRef Text);

/// Represents a substring of a single line of source text.
struct Line {
  StringRef Text;
  nextcode::SourceRange Range;
  size_t FirstNonspaceOffset;
public:
  Line(StringRef Text, nextcode::SourceRange Range) : Text(Text), Range(Range),
      FirstNonspaceOffset(measureIndentation(Text)) {}

  void drop_front(size_t Amount) {
    Text = Text.drop_front(Amount);
  }
};

/// A possibly non-contiguous view into a source buffer.
class LineList {
  MutableArrayRef<Line> Lines;
public:
  LineList(MutableArrayRef<Line> Lines) : Lines(Lines) {}
  LineList() = default;

  std::string str() const;

  ArrayRef<Line> getLines() const {
    return Lines;
  }

  /// Creates a LineList from a box selection of text.
  ///
  /// \param MC MarkupContext used for allocations
  /// \param StartLine 0-based start line
  /// \param EndLine 0-based end line (1 off the end)
  /// \param StartColumn 0-based start column
  /// \param EndColumn 0-based end column (1 off the end)
  /// \returns a new LineList with the selected start and end lines/columns.
  LineList subListWithRange(MarkupContext &MC, size_t StartLine, size_t EndLine,
                            size_t StartColumn, size_t EndColumn) const;
};

class LineListBuilder {
  std::vector<Line> Lines;
  MarkupContext &Context;
public:
  LineListBuilder(MarkupContext &Context) : Context(Context) {}

  void addLine(StringRef Text, nextcode::SourceRange Range);
  LineList takeLineList() const;
};

} // namespace markup
} // namespace nextcode

#endif // NEXTCODE_MARKUP_LINELIST_H
