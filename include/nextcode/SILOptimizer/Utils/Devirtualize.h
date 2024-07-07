//===--- Devirtualize.h - Helper for devirtualizing apply -------*- C++ -*-===//
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
// This contains helper functions that perform the work of devirtualizing a
// given apply when possible.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SIL_DEVIRTUALIZE_H
#define NEXTCODE_SIL_DEVIRTUALIZE_H

#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Types.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "nextcode/SIL/SILFunction.h"
#include "nextcode/SIL/SILInstruction.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILType.h"
#include "nextcode/SIL/SILValue.h"
#include "nextcode/SILOptimizer/Analysis/ClassHierarchyAnalysis.h"
#include "nextcode/SILOptimizer/Utils/InstOptUtils.h"
#include "llvm/ADT/ArrayRef.h"

namespace nextcode {
namespace OptRemark {
class Emitter;
}

/// Compute all subclasses of a given class.
///
/// \p CHA class hierarchy analysis
/// \p CD class declaration
/// \p ClassType type of the instance
/// \p M SILModule
/// \p Subs a container to be used for storing the set of subclasses
void getAllSubclasses(ClassHierarchyAnalysis *CHA,
                      ClassDecl *CD,
                      CanType ClassType,
                      SILModule &M,
                      ClassHierarchyAnalysis::ClassList &Subs);

/// Given an apply instruction of a protocol requirement and a witness method
/// for the requirement, compute a substitution suitable for a direct call
/// to the witness method.
///
/// \p Module SILModule
/// \p AI ApplySite that applies a protocol method
/// \p F SILFunction with convention @convention(witness_method)
/// \p CRef a concrete ProtocolConformanceRef
SubstitutionMap getWitnessMethodSubstitutions(SILModule &Module, ApplySite AI,
                                              SILFunction *F,
                                              ProtocolConformanceRef CRef);

/// Attempt to devirtualize the given apply site.  If this fails,
/// the returned ApplySite will be null.
///
/// If this succeeds, the caller must call deleteDevirtualizedApply on
/// the original apply site.
///
/// Return the new apply and true if the CFG was also modified.
std::pair<ApplySite, bool>
tryDevirtualizeApply(SILPassManager *pm, ApplySite AI, ClassHierarchyAnalysis *CHA,
                     OptRemark::Emitter *ORE = nullptr,
                     bool isMandatory = false);
bool canDevirtualizeApply(FullApplySite AI, ClassHierarchyAnalysis *CHA);
bool canDevirtualizeClassMethod(FullApplySite AI, ClassDecl *CD,
                                CanType ClassType,
                                OptRemark::Emitter *ORE = nullptr,
                                bool isEffectivelyFinalMethod = false);
SILFunction *getTargetClassMethod(SILModule &M, ClassDecl *CD,
                                  CanType ClassType, MethodInst *MI);
CanType getSelfInstanceType(CanType ClassOrMetatypeType);

/// Devirtualize the given apply site, which is known to be devirtualizable.
///
/// The caller must call deleteDevirtualizedApply on the original apply site.
///
/// Return the new apply and true if the CFG was also modified.
std::pair<FullApplySite, bool>
devirtualizeClassMethod(SILPassManager *pm,
                        FullApplySite AI, SILValue ClassInstance, ClassDecl *CD,
                        CanType classType, OptRemark::Emitter *ORE);

/// Attempt to devirtualize the given apply site, which is known to be
/// of a class method.  If this fails, the returned FullApplySite will be null.
///
/// If this succeeds, the caller must call deleteDevirtualizedApply on
/// the original apply site.
///
/// Return the new apply and true if the CFG was also modified.
std::pair<FullApplySite, bool>
tryDevirtualizeClassMethod(SILPassManager *pm,
                           FullApplySite AI, SILValue ClassInstance,
                           ClassDecl *CD, CanType ClassType,
                           OptRemark::Emitter *ORE,
                           bool isEffectivelyFinalMethod = false);

/// Attempt to devirtualize the given apply site, which is known to be
/// of a witness method.  If this fails, the returned FullApplySite
/// will be null.
///
/// If this succeeds, the caller must call deleteDevirtualizedApply on
/// the original apply site.
///
/// Return the new apply and true if the CFG was also modified.
std::pair<ApplySite, bool> tryDevirtualizeWitnessMethod(SILPassManager *pm,
                                                        ApplySite AI,
                                                        OptRemark::Emitter *ORE,
                                                        bool isMandatory);

/// Delete a successfully-devirtualized apply site.  This must always be
/// called after devirtualizing an apply; not only is it not semantically
/// equivalent to leave the old apply in-place, but the SIL isn't necessary
/// well-formed.
///
/// Devirtualization is responsible for replacing uses of the original
/// apply site with uses of the new one.  The only thing this does is delete
/// the instruction and any now-trivially-dead operands; it is separated
/// from the actual devirtualization step only to apply the caller to log
/// information about the original apply site.  This is probably not a
/// good enough reason to complicate the API.
void deleteDevirtualizedApply(ApplySite AI);

} // end namespace nextcode

#endif
