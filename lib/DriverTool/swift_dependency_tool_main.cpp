//===--- nextcode-dependency-tool.cpp - Convert binary nextcodedeps to YAML -----===//
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

#include "nextcode/AST/FileSystem.h"
#include "nextcode/AST/FineGrainedDependencies.h"
#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/AST/FineGrainedDependencyFormat.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Basic/LLVMInitialize.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/VirtualOutputBackends.h"
#include "llvm/Support/YAMLParser.h"
#include "llvm/Support/YAMLTraits.h"

using namespace nextcode;
using namespace fine_grained_dependencies;

//==============================================================================
// MARK: SourceFileDepGraph YAML reading & writing
//==============================================================================

// This introduces a redefinition wherever std::is_same_t<size_t, uint64_t>
// holds.
#if !(defined(__linux__) || defined(_WIN64) || defined(__FreeBSD__))
LLVM_YAML_DECLARE_SCALAR_TRAITS(size_t, QuotingType::None)
#endif
LLVM_YAML_DECLARE_ENUM_TRAITS(nextcode::fine_grained_dependencies::NodeKind)
LLVM_YAML_DECLARE_ENUM_TRAITS(nextcode::fine_grained_dependencies::DeclAspect)
LLVM_YAML_DECLARE_MAPPING_TRAITS(
    nextcode::fine_grained_dependencies::DependencyKey)
LLVM_YAML_DECLARE_MAPPING_TRAITS(nextcode::fine_grained_dependencies::DepGraphNode)

namespace llvm {
namespace yaml {
template <>
struct MappingContextTraits<
    nextcode::fine_grained_dependencies::SourceFileDepGraphNode,
    nextcode::fine_grained_dependencies::SourceFileDepGraph> {
  using SourceFileDepGraphNode =
      nextcode::fine_grained_dependencies::SourceFileDepGraphNode;
  using SourceFileDepGraph =
      nextcode::fine_grained_dependencies::SourceFileDepGraph;

  static void mapping(IO &io, SourceFileDepGraphNode &node,
                      SourceFileDepGraph &g);
};

template <>
struct SequenceTraits<
    std::vector<nextcode::fine_grained_dependencies::SourceFileDepGraphNode *>> {
  using SourceFileDepGraphNode =
      nextcode::fine_grained_dependencies::SourceFileDepGraphNode;
  using NodeVec = std::vector<SourceFileDepGraphNode *>;
  static size_t size(IO &, NodeVec &vec);
  static SourceFileDepGraphNode &element(IO &, NodeVec &vec, size_t index);
};

template <> struct ScalarTraits<nextcode::Fingerprint> {
  static void output(const nextcode::Fingerprint &fp, void *c, raw_ostream &os) {
    os << fp.getRawValue();
  }
  static StringRef input(StringRef s, void *, nextcode::Fingerprint &fp) {
    if (auto convertedFP = nextcode::Fingerprint::fromString(s))
      fp = convertedFP.value();
    else {
      llvm::errs() << "Failed to convert fingerprint '" << s << "'\n";
      exit(1);
    }
    return StringRef();
  }
  static QuotingType mustQuote(StringRef S) { return needsQuotes(S); }
};

} // namespace yaml
} // namespace llvm

LLVM_YAML_DECLARE_MAPPING_TRAITS(
    nextcode::fine_grained_dependencies::SourceFileDepGraph)

