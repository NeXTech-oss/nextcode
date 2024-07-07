//===---- FineGrainedDependencies.cpp - Generates nextcodedeps files ---------===//
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

#include "nextcode/AST/FineGrainedDependencies.h"

// may not all be needed
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/DiagnosticsCommon.h"
#include "nextcode/AST/DiagnosticsFrontend.h"
#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/FineGrainedDependencyFormat.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/FileSystem.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Demangling/Demangle.h"
#include "nextcode/Frontend/FrontendOptions.h"

#include "llvm/ADT/MapVector.h"
#include "llvm/ADT/SetVector.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Path.h"


// This file holds the definitions for the fine-grained dependency system
// that are likely to be stable as it moves away from the status quo.
// These include the graph structures common to both programs and also
// the frontend graph, which must be read by the driver.

using namespace nextcode;
using namespace fine_grained_dependencies;

//==============================================================================
// MARK: Emitting and reading SourceFileDepGraph
//==============================================================================

std::optional<SourceFileDepGraph>
SourceFileDepGraph::loadFromPath(StringRef path, const bool allowNeXTCodeModule) {
  const bool treatAsModule =
      allowNeXTCodeModule &&
      path.ends_with(file_types::getExtension(file_types::TY_NeXTCodeModuleFile));
  auto bufferOrError = llvm::MemoryBuffer::getFile(path);
  if (!bufferOrError)
    return std::nullopt;
  return treatAsModule ? loadFromNeXTCodeModuleBuffer(*bufferOrError.get())
                       : loadFromBuffer(*bufferOrError.get());
}

std::optional<SourceFileDepGraph>
SourceFileDepGraph::loadFromBuffer(llvm::MemoryBuffer &buffer) {
  SourceFileDepGraph fg;
  if (nextcode::fine_grained_dependencies::readFineGrainedDependencyGraph(
      buffer, fg))
    return std::nullopt;
  return std::optional<SourceFileDepGraph>(std::move(fg));
}

std::optional<SourceFileDepGraph>
SourceFileDepGraph::loadFromNeXTCodeModuleBuffer(llvm::MemoryBuffer &buffer) {
  SourceFileDepGraph fg;
  if (nextcode::fine_grained_dependencies::
          readFineGrainedDependencyGraphFromNeXTCodeModule(buffer, fg))
    return std::nullopt;
  return std::optional<SourceFileDepGraph>(std::move(fg));
}

//==============================================================================
// MARK: SourceFileDepGraph access
//==============================================================================

SourceFileDepGraphNode *
SourceFileDepGraph::getNode(size_t sequenceNumber) const {
  assert(sequenceNumber < allNodes.size() && "Bad node index");
  SourceFileDepGraphNode *n = allNodes[sequenceNumber];
  assert(n->getSequenceNumber() == sequenceNumber &&
         "Bad sequence number in node or bad entry in allNodes.");
  return n;
}

InterfaceAndImplementationPair<SourceFileDepGraphNode>
SourceFileDepGraph::getSourceFileNodePair() const {
  return InterfaceAndImplementationPair<SourceFileDepGraphNode>(
      getNode(
          SourceFileDepGraphNode::sourceFileProvidesInterfaceSequenceNumber),
      getNode(SourceFileDepGraphNode::
                  sourceFileProvidesImplementationSequenceNumber));
}

StringRef SourceFileDepGraph::getNeXTCodeDepsOfJobThatProducedThisGraph() const {
  return getSourceFileNodePair()
      .getInterface()
      ->getKey()
      .getNeXTCodeDepsFromASourceFileProvideNodeKey();
}

void SourceFileDepGraph::forEachArc(
    function_ref<void(const SourceFileDepGraphNode *def,
                      const SourceFileDepGraphNode *use)>
        fn) const {
  forEachNode([&](const SourceFileDepGraphNode *useNode) {
    forEachDefDependedUponBy(useNode, [&](SourceFileDepGraphNode *defNode) {
      fn(defNode, useNode);
    });
  });
}

