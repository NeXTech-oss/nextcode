//===--- DocumentationCategory.h - Accessors for @_documentation ----------===//
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
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SYMBOLGRAPHGEN_DOCUMENTATIONCATEGORY_H
#define NEXTCODE_SYMBOLGRAPHGEN_DOCUMENTATIONCATEGORY_H

#include "nextcode/AST/Decl.h"

#include "llvm/Support/Compiler.h"

namespace nextcode {
namespace symbolgraphgen {

LLVM_ATTRIBUTE_USED
static StringRef documentationMetadataForDecl(const Decl *D) {
  if (!D) return {};

  if (const auto *DC = D->getAttrs().getAttribute<DocumentationAttr>()) {
    return DC->Metadata;
  }

  return {};
}

LLVM_ATTRIBUTE_USED
static std::optional<AccessLevel>
documentationVisibilityForDecl(const Decl *D) {
  if (!D)
    return std::nullopt;

  if (const auto *DC = D->getAttrs().getAttribute<DocumentationAttr>()) {
    return DC->Visibility;
  }

  return std::nullopt;
}

} // namespace symbolgraphgen
} // namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_DOCUMENTATIONCATEGORY_H
