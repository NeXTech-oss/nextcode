//===--- SILGenRequests.h - SILGen Requests ---------------------*- C++ -*-===//
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
//  This file defines SILGen requests.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILGEN_REQUESTS_H
#define NEXTCODE_SILGEN_REQUESTS_H

#include "nextcode/AST/ASTTypeIDs.h"
#include "nextcode/AST/EvaluatorDependencies.h"
#include "nextcode/AST/SimpleRequest.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TBDGenRequests.h"
#include "nextcode/SIL/SILDeclRef.h"

namespace nextcode {

class LangOptions;
class ModuleDecl;
class SILModule;
class SILOptions;
class IRGenOptions;

namespace Lowering {
  class TypeConverter;
}

/// Report that a request of the given kind is being evaluated, so it
/// can be recorded by the stats reporter.
template<typename Request>
void reportEvaluatedRequest(UnifiedStatsReporter &stats,
                            const Request &request);

using SILRefsToEmit = llvm::SmallVector<SILDeclRef, 1>;

using SymbolSources = llvm::SmallVector<SymbolSource, 1>;

/// Describes a file or module to be lowered to SIL.
struct ASTLoweringDescriptor {
  llvm::PointerUnion<FileUnit *, ModuleDecl *> context;
  Lowering::TypeConverter &conv;
  const SILOptions &opts;
  const IRGenOptions *irgenOptions;

  /// A specific set of SILDeclRefs to emit. If set, only these refs will be
  /// emitted. Otherwise the entire \c context will be emitted.
  std::optional<SymbolSources> SourcesToEmit;

  friend llvm::hash_code hash_value(const ASTLoweringDescriptor &owner) {
    return llvm::hash_combine(owner.context, (void *)&owner.conv,
                              (void *)&owner.opts, owner.SourcesToEmit);
  }

  friend bool operator==(const ASTLoweringDescriptor &lhs,
                         const ASTLoweringDescriptor &rhs) {
    return lhs.context == rhs.context && &lhs.conv == &rhs.conv &&
           &lhs.opts == &rhs.opts && lhs.SourcesToEmit == rhs.SourcesToEmit;
  }

  friend bool operator!=(const ASTLoweringDescriptor &lhs,
                         const ASTLoweringDescriptor &rhs) {
    return !(lhs == rhs);
  }

public:
  static ASTLoweringDescriptor
  forFile(FileUnit &sf, Lowering::TypeConverter &conv, const SILOptions &opts,
          std::optional<SymbolSources> SourcesToEmit = std::nullopt,
          const IRGenOptions *irgenOptions = nullptr) {
    return ASTLoweringDescriptor{&sf, conv, opts, irgenOptions,
                                 std::move(SourcesToEmit)};
  }

  static ASTLoweringDescriptor
  forWholeModule(ModuleDecl *mod, Lowering::TypeConverter &conv,
                 const SILOptions &opts,
                 std::optional<SymbolSources> SourcesToEmit = std::nullopt,
                 const IRGenOptions *irgenOptions = nullptr) {
    return ASTLoweringDescriptor{mod, conv, opts, irgenOptions,
                                 std::move(SourcesToEmit)};
  }

  /// Retrieves the files to generate SIL for. If the descriptor is configured
  /// only to emit a specific set of SILDeclRefs, this will be empty.
  ArrayRef<FileUnit *> getFilesToEmit() const;

  /// If the module or file contains SIL that needs parsing, returns the file
  /// to be parsed, or \c nullptr if parsing isn't required.
  SourceFile *getSourceFileToParse() const;
};

void simple_display(llvm::raw_ostream &out, const ASTLoweringDescriptor &d);

SourceLoc extractNearestSourceLoc(const ASTLoweringDescriptor &desc);

/// Lowers a file or module to SIL. In most cases this involves transforming
/// a file's AST into SIL, through SILGen. However it can also handle files
/// containing SIL in textual or binary form, which will be parsed or
/// deserialized as needed.
class ASTLoweringRequest
    : public SimpleRequest<
          ASTLoweringRequest, std::unique_ptr<SILModule>(ASTLoweringDescriptor),
          RequestFlags::Uncached | RequestFlags::DependencySource> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  std::unique_ptr<SILModule> evaluate(Evaluator &evaluator,
                                      ASTLoweringDescriptor desc) const;

public:
  // Incremental dependencies.
  evaluator::DependencySource
  readDependencySource(const evaluator::DependencyRecorder &) const;
};

/// Parses a .sil file into a SILModule.
class ParseSILModuleRequest
    : public SimpleRequest<ParseSILModuleRequest,
                           std::unique_ptr<SILModule>(ASTLoweringDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  std::unique_ptr<SILModule> evaluate(Evaluator &evaluator,
                                      ASTLoweringDescriptor desc) const;
};

/// The zone number for SILGen.
#define NEXTCODE_TYPEID_ZONE SILGen
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/SILGenTypeIDZone.def"
#include "nextcode/Basic/DefineTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER

 // Set up reporting of evaluated requests.
#define NEXTCODE_REQUEST(Zone, RequestType, Sig, Caching, LocOptions)             \
template<>                                                                     \
inline void reportEvaluatedRequest(UnifiedStatsReporter &stats,                \
                            const RequestType &request) {                      \
  ++stats.getFrontendCounters().RequestType;                                   \
}
#include "nextcode/AST/SILGenTypeIDZone.def"
#undef NEXTCODE_REQUEST

} // end namespace nextcode

#endif // NEXTCODE_SILGEN_REQUESTS_H