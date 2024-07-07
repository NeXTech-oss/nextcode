//===--- CASOptions.cpp - CAS & caching options ---------------------------===//
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
//  This file defines the CASOptions class, which provides various
//  CAS and caching flags.
//
//===----------------------------------------------------------------------===//

#include "nextcode/Basic/CASOptions.h"

using namespace nextcode;

void CASOptions::enumerateCASConfigurationFlags(
      llvm::function_ref<void(llvm::StringRef)> Callback) const {
  if (EnableCaching) {
    Callback("-cache-compile-job");
    if (!CASOpts.CASPath.empty()) {
      Callback("-cas-path");
      Callback(CASOpts.CASPath);
    }
    if (!CASOpts.PluginPath.empty()) {
      Callback("-cas-plugin-path");
      Callback(CASOpts.PluginPath);
      for (auto Opt : CASOpts.PluginOptions) {
        Callback("-cas-plugin-option");
        Callback((llvm::Twine(Opt.first) + "=" + Opt.second).str());
      }
    }
  }
}
