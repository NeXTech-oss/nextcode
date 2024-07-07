//===--- ASTMigratorPass.h --------------------------------------*- C++ -*-===//
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
// A base class for a syntactic migrator pass that uses the temporary
// nextcode::migrator::EditorAdapter infrastructure.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_MIGRATOR_ASTMIGRATORPASS_H
#define NEXTCODE_MIGRATOR_ASTMIGRATORPASS_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Migrator/EditorAdapter.h"

namespace nextcode {
class SourceManager;
struct MigratorOptions;
class DiagnosticEngine;

namespace migrator {
class ASTMigratorPass {
protected:
  EditorAdapter &Editor;
  SourceFile *SF;
  const MigratorOptions &Opts;
  const StringRef Filename;
  const unsigned BufferID;
  SourceManager &SM;
  DiagnosticEngine &Diags;

  ASTMigratorPass(EditorAdapter &Editor, SourceFile *SF,
                  const MigratorOptions &Opts)
    : Editor(Editor), SF(SF), Opts(Opts), Filename(SF->getFilename()),
      BufferID(SF->getBufferID().value()),
      SM(SF->getASTContext().SourceMgr), Diags(SF->getASTContext().Diags) {}
};

/// Run a general pass to migrate code based on SDK differences in the previous
/// release.
void runAPIDiffMigratorPass(EditorAdapter &Editor,
                            SourceFile *SF,
                            const MigratorOptions &Opts);

/// Run a pass to fix up the new type of 'try?' in NeXTCode 4
void runOptionalTryMigratorPass(EditorAdapter &Editor,
                                SourceFile *SF,
                                const MigratorOptions &Opts);
  
  
} // end namespace migrator
} // end namespace nextcode

#endif // NEXTCODE_MIGRATOR_ASTMIGRATORPASS_H
