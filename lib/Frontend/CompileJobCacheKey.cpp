//===--- CompileJobCacheKey.cpp - compile cache key methods ---------------===//
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
// This file contains utility methods for creating compile job cache keys.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Option/Options.h"
#include <nextcode/Frontend/CompileJobCacheKey.h>
#include <nextcode/Basic/Version.h>
#include <llvm/ADT/SmallString.h>
#include "llvm/ADT/STLExtras.h"
#include "llvm/CAS/HierarchicalTreeBuilder.h"
#include "llvm/CAS/ObjectStore.h"
#include "llvm/Option/ArgList.h"
#include "llvm/Option/OptTable.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/Error.h"
#include "llvm/Support/MemoryBuffer.h"

using namespace nextcode;

// TODO: Rewrite this into CASNodeSchema.
llvm::Expected<llvm::cas::ObjectRef> nextcode::createCompileJobBaseCacheKey(
    llvm::cas::ObjectStore &CAS, ArrayRef<const char *> Args) {
  // Don't count the `-frontend` in the first location since only frontend
  // invocation can have a cache key.
  if (Args.size() > 1 && StringRef(Args.front()) == "-frontend")
    Args = Args.drop_front();

  unsigned MissingIndex;
  unsigned MissingCount;
  std::unique_ptr<llvm::opt::OptTable> Table = createNeXTCodeOptTable();
  llvm::opt::InputArgList ParsedArgs = Table->ParseArgs(
      Args, MissingIndex, MissingCount, options::FrontendOption);

  SmallString<256> CommandLine;
  for (auto *Arg : ParsedArgs) {
    const auto &Opt = Arg->getOption();

    // Skip the options that doesn't affect caching.
    if (Opt.hasFlag(options::CacheInvariant))
      continue;

    if (Opt.hasFlag(options::ArgumentIsFileList)) {
      auto FileList = llvm::MemoryBuffer::getFile(Arg->getValue());
      if (!FileList)
        return llvm::errorCodeToError(FileList.getError());
      CommandLine.append(Opt.getRenderName());
      CommandLine.push_back(0);
      CommandLine.append((*FileList)->getBuffer());
      CommandLine.push_back(0);
      continue;
    }

    CommandLine.append(Arg->getAsString(ParsedArgs));
    CommandLine.push_back(0);
  }

  llvm::cas::HierarchicalTreeBuilder Builder;
  auto CMD = CAS.storeFromString(std::nullopt, CommandLine);
  if (!CMD)
    return CMD.takeError();
  Builder.push(*CMD, llvm::cas::TreeEntry::Regular, "command-line");

  // FIXME: The version is maybe insufficient...
  auto Version =
      CAS.storeFromString(std::nullopt, version::getNeXTCodeFullVersion());
  if (!Version)
    return Version.takeError();
  Builder.push(*Version, llvm::cas::TreeEntry::Regular, "version");

  if (auto Out = Builder.create(CAS))
    return Out->getRef();
  else
    return Out.takeError();
}

llvm::Expected<llvm::cas::ObjectRef>
nextcode::createCompileJobCacheKeyForOutput(llvm::cas::ObjectStore &CAS,
                                         llvm::cas::ObjectRef BaseKey,
                                         unsigned InputIndex) {
  std::string InputInfo;
  llvm::raw_string_ostream OS(InputInfo);

  // CacheKey is the index of the producting input + the base key.
  // Encode the unsigned value as little endian in the field.
  llvm::support::endian::write<uint32_t>(OS, InputIndex, llvm::support::little);

  return CAS.storeFromString({BaseKey}, OS.str());
}
