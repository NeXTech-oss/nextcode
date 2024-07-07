//===------- ConstExtractRequests.h - Extraction  Requests ------*- C++ -*-===//
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
//  This file defines const-extraction requests.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_CONST_EXTRACT_REQUESTS_H
#define NEXTCODE_CONST_EXTRACT_REQUESTS_H

#include "nextcode/AST/ASTTypeIDs.h"
#include "nextcode/AST/ConstTypeInfo.h"
#include "nextcode/AST/EvaluatorDependencies.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/AST/SimpleRequest.h"
#include "nextcode/Basic/Statistic.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/PointerUnion.h"
#include "llvm/ADT/TinyPtrVector.h"

namespace nextcode {

class Decl;
class DeclName;
class EnumDecl;

/// Retrieve information about compile-time-known values
class ConstantValueInfoRequest
    : public SimpleRequest<
          ConstantValueInfoRequest,
          ConstValueTypeInfo(
              NominalTypeDecl *,
              llvm::PointerUnion<const SourceFile *, ModuleDecl *>),
          RequestFlags::Cached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  ConstValueTypeInfo
  evaluate(Evaluator &eval, NominalTypeDecl *nominal,
           llvm::PointerUnion<const SourceFile *, ModuleDecl *> extractionScope)
      const;

public:
  // Caching
  bool isCached() const { return true; }
};

#define NEXTCODE_TYPEID_ZONE ConstExtract
#define NEXTCODE_TYPEID_HEADER "nextcode/ConstExtract/ConstExtractTypeIDZone.def"
#include "nextcode/Basic/DefineTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER

// Set up reporting of evaluated requests.
template<typename Request>
void reportEvaluatedRequest(UnifiedStatsReporter &stats,
                            const Request &request);

#define NEXTCODE_REQUEST(Zone, RequestType, Sig, Caching, LocOptions)             \
  template <>                                                                  \
  inline void reportEvaluatedRequest(UnifiedStatsReporter &stats,              \
                                     const RequestType &request) {             \
    ++stats.getFrontendCounters().RequestType;                                 \
  }
#include "nextcode/ConstExtract/ConstExtractTypeIDZone.def"
#undef NEXTCODE_REQUEST

} // end namespace nextcode

#endif // NEXTCODE_CONST_EXTRACT_REQUESTS_H

