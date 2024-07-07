//===--- TypeContextInfo.h --------------------------------------*- C++ -*-===//
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

#ifndef NEXTCODE_IDE_TYPECONTEXTINFO_H
#define NEXTCODE_IDE_TYPECONTEXTINFO_H

#include "nextcode/AST/Type.h"
#include "nextcode/Basic/LLVM.h"

namespace nextcode {
class IDEInspectionCallbacksFactory;

namespace ide {

/// A result item for context info query.
class TypeContextInfoItem {
public:
  /// Possible expected type.
  Type ExpectedTy;
  /// Members of \c ExpectedTy which can be referenced by "Implicit Member
  /// Expression".
  SmallVector<ValueDecl *, 0> ImplicitMembers;

  TypeContextInfoItem(Type ExpectedTy) : ExpectedTy(ExpectedTy) {}
};

/// An abstract base class for consumers of context info results.
class TypeContextInfoConsumer {
public:
  virtual ~TypeContextInfoConsumer() {}
  virtual void handleResults(ArrayRef<TypeContextInfoItem>) = 0;
};

/// Create a factory for code completion callbacks.
IDEInspectionCallbacksFactory *
makeTypeContextInfoCallbacksFactory(TypeContextInfoConsumer &Consumer);

} // namespace ide
} // namespace nextcode

#endif // NEXTCODE_IDE_TYPECONTEXTINFO_H
