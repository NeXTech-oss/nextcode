//===--- AccessRequests.h - Access{Level,Scope} Requests -----*- C++ -*-===//
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
//  This file defines access-control requests.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_ACCESS_REQUESTS_H
#define NEXTCODE_ACCESS_REQUESTS_H

#include "nextcode/AST/AccessScope.h"
#include "nextcode/AST/AttrKind.h"
#include "nextcode/AST/Evaluator.h"
#include "nextcode/AST/SimpleRequest.h"
#include "nextcode/Basic/Statistic.h"
#include "llvm/ADT/Hashing.h"

namespace nextcode {

class AbstractStorageDecl;
class ExtensionDecl;
class ValueDecl;

/// Request the AccessLevel of the given ValueDecl.
class AccessLevelRequest :
    public SimpleRequest<AccessLevelRequest,
                         AccessLevel(ValueDecl *),
                         RequestFlags::SeparatelyCached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  AccessLevel evaluate(Evaluator &evaluator, ValueDecl *decl) const;

public:
  // Separate caching.
  bool isCached() const { return true; }
  std::optional<AccessLevel> getCachedResult() const;
  void cacheResult(AccessLevel value) const;
};

/// Request the setter AccessLevel of the given AbstractStorageDecl,
/// which may be lower than its normal AccessLevel, and determines
/// the accessibility of mutating accessors.
class SetterAccessLevelRequest :
    public SimpleRequest<SetterAccessLevelRequest,
                         AccessLevel(AbstractStorageDecl *),
                         RequestFlags::SeparatelyCached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  AccessLevel evaluate(Evaluator &evaluator, AbstractStorageDecl *decl) const;

public:
  // Separate caching.
  bool isCached() const { return true; }
  std::optional<AccessLevel> getCachedResult() const;
  void cacheResult(AccessLevel value) const;
};

using DefaultAndMax = std::pair<AccessLevel, AccessLevel>;

/// Request the Default and Max AccessLevels of the given ExtensionDecl.
class DefaultAndMaxAccessLevelRequest :
    public SimpleRequest<DefaultAndMaxAccessLevelRequest,
                         DefaultAndMax(ExtensionDecl *),
                         RequestFlags::SeparatelyCached> {
public:
  using SimpleRequest::SimpleRequest;
private:
  friend SimpleRequest;

  // Evaluation.
  DefaultAndMax evaluate(Evaluator &evaluator, ExtensionDecl *decl) const;

public:
  // Separate caching.
  bool isCached() const { return true; }
  std::optional<DefaultAndMax> getCachedResult() const;
  void cacheResult(DefaultAndMax value) const;
};

#define NEXTCODE_TYPEID_ZONE AccessControl
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/AccessTypeIDZone.def"
#include "nextcode/Basic/DefineTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER

// Set up reporting of evaluated requests.
#define NEXTCODE_REQUEST(Zone, RequestType, Sig, Caching, LocOptions)             \
  template <>                                                                  \
  inline void reportEvaluatedRequest(UnifiedStatsReporter &stats,              \
                                     const RequestType &request) {             \
    ++stats.getFrontendCounters().RequestType;                                 \
  }
#include "nextcode/AST/AccessTypeIDZone.def"
#undef NEXTCODE_REQUEST

} // end namespace nextcode

#endif // NEXTCODE_ACCESS_REQUESTS_H
