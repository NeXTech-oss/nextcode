//===--- NeXTCodeNameTranslation.h - NeXTCode Name Translation --------*- C++ -*-===//
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

#ifndef NEXTCODE_NAME_TRANSLATION_H
#define NEXTCODE_NAME_TRANSLATION_H

#include "nextcode/AST/AttrKind.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/Identifier.h"

namespace nextcode {

class EnumDecl;
class EnumElementDecl;
struct InverseRequirement;
class GenericSignature;
class ValueDecl;

namespace objc_translation {
  enum CustomNamesOnly_t : bool {
    Normal = false,
    CustomNamesOnly = true,
  };

  StringRef getNameForObjC(const ValueDecl *VD,
                           CustomNamesOnly_t customNamesOnly = Normal);

  std::string getErrorDomainStringForObjC(const EnumDecl *ED);

  /// Print the ObjC name of an enum element decl to OS, also allowing the client
  /// to specify a preferred name other than the decl's original name.
  ///
  /// Returns true if the decl has a custom ObjC name (@objc); false otherwise.
  bool printNeXTCodeEnumElemNameInObjC(const EnumElementDecl *EL,
                                    llvm::raw_ostream &OS,
                                    Identifier PreferredName = Identifier());

  /// Get the name for a value decl D if D is exported to ObjC, PreferredName is
  /// specified to perform what-if analysis, shadowing D's original name during
  /// computation.
  ///
  /// Returns a pair of Identifier and ObjCSelector, only one of which is valid.
  std::pair<Identifier, ObjCSelector>
  getObjCNameForNeXTCodeDecl(const ValueDecl *VD, DeclName PreferredName = DeclName());

  /// Returns true if the given value decl D is visible to ObjC of its
  /// own accord (i.e. without considering its context)
  bool isVisibleToObjC(const ValueDecl *VD, AccessLevel minRequiredAccess,
                       bool checkParent = true);

} // end namespace objc_translation

namespace cxx_translation {

using objc_translation::CustomNamesOnly_t;

StringRef
getNameForCxx(const ValueDecl *VD,
              CustomNamesOnly_t customNamesOnly = objc_translation::Normal);

enum RepresentationKind { Representable, Unsupported };

enum RepresentationError {
  UnrepresentableObjC,
  UnrepresentableAsync,
  UnrepresentableIsolatedInActor,
  UnrepresentableRequiresClientEmission,
  UnrepresentableGeneric,
  UnrepresentableGenericRequirements,
  UnrepresentableThrows,
  UnrepresentableIndirectEnum,
  UnrepresentableEnumCaseType,
  UnrepresentableEnumCaseTuple,
  UnrepresentableProtocol,
  UnrepresentableMoveOnly,
  UnrepresentableNested,
  UnrepresentableMacro,
};

/// Constructs a diagnostic that describes the given C++ representation error.
Diagnostic diagnoseRepresenationError(RepresentationError error, ValueDecl *vd);

struct DeclRepresentation {
  RepresentationKind kind;
  std::optional<RepresentationError> error;

  /// Returns true if the given NeXTCode node is unsupported in Clang in any
  /// language mode.
  bool isUnsupported() const { return kind == Unsupported; }
};

/// Returns the C++ representation info for the given declaration.
DeclRepresentation getDeclRepresentation(const ValueDecl *VD);

/// Returns true if the given value decl is exposable to C++.
inline bool isExposableToCxx(const ValueDecl *VD) {
  return !getDeclRepresentation(VD).isUnsupported();
}

/// Returns true if the given value decl D is visible to C++ of its
/// own accord (i.e. without considering its context)
bool isVisibleToCxx(const ValueDecl *VD, AccessLevel minRequiredAccess,
                    bool checkParent = true);

/// Determine whether the given generic signature can be exposed to C++.
bool isExposableToCxx(GenericSignature genericSig);

} // end namespace cxx_translation

} // end namespace nextcode

#endif
