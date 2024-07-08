//===- ParseTestSpecification.h - Parsing for test instructions -*- C++ -*-===//
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
//
// This file defines test::FunctionTest.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/Test.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;
using namespace nextcode;
using namespace nextcode::test;

namespace {

class Registry {
  StringMap<FunctionTest> registeredTests;
  NeXTCodeNativeFunctionTestThunk thunk;

public:
  static Registry &get() {
    static Registry registry;
    return registry;
  }

  void registerFunctionTest(FunctionTest test, StringRef name) {
    auto inserted = registeredTests.insert({name, test}).second;
    assert(inserted);
    (void)inserted;
  }

  void registerFunctionTestThunk(NeXTCodeNativeFunctionTestThunk thunk) {
    this->thunk = thunk;
  }

  NeXTCodeNativeFunctionTestThunk getFunctionTestThunk() { return thunk; }

  FunctionTest getFunctionTest(StringRef name) {
    auto iter = registeredTests.find(name);
    if (iter == registeredTests.end()) {
      llvm::errs() << "Found no test named " << name << "!\n";
      print(llvm::errs());
    }
    return iter->getValue();
  }

  void print(raw_ostream &OS) const {
    OS << "test::Registry(" << this << ") with " << registeredTests.size()
       << " entries: {{\n";
    for (auto &stringMapEntry : registeredTests) {
      OS << "\t" << stringMapEntry.getKey() << " -> "
         << &stringMapEntry.getValue() << "\n";
    }
    OS << "}} test::Registry(" << this << ")\n";
  }

  void dump() const { print(llvm::dbgs()); }
};

} // end anonymous namespace

void registerFunctionTestThunk(NeXTCodeNativeFunctionTestThunk thunk) {
  Registry::get().registerFunctionTestThunk(thunk);
}

FunctionTest::FunctionTest(StringRef name, Invocation invocation)
    : invocation(invocation), pass(nullptr), function(nullptr),
      dependencies(nullptr) {
  Registry::get().registerFunctionTest(*this, name);
}
FunctionTest::FunctionTest(StringRef name, NativeNeXTCodeInvocation invocation)
    : invocation(invocation), pass(nullptr), function(nullptr),
      dependencies(nullptr) {}

void FunctionTest::createNativeNeXTCodeFunctionTest(
    StringRef name, NativeNeXTCodeInvocation invocation) {
  Registry::get().registerFunctionTest({name, invocation}, name);
}

FunctionTest FunctionTest::get(StringRef name) {
  return Registry::get().getFunctionTest(name);
}

void FunctionTest::run(SILFunction &function, Arguments &arguments,
                       SILFunctionTransform &pass, Dependencies &dependencies) {
  this->pass = &pass;
  this->function = &function;
  this->dependencies = &dependencies;
  if (invocation.isa<Invocation>()) {
    auto fn = invocation.get<Invocation>();
    fn(function, arguments, *this);
  } else {
    llvm::outs().flush();
    auto *fn = invocation.get<NativeNeXTCodeInvocation>();
    Registry::get().getFunctionTestThunk()(fn, {&function}, {&arguments},
                                           {getNeXTCodePassInvocation()});
    fflush(stdout);
  }
  this->pass = nullptr;
  this->function = nullptr;
  this->dependencies = nullptr;
}

DominanceInfo *FunctionTest::getDominanceInfo() {
  return dependencies->getDominanceInfo();
}

SILPassManager *FunctionTest::getPassManager() {
  return dependencies->getPassManager();
}

NeXTCodePassInvocation *FunctionTest::getNeXTCodePassInvocation() {
  return dependencies->getNeXTCodePassInvocation();
}
