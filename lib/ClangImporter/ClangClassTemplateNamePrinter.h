//===--- ClangClassTemplateNamePrinter.h ------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_CLANG_TEMPLATE_NAME_PRINTER_H
#define NEXTCODE_CLANG_TEMPLATE_NAME_PRINTER_H

#include "ImportName.h"
#include "nextcode/AST/ASTContext.h"
#include "clang/AST/DeclTemplate.h"

namespace nextcode {
namespace importer {

/// Returns a NeXTCode representation of a C++ class template specialization name,
/// e.g. "vector<CWideChar, allocator<CWideChar>>".
///
/// This expands the entire tree of template instantiation names recursively.
/// While printing deep instantiation levels might not increase readability, it
/// is important to do because the C++ templated class names get mangled,
/// therefore they must be unique for different instantiations.
///
/// This function does not instantiate any templates and does not modify the AST
/// in any way.
std::string printClassTemplateSpecializationName(
    const clang::ClassTemplateSpecializationDecl *decl, ASTContext &nextcodeCtx,
    NameImporter *nameImporter, ImportNameVersion version);

} // namespace importer
} // namespace nextcode

#endif // NEXTCODE_CLANG_TEMPLATE_NAME_PRINTER_H
