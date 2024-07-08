//===--- MigrationState.cpp -----------------------------------------------===//
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
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/SourceManager.h"
#include "nextcode/Migrator/MigrationState.h"
#include "llvm/Support/Path.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/raw_ostream.h"

using llvm::StringRef;

using namespace nextcode;
using namespace nextcode::migrator;

#pragma mark - MigrationState

std::string MigrationState::getInputText() const {
  return SrcMgr.getEntireTextForBuffer(InputBufferID).str();
}

std::string MigrationState::getOutputText() const {
  return SrcMgr.getEntireTextForBuffer(OutputBufferID).str();
}

static bool quickDumpText(StringRef OutFilename, StringRef Text) {
  std::error_code Error;
  llvm::raw_fd_ostream FileOS(OutFilename,
                              Error, llvm::sys::fs::OF_Text);
  if (FileOS.has_error()) {
    return true;
  }

  FileOS << Text;

  FileOS.flush();

  return FileOS.has_error();
}

bool MigrationState::print(size_t StateNumber, StringRef OutDir) const {
  auto Failed = false;

  SmallString<256> InputFileStatePath;
  llvm::sys::path::append(InputFileStatePath, OutDir);

  {
    SmallString<256> InputStateFilenameScratch;
    llvm::raw_svector_ostream InputStateFilenameOS(InputStateFilenameScratch);
    InputStateFilenameOS << StateNumber << '-' << "FixitMigrationState";
    InputStateFilenameOS << '-' << "Input";
    llvm::sys::path::append(InputFileStatePath, InputStateFilenameOS.str());
    llvm::sys::path::replace_extension(InputFileStatePath, ".code");
  }

  Failed |= quickDumpText(InputFileStatePath, getInputText());

  SmallString<256> OutputFileStatePath;
  llvm::sys::path::append(OutputFileStatePath, OutDir);

  {
    SmallString<256> OutputStateFilenameScratch;
    llvm::raw_svector_ostream OutputStateFilenameOS(OutputStateFilenameScratch);
    OutputStateFilenameOS << StateNumber << '-' << "FixitMigrationState";
    OutputStateFilenameOS << '-' << "Output";
    llvm::sys::path::append(OutputFileStatePath, OutputStateFilenameOS.str());
    llvm::sys::path::replace_extension(OutputFileStatePath, ".code");
  }

  Failed |= quickDumpText(OutputFileStatePath, getOutputText());

  return Failed;
}