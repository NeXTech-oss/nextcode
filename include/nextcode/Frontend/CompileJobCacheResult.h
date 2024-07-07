//===------CompileJobCacheResult.h-- -----------------------------*-C++ -*-===//
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

#ifndef NEXTCODE_FRONTEND_COMPILEJOBCACHERESULT_H
#define NEXTCODE_FRONTEND_COMPILEJOBCACHERESULT_H

#include "nextcode/Basic/FileTypes.h"
#include "llvm/CAS/CASNodeSchema.h"
#include "llvm/CAS/ObjectStore.h"

namespace nextcode::cas {

class CompileJobResultSchema;
class CompileJobCacheResult : public llvm::cas::ObjectProxy {
public:
  /// A single output file or stream.
  struct Output {
    /// The CAS object for this output.
    llvm::cas::ObjectRef Object;
    /// The output kind.
    file_types::ID Kind;

    bool operator==(const Output &Other) const {
      return Object == Other.Object && Kind == Other.Kind;
    }
  };

  /// Retrieves each \c Output from this result.
  llvm::Error
  forEachOutput(llvm::function_ref<llvm::Error(Output)> Callback) const;

  /// Loads all outputs concurrently and passes the resulting \c ObjectProxy
  /// objects to \p Callback. If there was an error during loading then the
  /// callback will not be invoked.
  llvm::Error forEachLoadedOutput(
      llvm::function_ref<llvm::Error(Output, std::optional<ObjectProxy>)>
          Callback);

  size_t getNumOutputs() const;

  Output getOutput(size_t I) const;

  /// Retrieves a specific output specified by \p Kind, if it exists.
  std::optional<Output> getOutput(file_types::ID Kind) const;

  /// Print this result to \p OS.
  llvm::Error print(llvm::raw_ostream &OS);

  /// Helper to build a \c CompileJobCacheResult from individual outputs.
  class Builder {
  public:
    Builder();
    ~Builder();
    /// Treat outputs added for \p Path as having the given \p Kind. Otherwise
    /// they will have kind \c Unknown.
    void addKindMap(file_types::ID Kind, StringRef Path);
    /// Add an output with an explicit \p Kind.
    void addOutput(file_types::ID Kind, llvm::cas::ObjectRef Object);
    /// Add an output for the given \p Path. There must be a a kind map for it.
    llvm::Error addOutput(StringRef Path, llvm::cas::ObjectRef Object);
    /// Build a single \c ObjectRef representing the provided outputs. The
    /// result can be used with \c CompileJobResultSchema to retrieve the
    /// original outputs.
    llvm::Expected<llvm::cas::ObjectRef> build(llvm::cas::ObjectStore &CAS);

  private:
    struct PrivateImpl;
    PrivateImpl &Impl;
  };

private:
  llvm::cas::ObjectRef getOutputObject(size_t I) const;
  llvm::cas::ObjectRef getPathsListRef() const;
  file_types::ID getOutputKind(size_t I) const;
  llvm::Expected<llvm::cas::ObjectRef> getOutputPath(size_t I) const;

private:
  friend class CompileJobResultSchema;
  CompileJobCacheResult(const ObjectProxy &);
};

class CompileJobResultSchema
    : public llvm::RTTIExtends<CompileJobResultSchema, llvm::cas::NodeSchema> {
public:
  static char ID;

  CompileJobResultSchema(llvm::cas::ObjectStore &CAS);

  /// Attempt to load \p Ref as a \c CompileJobCacheResult if it matches the
  /// schema.
  llvm::Expected<CompileJobCacheResult> load(llvm::cas::ObjectRef Ref) const;

  bool isRootNode(const CompileJobCacheResult::ObjectProxy &Node) const final;
  bool isNode(const CompileJobCacheResult::ObjectProxy &Node) const final;

  /// Get this schema's marker node.
  llvm::cas::ObjectRef getKindRef() const { return KindRef; }

private:
  llvm::cas::ObjectRef KindRef;
};

} // namespace nextcode::cas

#endif
