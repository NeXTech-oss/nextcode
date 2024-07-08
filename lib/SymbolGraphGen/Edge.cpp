//===--- Edge.cpp - Symbol Graph Edge -------------------------------------===//
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

#include "nextcode/AST/Module.h"
#include "Edge.h"
#include "Symbol.h"
#include "SymbolGraphASTWalker.h"

#include <queue>

using namespace nextcode;
using namespace symbolgraphgen;

void Edge::serialize(llvm::json::OStream &OS) const {
  OS.object([&](){
    OS.attribute("kind", Kind.Name);
    SmallString<256> SourceUSR, TargetUSR;

    Source.getUSR(SourceUSR);
    OS.attribute("source", SourceUSR.str());

    Target.getUSR(TargetUSR);
    OS.attribute("target", TargetUSR.str());

    // In case a dependent module isn't available, serialize a fallback name.
    auto TargetModuleName = Target.getSymbolDecl()
        ->getModuleContext()->getName().str();

    if (TargetModuleName != Graph->M.getName().str()) {
      SmallString<128> Scratch(TargetModuleName);
      llvm::raw_svector_ostream PathOS(Scratch);
      PathOS << '.';
      Target.printPath(PathOS);
      OS.attribute("targetFallback", Scratch.str());
    }

    if (ConformanceExtension) {
      SmallVector<Requirement, 4> FilteredRequirements;
      filterGenericRequirements(
          ConformanceExtension->getGenericRequirements(),
          ConformanceExtension->getExtendedProtocolDecl(),
          FilteredRequirements);
      if (!FilteredRequirements.empty()) {
        OS.attributeArray("nextcodeConstraints", [&](){
          for (const auto &Req : FilteredRequirements) {
            ::serialize(Req, OS);
          }
        });
      }
    }
    
    const ValueDecl *InheritingDecl = Source.getInheritedDecl();

    // If our source symbol is a inheriting decl, write in information about
    // where it's inheriting docs from.
    if (InheritingDecl) {
      Symbol inheritedSym(Graph, InheritingDecl, nullptr);
      SmallString<256> USR, Display;
      llvm::raw_svector_ostream DisplayOS(Display);
      
      inheritedSym.getUSR(USR);
      inheritedSym.printPath(DisplayOS);
      
      OS.attributeObject("sourceOrigin", [&](){
        OS.attribute("identifier", USR.str());
        OS.attribute("displayName", Display.str());
      });
    }
  });
}
