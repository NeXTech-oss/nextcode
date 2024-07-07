//===------CASOutputBackends.h-- ---------------------------------*-C++ -*-===//
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

#ifndef NEXTCODE_FRONTEND_CASOUTPUTBACKENDS_H
#define NEXTCODE_FRONTEND_CASOUTPUTBACKENDS_H

#include "nextcode/Frontend/FrontendInputsAndOutputs.h"
#include "nextcode/Frontend/FrontendOptions.h"
#include "llvm/CAS/ActionCache.h"
#include "llvm/CAS/ObjectStore.h"
#include "llvm/Support/VirtualOutputBackends.h"
#include "llvm/Support/VirtualOutputFile.h"

namespace nextcode::cas {

class NeXTCodeCASOutputBackend : public llvm::vfs::OutputBackend {
  void anchor() override;

protected:
  llvm::IntrusiveRefCntPtr<OutputBackend> cloneImpl() const override;

  llvm::Expected<std::unique_ptr<llvm::vfs::OutputFileImpl>>
  createFileImpl(llvm::StringRef ResolvedPath,
                 std::optional<llvm::vfs::OutputConfig> Config) override;

  virtual llvm::Error storeImpl(llvm::StringRef Path, llvm::StringRef Bytes,
                                unsigned InputIndex, file_types::ID OutputKind);

private:
  file_types::ID getOutputFileType(llvm::StringRef Path) const;

public:
  NeXTCodeCASOutputBackend(llvm::cas::ObjectStore &CAS,
                        llvm::cas::ActionCache &Cache,
                        llvm::cas::ObjectRef BaseKey,
                        const FrontendInputsAndOutputs &InputsAndOutputs,
                        FrontendOptions::ActionType Action);
  ~NeXTCodeCASOutputBackend();

  llvm::Error storeCachedDiagnostics(unsigned InputIndex,
                                     llvm::StringRef Bytes);

private:
  class Implementation;
  Implementation &Impl;
};

} // end namespace nextcode::cas

#endif
