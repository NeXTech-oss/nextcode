//===--- CodeCompletion.h - Routines for code completion --------*- C++ -*-===//
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

#ifndef NEXTCODE_IDE_CODECOMPLETION_H
#define NEXTCODE_IDE_CODECOMPLETION_H

#include "nextcode/IDE/CodeCompletionConsumer.h"
#include "nextcode/IDE/CodeCompletionContext.h"
#include "nextcode/IDE/CodeCompletionResult.h"
#include "nextcode/IDE/CodeCompletionResultSink.h"
#include "nextcode/IDE/CompletionLookup.h"

namespace nextcode {
class IDEInspectionCallbacksFactory;
class Decl;
class DeclContext;
class FrontendOptions;
class ModuleDecl;
class SourceFile;

namespace ide {

class CodeCompletionCache;
struct RequestedCachedModule;

/// A routine to remove code completion tokens from code completion
/// tests.
///
/// \code
/// code-completion-token:
///     '#^' identifier '^#'
/// \endcode
///
/// \param Input test source code.
/// \param TokenName names the token whose position should be returned in
/// \p CompletionOffset.
/// \param CompletionOffset set to ~0U on error, or to a 0-based byte offset on
/// success.
///
/// \returns test source code without any code completion tokens.
std::string removeCodeCompletionTokens(StringRef Input,
                                       StringRef TokenName,
                                       unsigned *CompletionOffset);

template <typename T>
ArrayRef<T> copyArray(llvm::BumpPtrAllocator &Allocator,
                            ArrayRef<T> Arr) {
  T *Buffer = Allocator.Allocate<T>(Arr.size());
  std::copy(Arr.begin(), Arr.end(), Buffer);
  return llvm::ArrayRef(Buffer, Arr.size());
}

bool isDynamicLookup(Type T);

void postProcessCompletionResults(
    MutableArrayRef<CodeCompletionResult *> results, CompletionKind Kind,
    const DeclContext *DC, CodeCompletionResultSink *Sink);

void collectCompletionResults(CodeCompletionContext &CompletionContext,
                              CompletionLookup &Lookup, DeclContext *DC,
                              const ExpectedTypeContext &TypeContext,
                              bool CanCurrDeclContextHandleAsync);

/// Create a factory for code completion callbacks.
IDEInspectionCallbacksFactory *
makeCodeCompletionCallbacksFactory(CodeCompletionContext &CompletionContext,
                                   CodeCompletionConsumer &Consumer);

/// Lookup the top-level code completions from \p module and store them in
/// \p targetSink.
///
/// Results are looked up as if in \p currDeclContext, which may be null.
void lookupCodeCompletionResultsFromModule(CodeCompletionResultSink &targetSink,
                                           const ModuleDecl *module,
                                           ArrayRef<std::string> accessPath,
                                           bool needLeadingDot,
                                           const SourceFile *SF);

void addExprKeywords(CodeCompletionResultSink &Sink, DeclContext *DC);

void addSuperKeyword(CodeCompletionResultSink &Sink, DeclContext *DC);

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_CODECOMPLETION_H
