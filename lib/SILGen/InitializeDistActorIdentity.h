//===--- InitializeDistActorIdentity.h - dist actor ID init for SILGen ----===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "Cleanup.h"

namespace nextcode {

namespace Lowering {

/// A clean-up designed to emit an initialization of a distributed actor's
/// identity upon successful initialization of the actor's system.
struct InitializeDistActorIdentity : Cleanup {
private:
  ConstructorDecl *ctor;
  ManagedValue actorSelf;
  VarDecl *systemVar;
public:
  InitializeDistActorIdentity(ConstructorDecl *ctor, ManagedValue actorSelf);

  void emit(SILGenFunction &SGF, CleanupLocation loc,
            ForUnwind_t forUnwind) override;

  void dump(SILGenFunction &) const override;

  VarDecl* getSystemVar() const { return systemVar; }
};

} // end Lowering namespace
} // end nextcode namespace
