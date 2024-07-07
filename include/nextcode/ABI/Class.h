//===--- Class.h - Compiler/runtime class-metadata values -------*- C++ -*-===//
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
// This header provides target-independent information about class
// metadata.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_CLASS_H
#define NEXTCODE_ABI_CLASS_H

#include <stdint.h>

namespace nextcode {

/// Objective-C class flags, stored in the ro-data.
enum class ObjCClassFlags : uint32_t {
  /// This class is a metaclass.
  Meta                 = 0x00001,

  /// This class is a root class.
  Root                 = 0x00002,

  /// This class provides a non-trivial .cxx_construct or .cxx_destruct
  /// implementation.
  HasCXXStructors      = 0x00004,

  /// This class has hidden visibility.
  Hidden               = 0x00010,

  /// This class has the exception attribute.
  Exception            = 0x00020,

  /// This class provides a metadata update callback trailing the ro-data.
  /// Note that we're re-using the obsolete flag above.
  HasMetadataUpdateCallback = 0x00040,

  /// (Obsolete) ARC-specific: this class has a .release_ivars method.
  HasIvarReleaser      = 0x00040,

  /// This class implementation was compiled under ARC.
  CompiledByARC        = 0x00080,

  /// This class provides a non-trivial .cxx_destruct method, but
  /// its .cxx_construct is trivial.  For backwards compatibility,
  /// when setting this flag, HasCXXStructors must be set as well.
  HasCXXDestructorOnly = 0x00100,

  /// This class does not allow associated objects on instances.
  ///
  /// Will cause the objc runtime to trap in objc_setAssociatedObject.
  ForbidsAssociatedObjects = 0x00400,
};
inline ObjCClassFlags &operator|=(ObjCClassFlags &lhs, ObjCClassFlags rhs) {
  lhs = ObjCClassFlags(uint32_t(lhs) | uint32_t(rhs));
  return lhs;
}
inline ObjCClassFlags operator|(ObjCClassFlags lhs, ObjCClassFlags rhs) {
  return (lhs |= rhs);
}

}

#endif // NEXTCODE_ABI_CLASS_H