InterfaceAndImplementationPair<SourceFileDepGraphNode>
SourceFileDepGraph::findExistingNodePairOrCreateAndAddIfNew(
    const DependencyKey &interfaceKey, std::optional<Fingerprint> fingerprint) {

  // Optimization for whole-file users:
  if (interfaceKey.getKind() == NodeKind::sourceFileProvide &&
      !allNodes.empty())
    return getSourceFileNodePair();

  assert(interfaceKey.isInterface());
  const DependencyKey implementationKey =
      interfaceKey.correspondingImplementation();
  auto *interfaceNode = findExistingNodeOrCreateIfNew(interfaceKey, fingerprint,
                                                      true /* = isProvides */);
  auto *implementationNode = findExistingNodeOrCreateIfNew(
      implementationKey, fingerprint, true /* = isProvides */);

  InterfaceAndImplementationPair<SourceFileDepGraphNode> nodePair{
      interfaceNode, implementationNode};

  // if interface changes, have to rebuild implementation.
  // This dependency used to be represented by
  // addArc(nodePair.getInterface(), nodePair.getImplementation());
  // However, recall that the dependency scheme as of 1/2020 chunks
  // declarations together by base name.
  // So if the arc were added, a dirtying of a same-based-named interface
  // in a different file would dirty the implementation in this file,
  // causing the needless recompilation of this file.
  // But, if an arc is added for this, then *any* change that causes
  // a same-named interface to be dirty will dirty this implementation,
  // even if that interface is in another file.
  // Therefore no such arc is added here, and any dirtying of either
  // the interface or implementation of this declaration will cause
  // the driver to recompile this source file.
  return nodePair;
}

SourceFileDepGraphNode *SourceFileDepGraph::findExistingNodeOrCreateIfNew(
    const DependencyKey &key, const std::optional<Fingerprint> fingerprint,
    const bool isProvides) {
  SourceFileDepGraphNode *result = memoizedNodes.findExistingOrCreateIfNew(
      key, [&](DependencyKey key) -> SourceFileDepGraphNode * {
        SourceFileDepGraphNode *n =
            new SourceFileDepGraphNode(key, fingerprint, isProvides);
        addNode(n);
        return n;
      });
  assert(result->getKey() == key && "Keys must match.");
  if (!isProvides)
    return result;
  // If have provides and depends with same key, result is one node that
  // isProvides
  if (!result->getIsProvides() && fingerprint) {
    result->setIsProvides();
    assert(!result->getFingerprint() && "Depends should not have fingerprints");
    result->setFingerprint(fingerprint);
    return result;
  }
  // If there are two Decls with same base name but differ only in fingerprint,
  // since we won't be able to tell which Decl is depended-upon (is this right?)
  // just use the one node, but erase its print:
  if (fingerprint != result->getFingerprint())
    result->setFingerprint(std::nullopt);
  return result;
}

NullablePtr<SourceFileDepGraphNode>
SourceFileDepGraph::findExistingNode(const DependencyKey &key) {
  auto existing = memoizedNodes.findExisting(key);
  return existing ? existing.value() : NullablePtr<SourceFileDepGraphNode>();
}

std::string DependencyKey::demangleTypeAsContext(StringRef s) {
  return nextcode::Demangle::demangleTypeAsString(s.str());
}

DependencyKey DependencyKey::createKeyForWholeSourceFile(DeclAspect aspect,
                                                         StringRef nextcodeDeps) {
  assert(!nextcodeDeps.empty());
  return DependencyKey::Builder(NodeKind::sourceFileProvide, aspect)
      .withName(nextcodeDeps)
      .build();
}

//==============================================================================
// MARK: Debugging
//==============================================================================

bool SourceFileDepGraph::verify() const {
  DependencyKey::verifyNodeKindNames();
  DependencyKey::verifyDeclAspectNames();
  // Ensure Keys are unique
  std::unordered_map<DependencyKey, SourceFileDepGraphNode *> nodesSeen;
  // Ensure each node only appears once.
  std::unordered_set<void *> nodes;
  forEachNode([&](SourceFileDepGraphNode *n) {
    n->verify();
    assert(nodes.insert(n).second && "Frontend nodes are identified by "
                                     "sequence number, therefore must be "
                                     "unique.");

    auto iterInserted = nodesSeen.insert(std::make_pair(n->getKey(), n));
    if (!iterInserted.second) {
      llvm::errs() << "Duplicate frontend keys: ";
      iterInserted.first->second->dump(llvm::errs());
      llvm::errs() << " and ";
      n->dump(llvm::errs());
      llvm::errs() << "\n";
      llvm_unreachable("duplicate frontend keys");
    }

    forEachDefDependedUponBy(n, [&](SourceFileDepGraphNode *def) {
      assert(def != n && "Uses should be irreflexive.");
    });
  });
  return true;
}

bool SourceFileDepGraph::verifyReadsWhatIsWritten(StringRef path) const {
  auto loadedGraph = SourceFileDepGraph::loadFromPath(path);
  assert(loadedGraph.has_value() &&
         "Should be able to read the exported graph.");
  verifySame(loadedGraph.value());
  return true;
}

