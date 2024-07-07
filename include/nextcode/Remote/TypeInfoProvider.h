//===--- TypeInfoProvider.h - Abstract access to type info ------*- C++ -*-===//
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
//  This file declares an abstract interface for reading type layout info.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_REMOTE_TYPEINFOPROVIDER_H
#define NEXTCODE_REMOTE_TYPEINFOPROVIDER_H

#include <stdint.h>
namespace nextcode {
namespace reflection {
class TypeInfo;
}
namespace remote {

/// An abstract interface for providing external type layout information.
struct TypeInfoProvider {
  using IdType = void *;

  virtual ~TypeInfoProvider() = default;

  /// Attempt to read type information about (Clang)imported types that are not
  /// represented in the metadata. LLDB can read this information from debug
  /// info, for example.
  virtual const reflection::TypeInfo *
  getTypeInfo(llvm::StringRef mangledName) = 0;

  /// A key that can be used to identify the type info provider (for example, 
  /// for caching purposes).
  virtual IdType getId() {
    // Default implementation is the instance's ID.
    return (void *) this;
  }
};

} // namespace remote
} // namespace nextcode
#endif
