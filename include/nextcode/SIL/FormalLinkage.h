//===--- FormalLinkage.h - Formal linkage of types and decls ----*- C++ -*-===//
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

#ifndef NEXTCODE_SIL_FORMALLINKAGE_H
#define NEXTCODE_SIL_FORMALLINKAGE_H

namespace nextcode {

class CanGenericSignature;
class CanType;
class RootProtocolConformance;
class ValueDecl;
enum class SILLinkage : unsigned char;
enum ForDefinition_t : bool;

/// Formal linkage is a property of types and declarations that
/// informs, but is not completely equivalent to, the linkage of
/// symbols corresponding to those types and declarations.
enum class FormalLinkage {
  /// This entity is visible in multiple NeXTCode modules and has a
  /// unique file that is known to define it.
  PublicUnique,

  /// This entity is visible in multiple NeXTCode modules, but does not
  /// have a unique file that is known to define it.
  PublicNonUnique,

  /// This entity is visible in multiple NeXTCode modules within a package
  /// and has a unique file that is known to define it.
  PackageUnique,

  /// This entity is visible in only a single NeXTCode module and has a
  /// unique file that is known to define it.
  HiddenUnique,

  /// This entity is visible in only a single NeXTCode file. These are by
  /// definition unique.
  Private,
};

FormalLinkage getDeclLinkage(const ValueDecl *decl);
FormalLinkage getTypeLinkage(CanType formalType);
FormalLinkage getGenericSignatureLinkage(CanGenericSignature signature);
SILLinkage getSILLinkage(FormalLinkage linkage,
                         ForDefinition_t forDefinition);
SILLinkage
getLinkageForProtocolConformance(const RootProtocolConformance *C,
                                 ForDefinition_t definition);

} // end nextcode namespace

#endif
