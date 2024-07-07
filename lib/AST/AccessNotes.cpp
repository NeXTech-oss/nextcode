//===--- AccessNotes.cpp - Access Notes -------------------------*- C++ -*-===//
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

#include "nextcode/AST/AccessNotes.h"
#include "nextcode/AST/Attr.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Module.h"     // DeclContext::isModuleScopeContext()
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Parse/Parser.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/Support/YAMLTraits.h"

namespace nextcode {

AccessNoteDeclName::AccessNoteDeclName()
    : parentNames(), name(), accessorKind(std::nullopt) {}

AccessNoteDeclName::AccessNoteDeclName(ASTContext &ctx, StringRef str) {
  auto parsedName = parseDeclName(str);

  StringRef first, rest = parsedName.ContextName;
  while (!rest.empty()) {
    std::tie(first, rest) = rest.split('.');
    parentNames.push_back(ctx.getIdentifier(first));
  }

  if (parsedName.IsGetter)
    accessorKind = AccessorKind::Get;
  else if (parsedName.IsSetter)
    accessorKind = AccessorKind::Set;
  else
    accessorKind = std::nullopt;

  name = parsedName.formDeclName(ctx, /*isSubscript=*/true);
}

bool AccessNoteDeclName::matches(ValueDecl *VD) const {
  // The declaration whose name and context we ought to match. Usually `VD`, but
  // when `VD` is an accessor, it becomes the accessor's storage instead.
  auto lookupVD = VD;

  // First, we check if the accessor-ness of `VD` matches the accessor-ness of
  // the name, and update `lookupVD` if necessary.
  if (auto accessor = dyn_cast<AccessorDecl>(VD)) {
    if (!accessorKind || *accessorKind != accessor->getAccessorKind())
      return false;
    lookupVD = accessor->getStorage();
  }
  else if (accessorKind.has_value())
    return false;

  // Check that `name` matches `lookupVD`.
  if (!lookupVD->getName().matchesRef(name))
    return false;

  // The rest of this checks `parentNames` against the parents of `lookupVD`.

  ArrayRef<Identifier> remainingContextNames = parentNames;
  DeclContext *nextContext = lookupVD->getDeclContext();

  while (!nextContext->isModuleScopeContext()) {
    // If we've run out of names without reaching module scope, we've failed.
    if (remainingContextNames.empty())
      return false;

    Identifier contextName = remainingContextNames.back();

    // If the context is not a type (or extension), we can't name VD in an
    // access note and the match fails; if the name doesn't match, the match
    // fails too.
    auto contextType = nextContext->getSelfNominalTypeDecl();
    if (!contextType || contextType->getName() != contextName)
      return false;

    // Still checking. Move to the parent.
    remainingContextNames = remainingContextNames.drop_back();
    nextContext = contextType->getParent();
  }

  // If the context is module-scoped, we've succeeded if we're out of names, or
  // failed if we still have some names to go.
  return remainingContextNames.empty();
}

bool AccessNoteDeclName::empty() const {
  return !name;
}

void AccessNoteDeclName::print(llvm::raw_ostream &os) const {
  if (accessorKind)
    os << getAccessorLabel(*accessorKind) << "ter:";

  for (auto parentName : parentNames)
    os << parentName << '.';
  name.print(os, /*skipEmptyArgumentNames=*/false);
}

void AccessNoteDeclName::dump() const {
  print(llvm::errs());
  llvm::errs() << '\n';
}

NullablePtr<const AccessNote> AccessNotesFile::lookup(ValueDecl *VD) const {
  assert(VD != nullptr);

  auto iter = llvm::find_if(Notes, [&](const AccessNote &note) -> bool {
    return note.Name.matches(VD);
  });

  return NullablePtr<const AccessNote>(iter == Notes.end() ? nullptr : &*iter);
}

void AccessNotesFile::dump() const {
  dump(llvm::dbgs());
  llvm::dbgs() << "\n";
}
void AccessNote::dump() const {
  dump(llvm::dbgs());
  llvm::dbgs() << "\n";
}

void AccessNotesFile::dump(llvm::raw_ostream &os) const {
  os << "(access_notes reason='" << Reason << "'";
  for (const auto &note : Notes) {
    os << "\n";
    note.dump(os, /*indent=*/2);
  }
  os << ")";
}

void AccessNote::dump(llvm::raw_ostream &os, int indent) const {
  os.indent(indent) << "(note name='";
  Name.print(os);
  os << "'";

  if (Name.name.getBaseName().isSpecial())
    os << " is_special_name";

  if (ObjC)
    os << " objc=" << *ObjC;
  if (ObjCName)
    os << " objc_name='" << *ObjCName << "'";
  if (Dynamic)
    os << " dynamic=" << *Dynamic;

  os << ")";
}

}

