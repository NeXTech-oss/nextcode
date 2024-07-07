//===-- EntryPointArgumentEmission.h - Emit function entries. -------------===//
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

#pragma once

namespace llvm {
class Value;
}

namespace nextcode {

class GenericRequirement;
class SILArgument;

namespace irgen {

class Explosion;
class LoadableTypeInfo;
class TypeInfo;

class EntryPointArgumentEmission {

public:
  virtual ~EntryPointArgumentEmission() {}
  virtual bool requiresIndirectResult(SILType retType) = 0;
  virtual llvm::Value *getIndirectResultForFormallyDirectResult() = 0;
  virtual llvm::Value *getIndirectResult(unsigned index) = 0;
  virtual llvm::Value *getNextPolymorphicParameterAsMetadata() = 0;
  virtual llvm::Value *
  getNextPolymorphicParameter(GenericRequirement &requirement) = 0;
};

class NativeCCEntryPointArgumentEmission
    : public virtual EntryPointArgumentEmission {

public:
  virtual void mapAsyncParameters() = 0;
  virtual llvm::Value *getCallerErrorResultArgument() = 0;
  virtual llvm::Value *getCallerTypedErrorResultArgument() = 0;
  virtual llvm::Value *getContext() = 0;
  virtual Explosion getArgumentExplosion(unsigned index, unsigned size) = 0;
  virtual llvm::Value *getSelfWitnessTable() = 0;
  virtual llvm::Value *getSelfMetadata() = 0;
  virtual llvm::Value *getCoroutineBuffer() = 0;
  virtual Explosion
  explosionForObject(IRGenFunction &IGF, unsigned index, SILArgument *param,
                     SILType paramTy, const LoadableTypeInfo &loadableParamTI,
                     const LoadableTypeInfo &loadableArgTI,
                     std::function<Explosion(unsigned index, unsigned size)>
                         explosionForArgument) = 0;
};

} // end namespace irgen
} // end namespace nextcode
