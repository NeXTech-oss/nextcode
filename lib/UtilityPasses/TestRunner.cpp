//===-------------------------- TestRunner.cpp ----------------------------===//
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
// This file defines test::TestRunner, the pass responsible for running tests,
// specifically test::FunctionTest and test::ModuleTest (maybe someday).
//
// To see more about writing your own tests, see include/nextcode/SIL/Test.h.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/Test.h"
#include "nextcode/SILOptimizer/Analysis/DominanceAnalysis.h"
#include "nextcode/SILOptimizer/PassManager/Transforms.h"

using namespace nextcode;
using namespace nextcode::test;

namespace nextcode::test {
class TestRunner : public SILFunctionTransform {
  void printTestLifetime(bool begin, unsigned testIndex, unsigned testCount,
                         StringRef name, ArrayRef<StringRef> components);
  void runTest(StringRef name, Arguments &arguments);
  void run() override;
  struct FunctionTestDependenciesImpl final
      : public test::FunctionTest::Dependencies {
    TestRunner *pass;
    SILFunction *function;
    NeXTCodePassInvocation nextcodePassInvocation;
    FunctionTestDependenciesImpl(TestRunner *pass, SILFunction *function)
        : pass(pass), function(function),
          nextcodePassInvocation(pass->getPassManager(), pass, function) {}
    DominanceInfo *getDominanceInfo() override {
      auto *dominanceAnalysis = pass->getAnalysis<DominanceAnalysis>();
      return dominanceAnalysis->get(function);
    }
    NeXTCodePassInvocation *getNeXTCodePassInvocation() override {
      return &nextcodePassInvocation;
    }
    SILPassManager *getPassManager() override { return pass->getPassManager(); }
    ~FunctionTestDependenciesImpl() {}
  };
};

void TestRunner::printTestLifetime(bool begin, unsigned testIndex,
                                   unsigned testCount, StringRef name,
                                   ArrayRef<StringRef> components) {
  StringRef word = begin ? "\nbegin" : "end";
  llvm::outs() << word << " running test " << testIndex + 1 << " of "
               << testCount << " on " << getFunction()->getName() << ": "
               << name << " with: ";
  for (unsigned long index = 0, size = components.size(); index < size;
       ++index) {
    auto componentString = components[index].trim();
    if (componentString.empty())
      continue;

    llvm::outs() << componentString;
    if (index != size - 1) {
      llvm::outs() << ", ";
    }
  }
  llvm::outs() << "\n";
}

void TestRunner::runTest(StringRef name, Arguments &arguments) {
  FunctionTest test = FunctionTest::get(name);
  auto *function = getFunction();
  FunctionTestDependenciesImpl dependencies(this, function);
  test.run(*function, arguments, *this, dependencies);
}

void TestRunner::run() {
  llvm::SmallVector<UnparsedSpecification, 2> testSpecifications;
  getTestSpecifications(getFunction(), testSpecifications);
  Arguments arguments;
  SmallVector<StringRef, 4> components;
  for (unsigned long index = 0, size = testSpecifications.size(); index < size;
       ++index) {
    components.clear();
    arguments.clear();
    auto testSpecification = testSpecifications[index];
    test::parseTestArgumentsFromSpecification(getFunction(), testSpecification,
                                              arguments, components);
    auto name = arguments.takeString();
    ArrayRef<StringRef> argumentStrings = components;
    argumentStrings = argumentStrings.drop_front();
    printTestLifetime(/*begin=*/true, /*index=*/index, /*size=*/size, name,
                      argumentStrings);
    runTest(name, arguments);
    printTestLifetime(/*begin=*/false, /*index=*/index, /*size=*/size, name,
                      argumentStrings);
  }
}

//===----------------------------------------------------------------------===//
// MARK: General Unit Tests
//===----------------------------------------------------------------------===//

// Arguments: NONE
// Dumps:
// - the function
static FunctionTest DumpFunctionTest("dump-function",
                                     [](auto &function, auto &, auto &) {
                                       function.print(llvm::outs());
                                     });

} // namespace nextcode::test

//===----------------------------------------------------------------------===//
//                           Top Level Entry Point
//===----------------------------------------------------------------------===//

SILTransform *nextcode::createUnitTestRunner() { return new TestRunner(); }