std::string DependencyKey::humanReadableName() const {
  switch (kind) {
  case NodeKind::member:
    return demangleTypeAsContext(context) + "." + name;
  case NodeKind::externalDepend:
  case NodeKind::sourceFileProvide:
    return llvm::sys::path::filename(name).str();
  case NodeKind::potentialMember:
    return demangleTypeAsContext(context) + ".*";
  case NodeKind::nominal:
    return demangleTypeAsContext(context);
  case NodeKind::topLevel:
  case NodeKind::dynamicLookup:
    return name;
  default:
    llvm_unreachable("bad kind");
  }
}

std::string DependencyKey::asString() const {
  return NodeKindNames[size_t(kind)] + " " + "aspect: " + aspectName().str() +
         ", " + humanReadableName();
}

/// Needed for TwoStageMap::verify:
raw_ostream &fine_grained_dependencies::operator<<(raw_ostream &out,
                                                   const DependencyKey &key) {
  out << key.asString();
  return out;
}

bool DependencyKey::verify() const {
  assert((getKind() != NodeKind::externalDepend || isInterface()) &&
         "All external dependencies must be interfaces.");
  switch (getKind()) {
  case NodeKind::topLevel:
  case NodeKind::dynamicLookup:
  case NodeKind::externalDepend:
  case NodeKind::sourceFileProvide:
    assert(context.empty() && !name.empty() && "Must only have a name");
    break;
  case NodeKind::nominal:
  case NodeKind::potentialMember:
    assert(!context.empty() && name.empty() && "Must only have a context");
    break;
  case NodeKind::member:
    assert(!context.empty() && !name.empty() && "Must have both");
    break;
  case NodeKind::kindCount:
    llvm_unreachable("impossible");
  }
  return true;
}

/// Since I don't have NeXTCode enums, ensure name correspondence here.
void DependencyKey::verifyNodeKindNames() {
  for (size_t i = 0; i < size_t(NodeKind::kindCount); ++i)
    switch (NodeKind(i)) {
#define CHECK_NAME(n)                                                          \
  case NodeKind::n:                                                            \
    assert(#n == NodeKindNames[i]);                                            \
    break;
      CHECK_NAME(topLevel)
      CHECK_NAME(nominal)
      CHECK_NAME(potentialMember)
      CHECK_NAME(member)
      CHECK_NAME(dynamicLookup)
      CHECK_NAME(externalDepend)
      CHECK_NAME(sourceFileProvide)
    case NodeKind::kindCount:
      llvm_unreachable("impossible");
    }
#undef CHECK_NAME
}

/// Since I don't have NeXTCode enums, ensure name correspondence here.
void DependencyKey::verifyDeclAspectNames() {
  for (size_t i = 0; i < size_t(DeclAspect::aspectCount); ++i)
    switch (DeclAspect(i)) {
#define CHECK_NAME(n)                                                          \
  case DeclAspect::n:                                                          \
    assert(#n == DeclAspectNames[i]);                                          \
    break;
      CHECK_NAME(interface)
      CHECK_NAME(implementation)
    case DeclAspect::aspectCount:
      llvm_unreachable("impossible");
    }
#undef CHECK_NAME
}

void DepGraphNode::dump() const {
  dump(llvm::errs());
}

void DepGraphNode::dump(raw_ostream &os) const {
  key.dump(os);
  if (fingerprint.has_value())
    os << "fingerprint: " << fingerprint.value() << "";
  else
    os << "no fingerprint";
}

void SourceFileDepGraphNode::dump() const {
  dump(llvm::errs());
}

void SourceFileDepGraphNode::dump(raw_ostream &os) const {
  DepGraphNode::dump(os);
  os << " sequence number: " << sequenceNumber;
  os << " is provides: " << isProvides;
  os << " depends on:";
  for (auto def : defsIDependUpon)
    os << " " << def;
}

std::string DepGraphNode::humanReadableName(StringRef where) const {

  return getKey().humanReadableName() +
         (getKey().getKind() == NodeKind::sourceFileProvide || where.empty()
              ? std::string()
              : std::string(" in ") + where.str());
}

void SourceFileDepGraph::verifySame(const SourceFileDepGraph &other) const {
  assert(allNodes.size() == other.allNodes.size() &&
         "Both graphs must have same number of nodes.");
#ifndef NDEBUG
  for (size_t i : indices(allNodes)) {
    assert(*allNodes[i] == *other.allNodes[i] &&
           "Both graphs must have corresponding nodes");
  }
#endif
}

void SourceFileDepGraph::emitDotFile(llvm::vfs::OutputBackend &outputBackend,
                                     StringRef outputPath,
                                     DiagnosticEngine &diags) {
  std::string dotFileName = outputPath.str() + ".dot";
  withOutputPath(
      diags, outputBackend, dotFileName, [&](llvm::raw_pwrite_stream &out) {
        DotFileEmitter<SourceFileDepGraph>(out, *this, false, false).emit();
        return false;
      });
}