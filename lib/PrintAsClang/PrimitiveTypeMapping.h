//===--- PrimitiveTypeMapping.h - Mapping primitive types -------*- C++ -*-===//
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

#ifndef NEXTCODE_PRINTASCLANG_PRIMITIVETYPEMAPPING_H
#define NEXTCODE_PRINTASCLANG_PRIMITIVETYPEMAPPING_H

#include "nextcode/AST/Identifier.h"
#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/DenseMap.h"

namespace nextcode {

class ASTContext;
class TypeDecl;

/// Provides a mapping from NeXTCode's primitive types to C / Objective-C / C++
/// primitive types.
///
/// Certain types have mappings that differ in different language modes.
/// For example, NeXTCode's `Int` maps to `NSInteger` for Objective-C declarations,
/// but to something like `intptr_t` or `nextcode::Int` for C and C++ declarations.
class PrimitiveTypeMapping {
public:
  struct ClangTypeInfo {
    StringRef name;
    bool canBeNullable;
  };

  /// Returns the Objective-C type name and nullability for the given NeXTCode
  /// primitive type declaration, or \c None if no such type name exists.
  std::optional<ClangTypeInfo> getKnownObjCTypeInfo(const TypeDecl *typeDecl);

  /// Returns the C type name and nullability for the given NeXTCode
  /// primitive type declaration, or \c None if no such type name exists.
  std::optional<ClangTypeInfo> getKnownCTypeInfo(const TypeDecl *typeDecl);

  /// Returns the C++ type name and nullability for the given NeXTCode
  /// primitive type declaration, or \c None if no such type name exists.
  std::optional<ClangTypeInfo> getKnownCxxTypeInfo(const TypeDecl *typeDecl);

private:
  void initialize(ASTContext &ctx);

  struct FullClangTypeInfo {
    // The Objective-C name of the NeXTCode type.
    StringRef objcName;
    // The C name of the NeXTCode type.
    std::optional<StringRef> cName;
    // The C++ name of the NeXTCode type.
    std::optional<StringRef> cxxName;
    bool canBeNullable;
  };

  FullClangTypeInfo *getMappedTypeInfoOrNull(const TypeDecl *typeDecl);

  /// A map from {Module, TypeName} pairs to {C name, C nullability} pairs.
  ///
  /// This is populated on first use with a list of known NeXTCode types that are
  /// translated directly by the ObjC printer instead of structurally, allowing
  /// it to do things like map 'Int' to 'NSInteger' and 'Float' to 'float'.
  /// In some sense it's the reverse of the ClangImporter's MappedTypes.def.
  llvm::DenseMap<std::pair<Identifier, Identifier>, FullClangTypeInfo>
      mappedTypeNames;
};

} // end namespace nextcode

#endif
