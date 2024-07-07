//===- GenericSpecializationMangler.h - generic specializations -*- C++ -*-===//
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

#ifndef NEXTCODE_SIL_UTILS_GENERICSPECIALIZATIONMANGLER_H
#define NEXTCODE_SIL_UTILS_GENERICSPECIALIZATIONMANGLER_H

#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/Effects.h"
#include "nextcode/Basic/NullablePtr.h"
#include "nextcode/Demangling/Demangler.h"
#include "nextcode/SIL/SILFunction.h"

namespace nextcode {
namespace Mangle {

enum class SpecializationKind : uint8_t {
  Generic,
  NotReAbstractedGeneric,
  FunctionSignature,
};

/// Inject SpecializationPass into the Mangle namespace.
using SpecializationPass = Demangle::SpecializationPass;

/// The base class for specialization mangles.
class SpecializationMangler : public Mangle::ASTMangler {
protected:
  /// The specialization pass.
  SpecializationPass Pass;

  nextcode::SerializedKind_t Serialized;

  /// The original function which is specialized.
  SILFunction *Function;
  std::string FunctionName;

  llvm::SmallVector<char, 32> ArgOpStorage;
  llvm::raw_svector_ostream ArgOpBuffer;

  // Effects that are removed from the original function in this specialization.
  PossibleEffects RemovedEffects;

protected:
  SpecializationMangler(SpecializationPass P, nextcode::SerializedKind_t Serialized,
                        SILFunction *F)
      : Pass(P), Serialized(Serialized), Function(F),
        ArgOpBuffer(ArgOpStorage) {}

  SpecializationMangler(SpecializationPass P, nextcode::SerializedKind_t Serialized,
                        std::string functionName)
      : Pass(P), Serialized(Serialized), Function(nullptr),
        FunctionName(functionName), ArgOpBuffer(ArgOpStorage) {}

  void beginMangling();

  /// Finish the mangling of the symbol and return the mangled name.
  std::string finalize();

  void appendSpecializationOperator(StringRef Op) {
    appendOperator(Op, StringRef(ArgOpStorage.data(), ArgOpStorage.size()));
  }
};

// The mangler for specialized generic functions.
class GenericSpecializationMangler : public SpecializationMangler {

  GenericSpecializationMangler(std::string origFuncName)
      : SpecializationMangler(SpecializationPass::GenericSpecializer,
                              IsNotSerialized, origFuncName) {}

  GenericSignature getGenericSignature() {
    assert(Function && "Need a SIL function to get a generic signature");
    return Function->getLoweredFunctionType()->getInvocationGenericSignature();
  }

  void appendSubstitutions(GenericSignature sig, SubstitutionMap subs);

  std::string manglePrespecialized(GenericSignature sig,
                                      SubstitutionMap subs);

public:
  GenericSpecializationMangler(SILFunction *F, nextcode::SerializedKind_t Serialized)
      : SpecializationMangler(SpecializationPass::GenericSpecializer,
                              Serialized, F) {}

  std::string mangleNotReabstracted(SubstitutionMap subs,
                                    bool metatyeParamsRemoved);

  /// Mangle a generic specialization with re-abstracted parameters.
  ///
  /// Re-abstracted means that indirect (generic) parameters/returns are
  /// converted to direct parameters/returns.
  ///
  /// This is the default for generic specializations.
  ///
  /// \param alternativeMangling   true for specialized functions with a
  ///                              different resilience expansion.
  /// \param metatyeParamsRemoved  true if non-generic metatype parameters are
  ///                              removed in the specialized function.
  std::string mangleReabstracted(SubstitutionMap subs, bool alternativeMangling,
                                 bool metatyeParamsRemoved = false);

  std::string mangleForDebugInfo(GenericSignature sig, SubstitutionMap subs,
                                 bool forInlining);

  std::string manglePrespecialized(SubstitutionMap subs) {
    return manglePrespecialized(getGenericSignature(), subs);
  }
                                    
  static std::string manglePrespecialization(std::string unspecializedName,
                                             GenericSignature genericSig,
                                             GenericSignature specializedSig);
};

} // end namespace Mangle
} // end namespace nextcode

#endif