namespace llvm {
namespace yaml {
// This introduces a redefinition wherever std::is_same_t<size_t, uint64_t>
// holds.
#if !(defined(__linux__) || defined(_WIN64) || defined(__FreeBSD__))
void ScalarTraits<size_t>::output(const size_t &Val, void *, raw_ostream &out) {
  out << Val;
}

StringRef ScalarTraits<size_t>::input(StringRef scalar, void *ctxt,
                                      size_t &value) {
  return scalar.getAsInteger(10, value) ? "could not parse size_t" : "";
}
#endif

void ScalarEnumerationTraits<nextcode::fine_grained_dependencies::NodeKind>::
    enumeration(IO &io, nextcode::fine_grained_dependencies::NodeKind &value) {
  using NodeKind = nextcode::fine_grained_dependencies::NodeKind;
  io.enumCase(value, "topLevel", NodeKind::topLevel);
  io.enumCase(value, "nominal", NodeKind::nominal);
  io.enumCase(value, "potentialMember", NodeKind::potentialMember);
  io.enumCase(value, "member", NodeKind::member);
  io.enumCase(value, "dynamicLookup", NodeKind::dynamicLookup);
  io.enumCase(value, "externalDepend", NodeKind::externalDepend);
  io.enumCase(value, "sourceFileProvide", NodeKind::sourceFileProvide);
}

void ScalarEnumerationTraits<DeclAspect>::enumeration(
    IO &io, nextcode::fine_grained_dependencies::DeclAspect &value) {
  using DeclAspect = nextcode::fine_grained_dependencies::DeclAspect;
  io.enumCase(value, "interface", DeclAspect::interface);
  io.enumCase(value, "implementation", DeclAspect::implementation);
}

void MappingTraits<DependencyKey>::mapping(
    IO &io, nextcode::fine_grained_dependencies::DependencyKey &key) {
  io.mapRequired("kind", key.kind);
  io.mapRequired("aspect", key.aspect);
  io.mapRequired("context", key.context);
  io.mapRequired("name", key.name);
}

void MappingTraits<DepGraphNode>::mapping(
    IO &io, nextcode::fine_grained_dependencies::DepGraphNode &node) {
  io.mapRequired("key", node.key);
  io.mapOptional("fingerprint", node.fingerprint);
}

void MappingContextTraits<SourceFileDepGraphNode, SourceFileDepGraph>::mapping(
    IO &io, SourceFileDepGraphNode &node, SourceFileDepGraph &g) {
  MappingTraits<DepGraphNode>::mapping(io, node);
  io.mapRequired("sequenceNumber", node.sequenceNumber);
  std::vector<size_t> defsIDependUponVec(node.defsIDependUpon.begin(),
                                         node.defsIDependUpon.end());
  io.mapRequired("defsIDependUpon", defsIDependUponVec);
  io.mapRequired("isProvides", node.isProvides);
  if (!io.outputting()) {
    for (size_t u : defsIDependUponVec)
      node.defsIDependUpon.insert(u);
  }
  assert(g.getNode(node.sequenceNumber) && "Bad sequence number");
}

size_t SequenceTraits<std::vector<SourceFileDepGraphNode *>>::size(
    IO &, std::vector<SourceFileDepGraphNode *> &vec) {
  return vec.size();
}

SourceFileDepGraphNode &
SequenceTraits<std::vector<SourceFileDepGraphNode *>>::element(
    IO &, std::vector<SourceFileDepGraphNode *> &vec, size_t index) {
  while (vec.size() <= index)
    vec.push_back(new SourceFileDepGraphNode());
  return *vec[index];
}

void MappingTraits<SourceFileDepGraph>::mapping(IO &io, SourceFileDepGraph &g) {
  io.mapRequired("allNodes", g.allNodes, g);
}
} // namespace yaml
} // namespace llvm

enum class ActionType : unsigned {
  None,
  BinaryToYAML,
  YAMLToBinary
};

int nextcode_dependency_tool_main(ArrayRef<const char *> argv, void *MainAddr) {
  INITIALIZE_LLVM();

  llvm::cl::OptionCategory Category("nextcode-dependency-tool Options");

  llvm::cl::opt<std::string>
  InputFilename("input-filename",
                llvm::cl::desc("Name of the input file"),
                llvm::cl::cat(Category));

  llvm::cl::opt<std::string>
  OutputFilename("output-filename",
                 llvm::cl::desc("Name of the output file"),
                 llvm::cl::cat(Category));

  llvm::cl::opt<ActionType>
  Action(llvm::cl::desc("Mode:"), llvm::cl::init(ActionType::None),
         llvm::cl::cat(Category),
         llvm::cl::values(
             clEnumValN(ActionType::BinaryToYAML,
                        "to-yaml", "Convert new binary .codedeps format to YAML"),
             clEnumValN(ActionType::YAMLToBinary,
                        "from-yaml", "Convert YAML to new binary .codedeps format")));

  llvm::cl::HideUnrelatedOptions(Category);
  llvm::cl::ParseCommandLineOptions(argv.size(), argv.data(), "NeXTCode Dependency Tool\n");

  SourceManager sourceMgr;
  DiagnosticEngine diags(sourceMgr);
  llvm::vfs::OnDiskOutputBackend outputBackend;

  switch (Action) {
  case ActionType::None: {
    llvm::errs() << "action required\n";
    llvm::cl::PrintHelpMessage();
    return 1;
  }

  case ActionType::BinaryToYAML: {
    auto fg = SourceFileDepGraph::loadFromPath(InputFilename, true);
    if (!fg) {
      llvm::errs() << "Failed to read dependency file\n";
      return 1;
    }

    bool hadError =
      withOutputPath(diags, outputBackend, OutputFilename,
        [&](llvm::raw_pwrite_stream &out) {
          out << "# Fine-grained v0\n";
          llvm::yaml::Output yamlWriter(out);
          yamlWriter << *fg;
          return false;
        });

    if (hadError) {
      llvm::errs() << "Failed to write YAML nextcodedeps\n";
    }
    break;
  }

  case ActionType::YAMLToBinary: {
    auto bufferOrError = llvm::MemoryBuffer::getFile(InputFilename);
    if (!bufferOrError) {
      llvm::errs() << "Failed to read dependency file\n";
      return 1;
    }

    auto &buffer = *bufferOrError.get();

    SourceFileDepGraph fg;
    llvm::yaml::Input yamlReader(llvm::MemoryBufferRef(buffer), nullptr);
    yamlReader >> fg;
    if (yamlReader.error()) {
      llvm::errs() << "Failed to parse YAML nextcodedeps\n";
      return 1;
    }

    if (writeFineGrainedDependencyGraphToPath(
            diags, outputBackend, OutputFilename, fg)) {
      llvm::errs() << "Failed to write binary nextcodedeps\n";
      return 1;
    }

    break;
  }
  }

  return 0;
}