//- ClangNeXTCodeTypeCorrespondence.cpp - Relations between Clang & NeXTCode types -//
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
// See description in ClangNeXTCodeTypeCorrespondence.h.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ClangNeXTCodeTypeCorrespondence.h"
#include "nextcode/Basic/Assertions.h"
#include "clang/AST/Type.h"

bool nextcode::canImportAsOptional(const clang::Type *type) {
  // Note: this mimics ImportHint::canImportAsOptional.

  // Includes CoreFoundation types such as CFStringRef (== struct CFString *).
  return type && (type->isPointerType()
                  || type->isBlockPointerType()
                  || type->isObjCObjectPointerType());
}
