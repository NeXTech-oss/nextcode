//===--- SerializedSILLoader.h - Handle SIL section in modules --*- C++ -*-===//
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

#ifndef NEXTCODE_SERIALIZATION_SILLOADER_H
#define NEXTCODE_SERIALIZATION_SILLOADER_H

#include "nextcode/AST/AutoDiff.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/SIL/Notifications.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "nextcode/SIL/SILLinkage.h"
#include "nextcode/SIL/SILMoveOnlyDeinit.h"
#include <memory>
#include <vector>

namespace nextcode {
class ASTContext;
class FileUnit;
class ModuleDecl;
class SILDeserializer;
class SILFunction;
class SILGlobalVariable;
class SILProperty;
class SILModule;
class SILVTable;
class SILWitnessTable;
class SILDefaultWitnessTable;
class SILDifferentiabilityWitness;

/// Maintains a list of SILDeserializer, one for each serialized modules
/// in ASTContext. It provides lookupSILFunction that will perform lookup
/// on each SILDeserializer.
class SerializedSILLoader {
private:
  std::vector<std::unique_ptr<SILDeserializer>> LoadedSILSections;

  explicit SerializedSILLoader(
      ASTContext &ctx, SILModule *SILMod,
      DeserializationNotificationHandlerSet *callbacks);

public:
  /// Create a new loader.
  ///
  /// \param callbacks - not owned by the loader
  static std::unique_ptr<SerializedSILLoader>
  create(ASTContext &ctx, SILModule *SILMod,
         DeserializationNotificationHandlerSet *callbacks) {
    return std::unique_ptr<SerializedSILLoader>(
        new SerializedSILLoader(ctx, SILMod, callbacks));
  }
  ~SerializedSILLoader();

  SILFunction *lookupSILFunction(SILFunction *Callee, bool onlyUpdateLinkage);
  SILFunction *lookupSILFunction(StringRef Name,
                                 std::optional<SILLinkage> linkage);
  bool hasSILFunction(StringRef Name,
                      std::optional<SILLinkage> linkage = std::nullopt);
  SILVTable *lookupVTable(const ClassDecl *C);
  SILMoveOnlyDeinit *lookupMoveOnlyDeinit(const NominalTypeDecl *nomDecl);
  SILWitnessTable *lookupWitnessTable(SILWitnessTable *C);
  SILDefaultWitnessTable *lookupDefaultWitnessTable(SILDefaultWitnessTable *C);
  SILDifferentiabilityWitness *
  lookupDifferentiabilityWitness(SILDifferentiabilityWitnessKey key);

  /// Invalidate the cached entries for deserialized state. Must be
  /// called when erasing deserialized state in the SILModule.
  void invalidateAllCaches();
  bool invalidateFunction(SILFunction *f);
  bool invalidateGlobalVariable(SILGlobalVariable *gv);
  bool invalidateVTable(SILVTable *vt);
  bool invalidateWitnessTable(SILWitnessTable *wt);
  bool invalidateDefaultWitnessTable(SILDefaultWitnessTable *wt);
  bool invalidateProperty(SILProperty *p);
  bool invalidateDifferentiabilityWitness(SILDifferentiabilityWitness *w);

  /// Deserialize all SILFunctions, VTables, and WitnessTables for
  /// a given Module.
  ///
  /// If PrimaryFile is nullptr, all definitions are brought in with
  /// definition linkage.
  ///
  /// Otherwise, definitions not in the primary file are brought in
  /// with external linkage.
  void getAllForModule(Identifier Mod, FileUnit *PrimaryFile);

  /// Deserialize all SILFunctions in all SILModules.
  void getAllSILFunctions();

  /// Deserialize all VTables in all SILModules.
  void getAllVTables();

  /// Deserialize all WitnessTables in all SILModules.
  void getAllWitnessTables();

  /// Deserialize all DefaultWitnessTables in all SILModules.
  void getAllDefaultWitnessTables();

  /// Deserialize all Properties in all SILModules.
  void getAllProperties();

  /// Deserialize all DifferentiabilityWitnesses in all SILModules.
  void getAllDifferentiabilityWitnesses();

  SerializedSILLoader(const SerializedSILLoader &) = delete;
  SerializedSILLoader(SerializedSILLoader &&) = delete;
  SerializedSILLoader &operator=(const SerializedSILLoader &) = delete;
  SerializedSILLoader &operator=(SerializedSILLoader &&) = delete;
};

} // end namespace nextcode

#endif
