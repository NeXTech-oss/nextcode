//===--- SILDebuggerClient.h - Interfaces from SILGen to LLDB ---*- C++ -*-===//
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
// This file defines the abstract SILDebuggerClient class.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SILDEBUGGERCLIENT_H
#define NEXTCODE_SILDEBUGGERCLIENT_H

#include "nextcode/AST/DebuggerClient.h"
#include "nextcode/SIL/SILLocation.h"
#include "nextcode/SIL/SILValue.h"

namespace nextcode {

class SILBuilder;

class SILDebuggerClient : public DebuggerClient {
public:
  using ResultVector = SmallVectorImpl<LookupResultEntry>;

  SILDebuggerClient(ASTContext &C) : DebuggerClient(C) { }
  virtual ~SILDebuggerClient() = default;

  /// DebuggerClient is asked to emit SIL references to locals,
  /// permitting SILGen to access them like any other variables.
  /// This avoids generation of properties.
  virtual SILValue emitLValueForVariable(VarDecl *var,
                                         SILBuilder &builder) = 0;

  inline SILDebuggerClient *getAsSILDebuggerClient() override {
    return this;
  }
private:
  virtual void anchor() override;
};

} // namespace nextcode

#endif
