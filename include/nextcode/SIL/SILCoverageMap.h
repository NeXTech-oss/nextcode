//===--- SILCoverageMap.h - Defines the SILCoverageMap class ----*- C++ -*-===//
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
// This file defines the SILCoverageMap class, which is used to relay coverage
// mapping information from the AST to lower layers of the compiler.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILCOVERAGEMAP_H
#define NEXTCODE_SIL_SILCOVERAGEMAP_H

#include "nextcode/Basic/SourceLoc.h"
#include "nextcode/SIL/SILAllocated.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILPrintContext.h"
#include "llvm/ADT/ilist.h"
#include "llvm/ADT/ilist_node.h"
#include "llvm/ProfileData/Coverage/CoverageMapping.h"

namespace llvm {
namespace coverage {
struct CounterExpression;
struct Counter;
} // namespace coverage
} // namespace llvm

namespace nextcode {

/// A mapping from source locations to expressions made up of profiling
/// counters. This is used to embed information in build products for use with
/// coverage tools later.
class SILCoverageMap : public llvm::ilist_node<SILCoverageMap>,
                       public SILAllocated<SILCoverageMap> {
public:
  class MappedRegion {
  public:
    enum class Kind {
      /// A code region, which represents a regular region of source code.
      Code,

      /// A skipped region, which represents a region that cannot have any
      /// coverage associated with it. This is used for e.g the inactive body of
      /// a \c #if.
      Skipped
    };

    Kind RegionKind;
    unsigned StartLine;
    unsigned StartCol;
    unsigned EndLine;
    unsigned EndCol;
    llvm::coverage::Counter Counter;

  private:
    MappedRegion(Kind RegionKind, unsigned StartLine, unsigned StartCol,
                 unsigned EndLine, unsigned EndCol,
                 llvm::coverage::Counter Counter)
        : RegionKind(RegionKind), StartLine(StartLine), StartCol(StartCol),
          EndLine(EndLine), EndCol(EndCol), Counter(Counter) {}

  public:
    /// A code region, which represents a regular region of source code.
    static MappedRegion code(unsigned StartLine, unsigned StartCol,
                             unsigned EndLine, unsigned EndCol,
                             llvm::coverage::Counter Counter) {
      return MappedRegion(Kind::Code, StartLine, StartCol, EndLine, EndCol,
                          Counter);
    }

    /// A skipped region, which represents a region that cannot have any
    /// coverage associated with it. This is used for e.g the inactive body of
    /// a \c #if.
    static MappedRegion skipped(unsigned StartLine, unsigned StartCol,
                                unsigned EndLine, unsigned EndCol) {
      return MappedRegion(Kind::Skipped, StartLine, StartCol, EndLine, EndCol,
                          llvm::coverage::Counter());
    }

    /// Retrieve the equivalent LLVM mapped region.
    llvm::coverage::CounterMappingRegion getLLVMRegion(unsigned FileID) const;
  };

private:
  /// The parent source file containing the coverage map.
  /// 
  /// NOTE: `ParentSourceFile->getFilename()` is not necessarily equivalent to
  /// `Filename`. `Filename` could be a .code file, and `ParentSourceFile`
  /// could be a parsed .sil file. As such, this should only be used for
  /// determining which object file to emit the coverage map into.
  /// `Filename` should be used for coverage data.
  SourceFile *ParentSourceFile;

  // The name of the source file where this mapping is found.
  StringRef Filename;

  // The mangled name of the function covered by this mapping.
  StringRef Name;

  // The name of this function as recorded in the profile symtab.
  std::string PGOFuncName;

  // The coverage hash of the function covered by this mapping.
  uint64_t Hash;

  // Tail-allocated region mappings.
  MutableArrayRef<MappedRegion> MappedRegions;

  // Tail-allocated expression list.
  MutableArrayRef<llvm::coverage::CounterExpression> Expressions;

  // Disallow copying into temporary objects.
  SILCoverageMap(const SILCoverageMap &other) = delete;
  SILCoverageMap &operator=(const SILCoverageMap &) = delete;

  /// Private constructor. Create these using SILCoverageMap::create.
  SILCoverageMap(SourceFile *ParentSourceFile, uint64_t Hash);

public:
  ~SILCoverageMap();

  static SILCoverageMap *
  create(SILModule &M, SourceFile *ParentSourceFile, StringRef Filename,
         StringRef Name, StringRef PGOFuncName, uint64_t Hash,
         ArrayRef<MappedRegion> MappedRegions,
         ArrayRef<llvm::coverage::CounterExpression> Expressions);

  /// The parent source file containing the coverage map.
  ///
  /// NOTE: `getParentSourceFile()->getFilename()` is not necessarily equivalent
  /// to `getFilename()`. `getFilename()` could be a .code file, and
  /// `getParentSourceFile()` could be a parsed .sil file. As such, this should
  /// only be used for determining which object file to emit the coverage map
  /// in. `getFilename()` should be used for coverage data.
  SourceFile *getParentSourceFile() const { return ParentSourceFile; }

  /// Return the name of the source file where this mapping is found.
  StringRef getFilename() const { return Filename; }

  /// Return the mangled name of the function this mapping covers.
  StringRef getName() const { return Name; }

  /// Return the name of this function as recorded in the profile symtab.
  StringRef getPGOFuncName() const { return PGOFuncName; }

  /// Return the coverage hash for function this mapping covers.
  uint64_t getHash() const { return Hash; }

  /// Return all of the mapped regions.
  ArrayRef<MappedRegion> getMappedRegions() const { return MappedRegions; }

  /// Return all of the counter expressions.
  ArrayRef<llvm::coverage::CounterExpression> getExpressions() const {
    return Expressions;
  }

  /// Print a given profiling counter expression, given the reference to the
  /// counter, and the list of counters it may reference.
  static void
  printCounter(llvm::raw_ostream &OS, llvm::coverage::Counter C,
               ArrayRef<llvm::coverage::CounterExpression> Expressions);

  /// Print a given profiling counter expression.
  void printCounter(llvm::raw_ostream &OS, llvm::coverage::Counter C) const {
    printCounter(OS, C, getExpressions());
  }

  /// Print the coverage map.
  void print(llvm::raw_ostream &OS, bool Verbose = false,
             bool ShouldSort = false) const {
    SILPrintContext Ctx(OS, Verbose, ShouldSort);
    print(Ctx);
  }

  void print(SILPrintContext &PrintCtx) const;

  void dump() const;
};

} // namespace nextcode

namespace llvm {

//===----------------------------------------------------------------------===//
// ilist_traits for SILCoverageMap
//===----------------------------------------------------------------------===//

template <>
struct ilist_traits<::nextcode::SILCoverageMap>
    : public ilist_node_traits<::nextcode::SILCoverageMap> {
  using SILCoverageMap = ::nextcode::SILCoverageMap;

public:
  static void deleteNode(SILCoverageMap *VT) { VT->~SILCoverageMap(); }

private:
  void createNode(const SILCoverageMap &);
};

} // namespace llvm

#endif // NEXTCODE_SIL_SILCOVERAGEMAP_H
