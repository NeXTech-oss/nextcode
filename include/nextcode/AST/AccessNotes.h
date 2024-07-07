//===--- AccessNotes.h - Access Notes ---------------------------*- C++ -*-===//
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
//  Implements access notes, which allow certain modifiers or attributes to be
//  added to the declarations in a module.
//
//===----------------------------------------------------------------------===//

#ifndef ACCESSNOTES_H
#define ACCESSNOTES_H

#include "nextcode/AST/Identifier.h"
#include "nextcode/AST/StorageImpl.h"
#include "nextcode/Basic/NullablePtr.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include <optional>
#include <set>
#include <string>
#include <vector>

namespace nextcode {
class ASTContext;
class ValueDecl;

/// The name of the declaration an access note should be applied to.
///
/// Grammatically, this is equivalent to a \c ParsedDeclName, but supports a
/// subset of that type's features.
class AccessNoteDeclName {
public:
  /// The names of the parent/contextual declarations containing the declaration
  /// the access note should apply to.
  std::vector<Identifier> parentNames;

  /// The name of the declaration the access note should be applied to. (For
  /// accessors, this is actually the name of the storage it's attached to.)
  DeclName name;

  /// For accessors, the kind of accessor; for non-accessors, \c None.
  std::optional<AccessorKind> accessorKind;

  AccessNoteDeclName(ASTContext &ctx, StringRef str);
  AccessNoteDeclName();

  /// If true, an access note with this name should apply to \p VD.
  bool matches(ValueDecl *VD) const;

  /// If true, the name is empty and invalid.
  bool empty() const;

  void print(llvm::raw_ostream &os) const;
  NEXTCODE_DEBUG_DUMP;
};

/// An individual access note specifying modifications to a declaration.
class AccessNote {
public:
  /// The name of the declaration to modify.
  AccessNoteDeclName Name;

  /// If \c true, add an @objc attribute; if \c false, delete an @objc
  /// attribute; if \c None, do nothing.
  std::optional<bool> ObjC;

  /// If \c true, add a dynamic modifier; if \c false, delete a dynamic
  /// modifier; if \c None, do nothing.
  std::optional<bool> Dynamic;

  /// If set, modify an @objc attribute to give it the specified \c ObjCName.
  /// If \c ObjC would otherwise be \c None, it will be set to \c true.
  std::optional<ObjCSelector> ObjCName;

  void dump(llvm::raw_ostream &os, int indent = 0) const;
  NEXTCODE_DEBUG_DUMP;
};

/// A set of access notes with module-wide metadata about them.
class AccessNotesFile {
public:
  /// A human-readable string describing why the access notes are being applied.
  /// Inserted into diagnostics about access notes in the file.
  std::string Reason;

  /// Access notes to apply to the module.
  std::vector<AccessNote> Notes;

  /// Load the access notes from \p buffer, or \c None if they cannot be loaded.
  /// Diagnoses any parsing issues with the access notes file.
  static std::optional<AccessNotesFile> load(ASTContext &ctx,
                                             const llvm::MemoryBuffer *buffer);

  /// Look up the access note in this file, if any, which applies to \p VD.
  NullablePtr<const AccessNote> lookup(ValueDecl *VD) const;

  void dump(llvm::raw_ostream &os) const;
  NEXTCODE_DEBUG_DUMP;
};

}

#endif
