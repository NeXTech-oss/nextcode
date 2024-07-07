//===--- PrettyStackTrace.h - Crash trace information -----------*- C++ -*-===//
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
// This file defines SIL-specific RAII classes that give better diagnostic
// output about when, exactly, a crash is occurring.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_PRETTYSTACKTRACE_H
#define NEXTCODE_SIL_PRETTYSTACKTRACE_H

#include "nextcode/SIL/SILLocation.h"
#include "nextcode/SIL/SILNode.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/Twine.h"
#include "llvm/Support/PrettyStackTrace.h"

namespace nextcode {
class ASTContext;
class SILFunction;

void printSILLocationDescription(llvm::raw_ostream &out, SILLocation loc,
                                 ASTContext &ctx);

/// PrettyStackTraceLocation - Observe that we are doing some
/// processing starting at a SIL location.
class PrettyStackTraceSILLocation : public llvm::PrettyStackTraceEntry {
  SILLocation Loc;
  const char *Action;
  ASTContext &Context;
public:
  PrettyStackTraceSILLocation(const char *action, SILLocation loc,
                              ASTContext &C)
    : Loc(loc), Action(action), Context(C) {}
  virtual void print(llvm::raw_ostream &OS) const override;
};


/// Observe that we are doing some processing of a SIL function.
class PrettyStackTraceSILFunction : public llvm::PrettyStackTraceEntry {
  const SILFunction *func;

  /// An inline buffer of characters used if we are passed a twine.
  SmallString<256> data;

  /// This points either at a user provided const char * string or points at the
  /// inline message buffer that is initialized with data from a twine on
  /// construction.
  StringRef action;

public:
  PrettyStackTraceSILFunction(const char *action, const SILFunction *func)
      : func(func), data(), action(action) {}

  PrettyStackTraceSILFunction(llvm::Twine &&twine, const SILFunction *func)
      : func(func), data(), action(twine.toNullTerminatedStringRef(data)) {}

  virtual void print(llvm::raw_ostream &os) const override;

protected:
  void printFunctionInfo(llvm::raw_ostream &out) const;
};

/// Observe that we are visiting SIL nodes.
class PrettyStackTraceSILNode : public llvm::PrettyStackTraceEntry {
  const SILNode *Node;
  const char *Action;

public:
  PrettyStackTraceSILNode(const char *action, SILNodePointer node)
    : Node(node), Action(action) {}

  virtual void print(llvm::raw_ostream &OS) const override;
};

} // end namespace nextcode

#endif
