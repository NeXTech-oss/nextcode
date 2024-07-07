//===--- SerializedDiagnosticConsumer.h - Serialize Diagnostics -*- C++ -*-===//
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
//  This file defines the SerializedDiagnosticConsumer class, which
//  serializes diagnostics to Clang's serialized diagnostic bitcode format.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_SERIALIZEDDIAGNOSTICCONSUMER_H
#define NEXTCODE_SERIALIZEDDIAGNOSTICCONSUMER_H

#include <memory>

namespace llvm {
  class StringRef;
}

namespace nextcode {

  class DiagnosticConsumer;

  namespace serialized_diagnostics {
    /// Create a DiagnosticConsumer that serializes diagnostics to a file, using
    /// Clang's serialized diagnostics format.
    ///
    /// \param outputPath the file path to write the diagnostics to.
    ///
    /// \returns A new diagnostic consumer that serializes diagnostics.
    std::unique_ptr<DiagnosticConsumer>
    createConsumer(llvm::StringRef outputPath, bool emitMacroExpansionFiles);
  }
}

#endif
