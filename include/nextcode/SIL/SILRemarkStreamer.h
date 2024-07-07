//===--- SILRemarkStreamer.h - Interface for streaming remarks --*- C++ -*-===//
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
/// \file
/// This file defines the interface used to stream SIL optimization diagnostics
/// through LLVM's RemarkStreamer interface.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_SILREMARKSTREAMER_H
#define NEXTCODE_SIL_SILREMARKSTREAMER_H

#include "nextcode/Basic/SourceManager.h"
#include "nextcode/SIL/OptimizationRemark.h"
#include "llvm/Remarks/Remark.h"
#include "llvm/Remarks/RemarkStreamer.h"

namespace nextcode {

class SILRemarkStreamer {
private:
  enum class Owner {
    SILModule,
    LLVM,
  } owner;

  /// The underlying LLVM streamer.
  ///
  /// If owned by a SILModule, this will be non-null.
  std::unique_ptr<llvm::remarks::RemarkStreamer> streamer;
  /// The owning LLVM context.
  ///
  /// If owned by LLVM, this will be non-null.
  llvm::LLVMContext *context;

  /// The remark output stream used to record SIL remarks to a file.
  std::unique_ptr<llvm::raw_fd_ostream> remarkStream;

  // Source manager for resolving source locations.
  const ASTContext &ctx;
  /// Convert diagnostics into LLVM remark objects.
  /// The lifetime of the members of the result is bound to the lifetime of
  /// the SIL remarks.
  template <typename RemarkT>
  llvm::remarks::Remark
  toLLVMRemark(const OptRemark::Remark<RemarkT> &remark) const;

  SILRemarkStreamer(std::unique_ptr<llvm::remarks::RemarkStreamer> &&streamer,
                    std::unique_ptr<llvm::raw_fd_ostream> &&stream,
                    const ASTContext &Ctx);

public:
  static std::unique_ptr<SILRemarkStreamer> create(SILModule &silModule);

public:
  llvm::remarks::RemarkStreamer &getLLVMStreamer();
  const llvm::remarks::RemarkStreamer &getLLVMStreamer() const;

  const ASTContext &getASTContext() const { return ctx; }

public:
  /// Perform a one-time ownership transfer to associate the underlying
  /// \c llvm::remarks::RemarkStreamer with the given \c LLVMContext.
  void intoLLVMContext(llvm::LLVMContext &Ctx) &;

public:
  /// Emit a remark through the streamer.
  template <typename RemarkT>
  void emit(const OptRemark::Remark<RemarkT> &remark);
};

// Implementation for template member functions.

// OptRemark type -> llvm::remarks::Type
template <typename RemarkT> static llvm::remarks::Type toRemarkType() {
  if (std::is_same<RemarkT, OptRemark::RemarkPassed>::value)
    return llvm::remarks::Type::Passed;
  if (std::is_same<RemarkT, OptRemark::RemarkMissed>::value)
    return llvm::remarks::Type::Missed;
  llvm_unreachable("Unknown remark type");
}

static inline std::optional<llvm::remarks::RemarkLocation>
toRemarkLocation(const SourceLoc &loc, const SourceManager &srcMgr) {
  if (!loc.isValid())
    return std::nullopt;

  StringRef file = srcMgr.getDisplayNameForLoc(loc);
  unsigned line, col;
  std::tie(line, col) = srcMgr.getPresumedLineAndColumnForLoc(loc);
  return llvm::remarks::RemarkLocation{file, line, col};
}

template <typename RemarkT>
llvm::remarks::Remark SILRemarkStreamer::toLLVMRemark(
    const OptRemark::Remark<RemarkT> &optRemark) const {
  llvm::remarks::Remark llvmRemark; // The result.
  llvmRemark.RemarkType = toRemarkType<RemarkT>();
  llvmRemark.PassName = optRemark.getPassName();
  llvmRemark.RemarkName = optRemark.getIdentifier();
  llvmRemark.FunctionName = optRemark.getDemangledFunctionName();
  llvmRemark.Loc =
      toRemarkLocation(optRemark.getLocation(), getASTContext().SourceMgr);

  for (const OptRemark::Argument &arg : optRemark.getArgs()) {
    llvmRemark.Args.emplace_back();
    llvmRemark.Args.back().Key = arg.key.data;
    llvmRemark.Args.back().Val = arg.val;
    llvmRemark.Args.back().Loc =
        toRemarkLocation(arg.loc, getASTContext().SourceMgr);
  }

  return llvmRemark;
}

template <typename RemarkT>
void SILRemarkStreamer::emit(const OptRemark::Remark<RemarkT> &optRemark) {
  if (!getLLVMStreamer().matchesFilter(optRemark.getPassName()))
    return;

  return getLLVMStreamer().getSerializer().emit(toLLVMRemark(optRemark));
}

} // namespace nextcode
#endif