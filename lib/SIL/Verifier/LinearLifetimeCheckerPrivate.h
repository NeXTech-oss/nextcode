//===--- LinearLifetimeCheckerPrivate.h -----------------------------------===//
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

#ifndef NEXTCODE_SIL_LINEARLIFETIMECHECKER_PRIVATE_H
#define NEXTCODE_SIL_LINEARLIFETIMECHECKER_PRIVATE_H

#include "nextcode/SIL/LinearLifetimeChecker.h"
#include "llvm/Support/ErrorHandling.h"

namespace nextcode {

struct LLVM_LIBRARY_VISIBILITY LinearLifetimeChecker::ErrorBehaviorKind {
  enum inner_t {
    Invalid = 0,
    ReturnFalse = 1,
    PrintMessage = 2,
    Assert = 4,
    ReturnFalseOnLeak = 8,
    StoreNonConsumingUsesOutsideLifetime = 16,
    PrintMessageAndReturnFalse = PrintMessage | ReturnFalse,
    PrintMessageAndAssert = PrintMessage | Assert,
    ReturnFalseOnLeakAssertOtherwise = ReturnFalseOnLeak | Assert,
  } Value;

  ErrorBehaviorKind() : Value(Invalid) {}
  ErrorBehaviorKind(inner_t Inner) : Value(Inner) { assert(Value != Invalid); }
  ErrorBehaviorKind(unsigned Inner) : Value(inner_t(Inner)) {
    assert(Value != Invalid);
  }

  bool shouldStoreNonConsumingUsesOutsideLifetime() const {
    assert(Value != Invalid);
    return Value & StoreNonConsumingUsesOutsideLifetime;
  }

  bool shouldAssert() const {
    assert(Value != Invalid);
    return Value & Assert;
  }

  bool shouldReturnFalseOnLeak() const {
    assert(Value != Invalid);
    return Value & ReturnFalseOnLeak;
  }

  bool shouldPrintMessage() const {
    assert(Value != Invalid);
    return Value & PrintMessage;
  }

  bool shouldReturnFalse() const {
    assert(Value != Invalid);
    return Value & ReturnFalse;
  }
};

class LLVM_LIBRARY_VISIBILITY LinearLifetimeChecker::Error {
  friend class ErrorBuilder;

  bool foundUseAfterFree = false;
  bool foundLeak = false;
  bool foundOverConsume = false;
  bool foundUseOutsideOfLifetime = false;

public:
  Error() {}

  bool getFoundError() const {
    return foundUseAfterFree || foundLeak || foundOverConsume ||
           foundUseOutsideOfLifetime;
  }

  bool getFoundLeak() const { return foundLeak; }

  bool getFoundUseAfterFree() const { return foundUseAfterFree; }

  bool getFoundOverConsume() const { return foundOverConsume; }

  bool getFoundUseOutsideOfLifetime() const {
    return foundUseOutsideOfLifetime;
  }
};

class LLVM_LIBRARY_VISIBILITY LinearLifetimeChecker::ErrorBuilder {
  StringRef functionName;
  ErrorBehaviorKind behavior;
  std::optional<Error> error;
  unsigned *errorMessageCounter;

public:
  ErrorBuilder(const SILFunction &fn,
               LinearLifetimeChecker::ErrorBehaviorKind behavior,
               unsigned *errorMessageCounter = nullptr)
      : functionName(fn.getName()), behavior(behavior), error(Error()),
        errorMessageCounter(errorMessageCounter) {}

  ErrorBuilder(const SILFunction &fn,
               LinearLifetimeChecker::ErrorBehaviorKind::inner_t behavior,
               unsigned *errorMessageCounter = nullptr)
      : ErrorBuilder(fn, LinearLifetimeChecker::ErrorBehaviorKind(behavior),
                     errorMessageCounter) {}

  ErrorBuilder(const ErrorBuilder &other)
      : functionName(other.functionName), behavior(other.behavior),
        error(other.error), errorMessageCounter(other.errorMessageCounter) {}

  ErrorBuilder &operator=(const ErrorBuilder &other) {
    functionName = other.functionName;
    behavior = other.behavior;
    error = other.error;
    errorMessageCounter = other.errorMessageCounter;
    return *this;
  }

  Error consumeAndGetFinalError() && {
    auto result = *error;
    error = std::nullopt;
    errorMessageCounter = nullptr;
    return result;
  }

  void tryDumpErrorCounter() const {
    if (!errorMessageCounter) {
      return;
    }
    llvm::errs() << "Error#: " << *errorMessageCounter << ". ";
  }

  void tryIncrementErrorCounter() {
    if (!errorMessageCounter) {
      return;
    }
    ++(*errorMessageCounter);
  }

  bool handleLeak(llvm::function_ref<void()> &&messagePrinterFunc) {
    error->foundLeak = true;

    if (behavior.shouldPrintMessage()) {
      tryDumpErrorCounter();
      llvm::errs() << "Begin Error in Function: '" << functionName << "'\n";
      messagePrinterFunc();
      tryDumpErrorCounter();
      llvm::errs() << "End Error in Function: '" << functionName << "'\n";
      tryIncrementErrorCounter();
    }

    if (behavior.shouldReturnFalseOnLeak()) {
      return false;
    }

    // We already printed out our error if we needed to, so don't pass it along.
    return handleError([]() {}, true);
  }

  bool handleOverConsume(llvm::function_ref<void()> &&messagePrinterFunc) {
    error->foundOverConsume = true;
    return handleError(std::move(messagePrinterFunc));
  }

  bool handleUseAfterFree(llvm::function_ref<void()> &&messagePrinterFunc) {
    error->foundUseAfterFree = true;
    return handleError(std::move(messagePrinterFunc));
  }

  bool
  handleMalformedSIL(llvm::function_ref<void()> &&messagePrinterFunc) const {
    return handleError(std::move(messagePrinterFunc));
  }

  /// Handle a case where we either found a use-after-free due to a
  /// non-consuming use after our lifetime has ended /or/ if we found a use
  /// before def of a non consuming value.
  void
  handleUseOutsideOfLifetime(llvm::function_ref<void()> &&messagePrinterFunc) {
    error->foundUseOutsideOfLifetime = true;
    handleError(std::move(messagePrinterFunc));
  }

private:
  bool handleError(llvm::function_ref<void()> &&messagePrinterFunc,
                   bool quiet = false) const {
    if (behavior.shouldPrintMessage()) {
      if (!quiet) {
        tryDumpErrorCounter();
        llvm::errs() << "Begin Error in Function: '" << functionName << "'\n";
      }
      messagePrinterFunc();
      if (!quiet) {
        tryDumpErrorCounter();
        llvm::errs() << "End Error in Function: '" << functionName << "'\n";
        auto *self = const_cast<ErrorBuilder *>(this);
        self->tryIncrementErrorCounter();
      }
    }

    if (behavior.shouldReturnFalse()) {
      return false;
    }

    llvm::errs() << "Found ownership error?!\n";
    llvm_unreachable("triggering standard assertion failure routine");
  }
};

} // namespace nextcode

#endif