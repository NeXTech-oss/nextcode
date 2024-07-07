//===--- Effects.cpp - Effect Checking ASTs -------------------------------===//
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
//  This file implements some logic for rethrows and reasync checking.
//
//===----------------------------------------------------------------------===//

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Effects.h"
#include "nextcode/AST/Evaluator.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/ProtocolConformanceRef.h"
#include "nextcode/AST/Type.h"
#include "nextcode/AST/Types.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

bool AnyFunctionType::hasEffect(EffectKind kind) const {
  switch (kind) {
  case EffectKind::Throws: return getExtInfo().isThrowing();
  case EffectKind::Async: return getExtInfo().isAsync();
  }
  llvm_unreachable("Bad effect kind");
}

void nextcode::simple_display(llvm::raw_ostream &out, const EffectKind kind) {
  switch (kind) {
  case EffectKind::Throws: out << "throws"; return;
  case EffectKind::Async: out << "async"; return;
  }
  llvm_unreachable("Bad effect kind");
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const PolymorphicEffectRequirementList list) {
  for (auto req : list.getRequirements()) {
    simple_display(out, req);
    out << "\n";
  }

  for (auto conf : list.getConformances()) {
    simple_display(out, conf.first);
    out << " : ";
    simple_display(out, conf.second);
    llvm::errs() << "\n";
  }
}

PolymorphicEffectRequirementList 
ProtocolDecl::getPolymorphicEffectRequirements(EffectKind kind) const {
  return evaluateOrDefault(getASTContext().evaluator,
    PolymorphicEffectRequirementsRequest{kind, const_cast<ProtocolDecl *>(this)},
    PolymorphicEffectRequirementList());
}

bool ProtocolDecl::hasPolymorphicEffect(EffectKind kind) const {
  switch (kind) {
  case EffectKind::Throws:
    return getAttrs().hasAttribute<nextcode::AtRethrowsAttr>();
  case EffectKind::Async:
    return getAttrs().hasAttribute<nextcode::AtReasyncAttr>();
  }
  llvm_unreachable("Bad effect kind");
}

bool AbstractFunctionDecl::hasEffect(EffectKind kind) const {
  switch (kind) {
  case EffectKind::Throws:
    return hasThrows();
  case EffectKind::Async:
    return hasAsync();
  }
  llvm_unreachable("Bad effect kind");
}

bool AbstractFunctionDecl::hasPolymorphicEffect(EffectKind kind) const {
  switch (kind) {
  case EffectKind::Throws:
    return getAttrs().hasAttribute<nextcode::RethrowsAttr>();
  case EffectKind::Async:
    return getAttrs().hasAttribute<nextcode::ReasyncAttr>();
  }
  llvm_unreachable("Bad effect kind");
}

PolymorphicEffectKind
AbstractFunctionDecl::getPolymorphicEffectKind(EffectKind kind) const {
  return evaluateOrDefault(getASTContext().evaluator,
    PolymorphicEffectKindRequest{kind, const_cast<AbstractFunctionDecl *>(this)},
    PolymorphicEffectKind::Invalid);
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           PolymorphicEffectKind kind) {
  switch (kind) {
  case PolymorphicEffectKind::None:
    out << "none";
    break;
  case PolymorphicEffectKind::ByClosure:
    out << "by closure";
    break;
  case PolymorphicEffectKind::ByConformance:
    out << "by conformance";
    break;
  case PolymorphicEffectKind::AsyncSequenceRethrows:
    out << "by async sequence implicit @rethrows";
    break;
  case PolymorphicEffectKind::Always:
    out << "always";
    break;
  case PolymorphicEffectKind::Invalid:
    out << "invalid";
    break;
  }
}

bool ProtocolConformanceRef::hasEffect(EffectKind kind) const {
  if (!isConcrete()) { return true; }
  return evaluateOrDefault(getRequirement()->getASTContext().evaluator,
     ConformanceHasEffectRequest{kind, getConcrete()},
     true);
}
