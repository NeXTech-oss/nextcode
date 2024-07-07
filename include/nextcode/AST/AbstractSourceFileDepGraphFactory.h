//===----- AbstractSourceFileDepGraphFactory.h ----------------------*- C++ -*-===//
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

#ifndef NEXTCODE_AST_SOURCE_FILE_DEP_GRAPH_CONSTRUCTOR_H
#define NEXTCODE_AST_SOURCE_FILE_DEP_GRAPH_CONSTRUCTOR_H

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DeclContext.h"
#include "nextcode/AST/FineGrainedDependencies.h"
#include "llvm/Support/VirtualOutputBackend.h"
#include <optional>

namespace nextcode {
class DiagnosticEngine;
namespace fine_grained_dependencies {

/// Abstract class for building a \c SourceFileDepGraph from either a real
/// \c SourceFile or a unit test
class AbstractSourceFileDepGraphFactory {
protected:
  /// If there was an error, cannot get accurate info.
  const bool hadCompilationError;

  /// The name of the nextcodeDeps file.
  const std::string nextcodeDeps;

  /// The fingerprint of the whole file
  Fingerprint fileFingerprint;

  /// For debugging
  const bool emitDotFileAfterConstruction;

  DiagnosticEngine &diags;

  /// OutputBackend.
  llvm::vfs::OutputBackend &backend;

  /// Graph under construction
  SourceFileDepGraph g;

public:
  /// Expose this layer to enable faking up a constructor for testing.
  /// See the instance variable comments for explanation.
  AbstractSourceFileDepGraphFactory(bool hadCompilationError,
                                    StringRef nextcodeDeps,
                                    Fingerprint fileFingerprint,
                                    bool emitDotFileAfterConstruction,
                                    DiagnosticEngine &diags,
                                    llvm::vfs::OutputBackend &outputBackend);

  virtual ~AbstractSourceFileDepGraphFactory() = default;

  /// Create a SourceFileDepGraph.
  SourceFileDepGraph construct();

private:
  void addSourceFileNodesToGraph();

  /// Add the "provides" nodes when mocking up a graph
  virtual void addAllDefinedDecls() = 0;

  /// Add the "depends" nodes and arcs when mocking a graph
  virtual void addAllUsedDecls() = 0;

protected:
  /// Given an array of Decls or pairs of them in \p declsOrPairs
  /// create node pairs for context and name
  template <NodeKind kind, typename ContentsT>
  void addAllDefinedDeclsOfAGivenType(std::vector<ContentsT> &contentsVec) {
    for (const auto &declOrPair : contentsVec) {
      auto fp =
          AbstractSourceFileDepGraphFactory::getFingerprintIfAny(declOrPair);
      auto key = DependencyKey::Builder{kind, DeclAspect::interface}
                    .withContext(declOrPair)
                    .withName(declOrPair)
                    .build();
      addADefinedDecl(key, fp);
    }
  }

  /// Add an pair of interface, implementation nodes to the graph, which
  /// represent some \c Decl defined in this source file. \param key the
  /// interface key of the pair
  void addADefinedDecl(const DependencyKey &key,
                       std::optional<Fingerprint> fingerprint);

  void addAUsedDecl(const DependencyKey &def, const DependencyKey &use);

  /// Add an external dependency node to the graph. If the provided fingerprint
  /// is not \c None, it is added to the def key.
  void
  addAnExternalDependency(const DependencyKey &def, const DependencyKey &use,
                          std::optional<Fingerprint> dependencyFingerprint);

  static std::optional<Fingerprint>
  getFingerprintIfAny(std::pair<const NominalTypeDecl *, const ValueDecl *>) {
    return std::nullopt;
  }

  static std::optional<Fingerprint> getFingerprintIfAny(const Decl *d) {
    if (const auto *idc = dyn_cast<IterableDeclContext>(d)) {
      return idc->getBodyFingerprint();
    }
    return std::nullopt;
  }
};

} // namespace fine_grained_dependencies
} // namespace nextcode

#endif // NEXTCODE_AST_SOURCE_FILE_DEP_GRAPH_CONSTRUCTOR_H
