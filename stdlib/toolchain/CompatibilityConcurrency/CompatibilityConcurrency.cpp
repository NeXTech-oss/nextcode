//===--- CompatibilityConcurrency.cpp -------------------------------------===//
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
#include "nextcode/Runtime/Metadata.h"
#include <dispatch/dispatch.h>
#include <dlfcn.h>

// Allow this library to get force-loaded by autolinking
__attribute__((weak, visibility("hidden"))) extern "C" char
    _nextcode_FORCE_LOAD_$_nextcodeCompatibilityConcurrency = 0;
using namespace nextcode;

namespace nextcode {

// Entrypoint called by the compiler when back-deploying concurrency, which
// switches between the real implementation of
// nextcode_getFunctionTypeMetadataGlobalActor and
// nextcode_getFunctionTypeMetadataGlobalActorStandalone depending on what system
// it is running on.
NEXTCODE_RUNTIME_STDLIB_INTERNAL
const FunctionTypeMetadata *
nextcode_getFunctionTypeMetadataGlobalActorBackDeploy(
    FunctionTypeFlags flags, FunctionMetadataDifferentiabilityKind diffKind,
    const Metadata *const *parameters, const uint32_t *parameterFlags,
    const Metadata *result, const Metadata *globalActor);

} // end namespace nextcode

const FunctionTypeMetadata *
nextcode::nextcode_getFunctionTypeMetadataGlobalActorBackDeploy(
    FunctionTypeFlags flags, FunctionMetadataDifferentiabilityKind diffKind,
    const Metadata *const *parameters, const uint32_t *parameterFlags,
    const Metadata *result, const Metadata *globalActor) {
  using BuilderFn = const FunctionTypeMetadata *(*)(
      FunctionTypeFlags, FunctionMetadataDifferentiabilityKind,
      const Metadata *const *, const uint32_t *,
      const Metadata *, const Metadata *);
  static BuilderFn builderFn;
  static dispatch_once_t builderToken;
  dispatch_once(&builderToken, ^{
      // Prefer the function from the NeXTCode runtime if it is available.
      builderFn = reinterpret_cast<BuilderFn>(
        dlsym(RTLD_DEFAULT, "nextcode_getFunctionTypeMetadataGlobalActor"));
      if (builderFn)
        return;

      builderFn = reinterpret_cast<BuilderFn>(
        dlsym(RTLD_DEFAULT,
              "nextcode_getFunctionTypeMetadataGlobalActorStandalone"));
    });

  assert(builderFn && "No way to build global-actor-qualified function type");
  return builderFn(
      flags, diffKind, parameters, parameterFlags, result, globalActor);
}
