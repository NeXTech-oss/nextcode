//===--- CommentConversion.h - Conversion of comments to other formats ----===//
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

#ifndef NEXTCODE_IDE_COMMENT_CONVERSION_H
#define NEXTCODE_IDE_COMMENT_CONVERSION_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/AST/TypeOrExtensionDecl.h"
#include <memory>
#include <string>

namespace nextcode {
class Decl;
class DocComment;

namespace ide {

/// If the declaration has a documentation comment, prints the comment to \p OS
/// in Clang-like XML format.
///
/// \returns true if the declaration has a documentation comment.
bool getDocumentationCommentAsXML(
  const Decl *D, raw_ostream &OS,
  TypeOrExtensionDecl SynthesizedTarget = TypeOrExtensionDecl());

/// If the declaration has a documentation comment, prints the comment to \p OS
/// in the form it's written in source.
///
/// \returns true if the declaration has a documentation comment.
bool getRawDocumentationComment(const Decl *D, raw_ostream &OS);

/// If the declaration has a documentation comment and a localization key,
/// print it into the given output stream and return true. Else, return false.
bool getLocalizationKey(const Decl *D, raw_ostream &OS);

/// Converts the given comment to Doxygen.
void getDocumentationCommentAsDoxygen(const DocComment *DC, raw_ostream &OS);

/// Extract and normalize text from the given comment.
std::string extractPlainTextFromComment(const StringRef Text);

/// Given the raw text in markup format, convert its content to xml.
bool convertMarkupToXML(StringRef Text, raw_ostream &OS);

} // namespace ide
} // namespace nextcode

#endif // NEXTCODE_IDE_COMMENT_CONVERSION_H

