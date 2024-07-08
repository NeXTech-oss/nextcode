//===--- SILOptimizerRequests.cpp - Requests for SIL Optimization  --------===//
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

#include "nextcode/AST/SILOptimizerRequests.h"
#include "nextcode/AST/Evaluator.h"
#include "nextcode/SILOptimizer/PassManager/PassPipeline.h"
#include "nextcode/Subsystems.h"
#include "llvm/ADT/Hashing.h"

using namespace nextcode;

namespace nextcode {
// Implement the SILOptimizer type zone (zone 13).
#define NEXTCODE_TYPEID_ZONE SILOptimizer
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/SILOptimizerTypeIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
} // end namespace nextcode

//----------------------------------------------------------------------------//
// ExecuteSILPipelineRequest computation.
//----------------------------------------------------------------------------//

bool SILPipelineExecutionDescriptor::
operator==(const SILPipelineExecutionDescriptor &other) const {
  return SM == other.SM && Plan == other.Plan &&
         IsMandatory == other.IsMandatory && IRMod == other.IRMod;
}

llvm::hash_code nextcode::hash_value(const SILPipelineExecutionDescriptor &desc) {
  return llvm::hash_combine(desc.SM, desc.Plan, desc.IsMandatory, desc.IRMod);
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const SILPipelineExecutionDescriptor &desc) {
  out << "Run pipelines { ";
  interleave(
      desc.Plan.getPipelines(),
      [&](SILPassPipeline stage) { out << stage.Name; },
      [&]() { out << ", "; });
  out << " } on ";
  simple_display(out, desc.SM);
}

SourceLoc
nextcode::extractNearestSourceLoc(const SILPipelineExecutionDescriptor &desc) {
  return extractNearestSourceLoc(desc.SM);
}

//----------------------------------------------------------------------------//
// LoweredSILRequest computation.
//----------------------------------------------------------------------------//

std::unique_ptr<SILModule>
LoweredSILRequest::evaluate(Evaluator &evaluator,
                            ASTLoweringDescriptor desc) const {
  auto silMod = evaluateOrFatal(evaluator, ASTLoweringRequest{desc});
  silMod->installSILRemarkStreamer();
  silMod->setSerializeSILAction([]() {});

  runSILDiagnosticPasses(*silMod);

  {
    FrontendStatsTracer tracer(silMod->getASTContext().Stats,
                               "SIL verification, pre-optimization");
    silMod->verify();
  }

  runSILOptimizationPasses(*silMod);

  {
    FrontendStatsTracer tracer(silMod->getASTContext().Stats,
                               "SIL verification, post-optimization");
    silMod->verify();
  }

  runSILLoweringPasses(*silMod);
  return silMod;
}

// Define request evaluation functions for each of the SILGen requests.
static AbstractRequestFunction *silOptimizerRequestFunctions[] = {
#define NEXTCODE_REQUEST(Zone, Name, Sig, Caching, LocOptions)                    \
  reinterpret_cast<AbstractRequestFunction *>(&Name::evaluateRequest),
#include "nextcode/AST/SILOptimizerTypeIDZone.def"
#undef NEXTCODE_REQUEST
};

void nextcode::registerSILOptimizerRequestFunctions(Evaluator &evaluator) {
  evaluator.registerRequestFunctions(Zone::SILOptimizer,
                                     silOptimizerRequestFunctions);
}
