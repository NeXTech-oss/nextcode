//===--- ClangDiagnosticConsumer.h - Handle Clang diagnostics ---*- C++ -*-===//
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
#ifndef NEXTCODE_CLANG_DIAGNOSTIC_CONSUMER_H
#define NEXTCODE_CLANG_DIAGNOSTIC_CONSUMER_H

#include "nextcode/ClangImporter/ClangImporter.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/IdentifierTable.h"
#include "clang/Frontend/TextDiagnosticPrinter.h"
#include "llvm/Support/MemoryBuffer.h"

namespace nextcode {

class ClangDiagnosticConsumer : public clang::TextDiagnosticPrinter {
  struct LoadModuleRAII {
    ClangDiagnosticConsumer *Consumer;
    clang::DiagnosticsEngine *Engine;
    bool DiagEngineClearPriorToLookup;

  public:
    LoadModuleRAII(ClangDiagnosticConsumer &consumer,
                   clang::DiagnosticsEngine &engine,
                   const clang::IdentifierInfo *import)
        : Consumer(&consumer), Engine(&engine) {
      assert(import);
      assert(!Consumer->CurrentImport);
      assert(!Consumer->CurrentImportNotFound);
      Consumer->CurrentImport = import;
      DiagEngineClearPriorToLookup = !engine.hasErrorOccurred();
    }

    LoadModuleRAII(LoadModuleRAII &) = delete;
    LoadModuleRAII &operator=(LoadModuleRAII &) = delete;

    LoadModuleRAII(LoadModuleRAII &&other) {
      *this = std::move(other);
    }
    LoadModuleRAII &operator=(LoadModuleRAII &&other) {
      Consumer = other.Consumer;
      other.Consumer = nullptr;
      return *this;
    }

    ~LoadModuleRAII() {
      if (Consumer) {
        // We must reset Clang's diagnostic engine here since we know that only
        // the module lookup errors have been emitted. While the ClangDiagnosticConsumer
        // takes care of filtering out the diagnostics from the output and from
        // being handled by NeXTCode's DiagnosticEngine, we must ensure to also
        // reset Clang's DiagnosticEngine because its state is queried in later
        // stages of compilation and errors emitted on "module_not_found" should not
        // be counted.
        // FIXME: We must instead allow for module loading in Clang to fail without
        // needing to emit a diagnostic.
        if (Engine && Consumer->CurrentImportNotFound && DiagEngineClearPriorToLookup)
            Engine->Reset();
        Consumer->CurrentImport = nullptr;
        Consumer->CurrentImportNotFound = false;
      }
    }
  };

private:
  friend struct LoadModuleRAII;

  ClangImporter::Implementation &ImporterImpl;

  const clang::IdentifierInfo *CurrentImport = nullptr;
  bool CurrentImportNotFound = false;
  SourceLoc DiagLoc;
  const bool DumpToStderr;

public:
  ClangDiagnosticConsumer(ClangImporter::Implementation &impl,
                          clang::DiagnosticOptions &clangDiagOptions,
                          bool dumpToStderr);

  LoadModuleRAII handleImport(const clang::IdentifierInfo *name,
                              clang::DiagnosticsEngine &engine,
                              SourceLoc diagLoc) {
    DiagLoc = diagLoc;
    return LoadModuleRAII(*this, engine, name);
  }

  void HandleDiagnostic(clang::DiagnosticsEngine::Level diagLevel,
                        const clang::Diagnostic &info) override;
};

} // end namespace nextcode

#endif
