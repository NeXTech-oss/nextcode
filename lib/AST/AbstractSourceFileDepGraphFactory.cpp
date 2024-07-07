//===-------- AbstractSourceFileDepGraphFactory.cpp -----------------------===//
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

#include "nextcode/AST/AbstractSourceFileDepGraphFactory.h"

// may not all be needed
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/FineGrainedDependencies.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/FileSystem.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/ReferenceDependencyKeys.h"
#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/VirtualOutputBackend.h"
#include "llvm/Support/YAMLParser.h"

using namespace nextcode;
using namespace fine_grained_dependencies;

//==============================================================================
// MARK: AbstractSourceFileDepGraphFactory - client interface
//==============================================================================

AbstractSourceFileDepGraphFactory::AbstractSourceFileDepGraphFactory(
    bool hadCompilationError, StringRef nextcodeDeps, Fingerprint fileFingerprint,
    bool emitDotFileAfterConstruction, DiagnosticEngine &diags,
    llvm::vfs::OutputBackend &backend)
    : hadCompilationError(hadCompilationError), nextcodeDeps(nextcodeDeps.str()),
      fileFingerprint(fileFingerprint),
      emitDotFileAfterConstruction(emitDotFileAfterConstruction), diags(diags),
      backend(backend) {}

SourceFileDepGraph AbstractSourceFileDepGraphFactory::construct() {
  addSourceFileNodesToGraph();
  if (!hadCompilationError) {
    addAllDefinedDecls();
    addAllUsedDecls();
  }
  assert(g.verify());
  if (emitDotFileAfterConstruction)
    g.emitDotFile(backend, nextcodeDeps, diags);
  return std::move(g);
}

//==============================================================================
// MARK: AbstractSourceFileDepGraphFactory - adding a defined or used Decl
//==============================================================================
void AbstractSourceFileDepGraphFactory::addSourceFileNodesToGraph() {
  g.findExistingNodePairOrCreateAndAddIfNew(
      DependencyKey::createKeyForWholeSourceFile(DeclAspect::interface,
                                                 nextcodeDeps),
      Fingerprint{fileFingerprint});
}

void AbstractSourceFileDepGraphFactory::addADefinedDecl(
    const DependencyKey &interfaceKey, std::optional<Fingerprint> fingerprint) {

  auto nodePair =
      g.findExistingNodePairOrCreateAndAddIfNew(interfaceKey, fingerprint);
  // Since the current type fingerprints only include tokens in the body,
  // when the interface hash changes, it is possible that the type in the
  // file has changed.
  g.addArc(g.getSourceFileNodePair().getInterface(), nodePair.getInterface());
}

void AbstractSourceFileDepGraphFactory::addAUsedDecl(
    const DependencyKey &defKey, const DependencyKey &useKey) {
  auto *defNode = g.findExistingNodeOrCreateIfNew(defKey, std::nullopt,
                                                  false /* = !isProvides */);

  // If the depended-upon node is defined in this file, then don't
  // create an arc to the user, when the user is the whole file.
  // Otherwise, if the defNode's type-body fingerprint changes,
  // the whole file will be marked as dirty, losing the benefit of the
  // fingerprint.

  //  if (defNode->getIsProvides() &&
  //      useKey.getKind() == NodeKind::sourceFileProvide)
  //    return;

  // Turns out the above three lines cause miscompiles, so comment them out
  // for now. We might want them back if we can change the inputs to this
  // function to be more precise.

  // Example of a miscompile:
  // In main.code
  // func foo(_: Any) { print("Hello Any") }
  //    foo(123)
  // Then add the following line to another file:
  // func foo(_: Int) { print("Hello Int") }
  // Although main.code needs to get recompiled, the commented-out code below
  // prevents that.

  auto nullableUse = g.findExistingNode(useKey);
  assert(nullableUse.isNonNull() && "Use must be an already-added provides");
  auto *useNode = nullableUse.get();
  assert(useNode->getIsProvides() && "Use (using node) must be a provides");
  g.addArc(defNode, useNode);
}

void AbstractSourceFileDepGraphFactory::addAnExternalDependency(
    const DependencyKey &defKey, const DependencyKey &useKey,
    std::optional<Fingerprint> maybeFP) {
  auto *defNode = g.findExistingNodeOrCreateIfNew(defKey, maybeFP,
                                                  false /* = !isProvides */);

  auto nullableUse = g.findExistingNode(useKey);
  assert(nullableUse.isNonNull() && "Use must be an already-added provides");
  auto *useNode = nullableUse.get();
  assert(useNode->getIsProvides() && "Use (using node) must be a provides");
  g.addArc(defNode, useNode);
}