LLVM_YAML_DECLARE_SCALAR_TRAITS(nextcode::AccessNoteDeclName, QuotingType::Single)
LLVM_YAML_DECLARE_SCALAR_TRAITS(nextcode::ObjCSelector, QuotingType::Single)
LLVM_YAML_IS_SEQUENCE_VECTOR(nextcode::AccessNote)
LLVM_YAML_DECLARE_MAPPING_TRAITS(nextcode::AccessNotesFile)

// Not using macro to avoid validation issues.
template <> struct llvm::yaml::MappingTraits<nextcode::AccessNote> {
  static void mapping(IO &IO, nextcode::AccessNote &Obj);
  static std::string validate(IO &IO, nextcode::AccessNote &Obj);
};

namespace nextcode {

static void
convertToErrorAndJoin(const llvm::SMDiagnostic &diag, void *ctxPtr) {
  ASTContext &ctx = *(ASTContext*)ctxPtr;

  SourceLoc loc{diag.getLoc()};
  assert(ctx.SourceMgr.isOwning(loc));

  switch (diag.getKind()) {
  case llvm::SourceMgr::DK_Error:
    ctx.Diags.diagnose(loc, diag::error_in_access_notes_file,
                       diag.getMessage());
    break;

  default:
    ctx.Diags.diagnose(loc, diag::warning_in_access_notes_file,
                       diag.getMessage());
    break;
  }
}

std::optional<AccessNotesFile>
AccessNotesFile::load(ASTContext &ctx, const llvm::MemoryBuffer *buffer) {
  llvm::yaml::Input yamlIn(llvm::MemoryBufferRef(*buffer), (void *)&ctx,
                           convertToErrorAndJoin, &ctx);
  yamlIn.setAllowUnknownKeys(true);

  AccessNotesFile notes;
  yamlIn >> notes;

  if (yamlIn.error())
    return std::nullopt;

  return notes;
}
}


namespace llvm {
namespace yaml {

using AccessNote = nextcode::AccessNote;
using AccessNotesFile = nextcode::AccessNotesFile;
using ASTContext = nextcode::ASTContext;
using AccessNoteDeclName = nextcode::AccessNoteDeclName;
using ObjCSelector = nextcode::ObjCSelector;

void ScalarTraits<AccessNoteDeclName>::
output(const AccessNoteDeclName &name, void *ctxPtr, raw_ostream &os) {
  name.print(os);
}

StringRef ScalarTraits<AccessNoteDeclName>::
input(StringRef str, void *ctxPtr, AccessNoteDeclName &name) {
  auto &ctx = *static_cast<nextcode::ASTContext *>(ctxPtr);

  name = AccessNoteDeclName(ctx, str);
  return name.empty() ? "invalid declaration name" : "";
}

void ScalarTraits<ObjCSelector>::output(const ObjCSelector &selector,
                                        void *ctxPtr, raw_ostream &os) {
  os << selector;
}

StringRef ScalarTraits<ObjCSelector>::input(StringRef str, void *ctxPtr,
                                            ObjCSelector &selector) {
  auto &ctx = *static_cast<nextcode::ASTContext *>(ctxPtr);

  if (auto sel = ObjCSelector::parse(ctx, str)) {
    selector = *sel;
    return "";
  }

  return "invalid selector";
}

void MappingTraits<AccessNote>::mapping(IO &io, AccessNote &note) {
  io.mapRequired("Name", note.Name);
  io.mapOptional("ObjC", note.ObjC);
  io.mapOptional("Dynamic", note.Dynamic);
  io.mapOptional("ObjCName", note.ObjCName);
}

std::string MappingTraits<AccessNote>::validate(IO &io, AccessNote &note) {
  if (note.ObjCName.has_value()) {
    if (!note.ObjC)
      note.ObjC = true;
    else if (!*note.ObjC)
      return "cannot have an 'ObjCName' if 'ObjC' is false";
  }

  return "";
}

void MappingTraits<AccessNotesFile>::mapping(IO &io, AccessNotesFile &notes) {
  io.mapRequired("Reason", notes.Reason);
  io.mapRequired("Notes", notes.Notes);
}

}
}