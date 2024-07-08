//===--- Notifications.cpp ------------------------------------------------===//
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

#define DEBUG_TYPE "sil-notifications"

#include "nextcode/SIL/Notifications.h"
#include "nextcode/SIL/SILMoveOnlyDeinit.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace nextcode;

//===----------------------------------------------------------------------===//
// DeserializationNotificationHandler Impl of
// DeserializationNotificationHandlerSet
//===----------------------------------------------------------------------===//

#ifdef DNS_CHAIN_METHOD
#error "DNS_CHAIN_METHOD is defined?!"
#endif
#define DNS_CHAIN_METHOD(Name, FirstTy, SecondTy)                              \
  void DeserializationNotificationHandlerSet::did##Name(FirstTy first,         \
                                                        SecondTy second) {     \
    LLVM_DEBUG(llvm::dbgs()                                                    \
               << "*** Deserialization Notification: " << #Name << " ***\n");  \
    for (auto *p : getRange()) {                                               \
      LLVM_DEBUG(llvm::dbgs()                                                  \
                 << "    Begin Notifying: " << p->getName() << "\n");          \
      p->did##Name(first, second);                                             \
      LLVM_DEBUG(llvm::dbgs()                                                  \
                 << "    End Notifying: " << p->getName() << "\n");            \
    }                                                                          \
    LLVM_DEBUG(llvm::dbgs()                                                    \
               << "*** Completed Deserialization Notifications for " #Name     \
                  "\n");                                                       \
  }
DNS_CHAIN_METHOD(Deserialize, ModuleDecl *, SILFunction *)
DNS_CHAIN_METHOD(DeserializeFunctionBody, ModuleDecl *, SILFunction *)
DNS_CHAIN_METHOD(DeserializeWitnessTableEntries, ModuleDecl *,
                 SILWitnessTable *)
DNS_CHAIN_METHOD(DeserializeDefaultWitnessTableEntries, ModuleDecl *,
                 SILDefaultWitnessTable *)
DNS_CHAIN_METHOD(Deserialize, ModuleDecl *, SILGlobalVariable *)
DNS_CHAIN_METHOD(Deserialize, ModuleDecl *, SILVTable *)
DNS_CHAIN_METHOD(Deserialize, ModuleDecl *, SILMoveOnlyDeinit *)
DNS_CHAIN_METHOD(Deserialize, ModuleDecl *, SILWitnessTable *)
DNS_CHAIN_METHOD(Deserialize, ModuleDecl *, SILDefaultWitnessTable *)
#undef DNS_CHAIN_METHOD
