//===--- CachedDiagnostics.h - Cached Diagnostics ---------------*- C++ -*-===//
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
//  This file defines the CachedDiagnosticConsumer class, which
//  caches the diagnostics which can be replayed with other DiagnosticConumers.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_CACHEDDIAGNOSTICS_H
#define NEXTCODE_CACHEDDIAGNOSTICS_H

#include "llvm/Support/Error.h"

namespace nextcode {

class CompilerInstance;
class DiagnosticEngine;
class SourceManager;
class FrontendInputsAndOutputs;

class CachingDiagnosticsProcessor {
public:
  CachingDiagnosticsProcessor(CompilerInstance &Instance);
  ~CachingDiagnosticsProcessor();

  /// Start capturing all the diagnostics from DiagnosticsEngine.
  void startDiagnosticCapture();
  /// End capturing all the diagnostics from DiagnosticsEngine.
  void endDiagnosticCapture();

  /// Emit serialized diagnostics into output stream.
  llvm::Error serializeEmittedDiagnostics(llvm::raw_ostream &os);

  /// Used to replay the previously cached diagnostics, after a cache hit.
  llvm::Error replayCachedDiagnostics(llvm::StringRef Buffer);

private:
  class Implementation;
  Implementation& Impl;
};

}

#endif