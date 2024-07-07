//===--- ClangImporterRequests.h - Clang Importer Requests ------*- C++ -*-===//
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
//  This file defines clang-importer requests.
//
//===----------------------------------------------------------------------===//
#ifndef NEXTCODE_CLANG_IMPORTER_REQUESTS_H
#define NEXTCODE_CLANG_IMPORTER_REQUESTS_H

#include "nextcode/AST/SimpleRequest.h"
#include "nextcode/AST/ASTTypeIDs.h"
#include "nextcode/AST/EvaluatorDependencies.h"
#include "nextcode/AST/FileUnit.h"
#include "nextcode/AST/Identifier.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/Basic/Statistic.h"
#include "llvm/ADT/Hashing.h"
#include "llvm/ADT/TinyPtrVector.h"

namespace nextcode {
class Decl;
class DeclName;
class EnumDecl;

/// The input type for a clang direct lookup request.
struct ClangDirectLookupDescriptor final {
  Decl *decl;
  const clang::Decl *clangDecl;
  DeclName name;

  ClangDirectLookupDescriptor(Decl *decl, const clang::Decl *clangDecl,
                              DeclName name)
      : decl(decl), clangDecl(clangDecl), name(name) {}

  friend llvm::hash_code hash_value(const ClangDirectLookupDescriptor &desc) {
    return llvm::hash_combine(desc.name, desc.decl, desc.clangDecl);
  }

  friend bool operator==(const ClangDirectLookupDescriptor &lhs,
                         const ClangDirectLookupDescriptor &rhs) {
    return lhs.name == rhs.name && lhs.decl == rhs.decl &&
           lhs.clangDecl == rhs.clangDecl;
  }

  friend bool operator!=(const ClangDirectLookupDescriptor &lhs,
                         const ClangDirectLookupDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out,
                    const ClangDirectLookupDescriptor &desc);
SourceLoc extractNearestSourceLoc(const ClangDirectLookupDescriptor &desc);

/// This matches NeXTCodeLookupTable::SingleEntry;
using SingleEntry = llvm::PointerUnion<clang::NamedDecl *, clang::MacroInfo *,
                                       clang::ModuleMacro *>;
/// Uses the appropriate NeXTCodeLookupTable to find a set of clang decls given
/// their name.
class ClangDirectLookupRequest
    : public SimpleRequest<ClangDirectLookupRequest,
                           SmallVector<SingleEntry, 4>(
                               ClangDirectLookupDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  SmallVector<SingleEntry, 4> evaluate(Evaluator &evaluator,
                                       ClangDirectLookupDescriptor desc) const;
};

/// The input type for a namespace member lookup request.
struct CXXNamespaceMemberLookupDescriptor final {
  EnumDecl *namespaceDecl;
  DeclName name;

  CXXNamespaceMemberLookupDescriptor(EnumDecl *namespaceDecl, DeclName name)
      : namespaceDecl(namespaceDecl), name(name) {
    assert(isa<clang::NamespaceDecl>(namespaceDecl->getClangDecl()));
  }

  friend llvm::hash_code
  hash_value(const CXXNamespaceMemberLookupDescriptor &desc) {
    return llvm::hash_combine(desc.name, desc.namespaceDecl);
  }

  friend bool operator==(const CXXNamespaceMemberLookupDescriptor &lhs,
                         const CXXNamespaceMemberLookupDescriptor &rhs) {
    return lhs.name == rhs.name && lhs.namespaceDecl == rhs.namespaceDecl;
  }

  friend bool operator!=(const CXXNamespaceMemberLookupDescriptor &lhs,
                         const CXXNamespaceMemberLookupDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out,
                    const CXXNamespaceMemberLookupDescriptor &desc);
SourceLoc
extractNearestSourceLoc(const CXXNamespaceMemberLookupDescriptor &desc);

/// Uses ClangDirectLookup to find a named member inside of the given namespace.
class CXXNamespaceMemberLookup
    : public SimpleRequest<CXXNamespaceMemberLookup,
                           TinyPtrVector<ValueDecl *>(
                               CXXNamespaceMemberLookupDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  TinyPtrVector<ValueDecl *>
  evaluate(Evaluator &evaluator, CXXNamespaceMemberLookupDescriptor desc) const;
};

/// The input type for a record member lookup request.
struct ClangRecordMemberLookupDescriptor final {
  NominalTypeDecl *recordDecl;
  DeclName name;

  ClangRecordMemberLookupDescriptor(NominalTypeDecl *recordDecl, DeclName name)
      : recordDecl(recordDecl), name(name) {
    assert(isa<clang::RecordDecl>(recordDecl->getClangDecl()));
  }

  friend llvm::hash_code
  hash_value(const ClangRecordMemberLookupDescriptor &desc) {
    return llvm::hash_combine(desc.name, desc.recordDecl);
  }

  friend bool operator==(const ClangRecordMemberLookupDescriptor &lhs,
                         const ClangRecordMemberLookupDescriptor &rhs) {
    return lhs.name == rhs.name && lhs.recordDecl == rhs.recordDecl;
  }

  friend bool operator!=(const ClangRecordMemberLookupDescriptor &lhs,
                         const ClangRecordMemberLookupDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out,
                    const ClangRecordMemberLookupDescriptor &desc);
SourceLoc
extractNearestSourceLoc(const ClangRecordMemberLookupDescriptor &desc);

/// Uses ClangDirectLookup to find a named member inside of the given record.
class ClangRecordMemberLookup
    : public SimpleRequest<ClangRecordMemberLookup,
                           TinyPtrVector<ValueDecl *>(
                               ClangRecordMemberLookupDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  TinyPtrVector<ValueDecl *>
  evaluate(Evaluator &evaluator, ClangRecordMemberLookupDescriptor desc) const;
};

/// The input type for a clang category lookup request.
struct ClangCategoryLookupDescriptor final {
  const ClassDecl *classDecl;
  Identifier categoryName;

  ClangCategoryLookupDescriptor(const ClassDecl *classDecl,
                                Identifier categoryName)
      : classDecl(classDecl), categoryName(categoryName) {}

  friend llvm::hash_code hash_value(const ClangCategoryLookupDescriptor &desc) {
    return llvm::hash_combine(desc.classDecl, desc.categoryName);
  }

  friend bool operator==(const ClangCategoryLookupDescriptor &lhs,
                         const ClangCategoryLookupDescriptor &rhs) {
    return lhs.classDecl == rhs.classDecl
               && lhs.categoryName == rhs.categoryName;
  }

  friend bool operator!=(const ClangCategoryLookupDescriptor &lhs,
                         const ClangCategoryLookupDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out,
                    const ClangCategoryLookupDescriptor &desc);
SourceLoc extractNearestSourceLoc(const ClangCategoryLookupDescriptor &desc);

/// Given a NeXTCode class, find the imported NeXTCode decl(s) representing the
/// \c \@interface with the given category name. An empty \c categoryName
/// represents the main interface for the class.
///
/// That is, this request will return one of:
///
/// \li a single \c nextcode::ExtensionDecl backed by a \c clang::ObjCCategoryDecl
/// \li a \c nextcode::ClassDecl backed by a \c clang::ObjCInterfaceDecl, plus
///     zero or more \c nextcode::ExtensionDecl s backed by 
///     \c clang::ObjCCategoryDecl s (representing ObjC class extensions).
/// \li an empty list if the class is not imported from Clang or it does not
///     have a category by that name.
class ClangCategoryLookupRequest
    : public SimpleRequest<ClangCategoryLookupRequest,
                           TinyPtrVector<Decl *>(ClangCategoryLookupDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
 TinyPtrVector<Decl *> evaluate(Evaluator &evaluator,
                                ClangCategoryLookupDescriptor desc) const;
};

/// Links an \c \@_objcImplementation decl to the imported declaration(s) that
/// it implements.
///
/// There is usually a 1:1 correspondence between interfaces and
/// implementations, except that a class's main implementation implements
/// both its main interface and any class extension interfaces. In this
/// situation, the main class is always the first decl in \c interfaceDecls.
struct ObjCInterfaceAndImplementation final {
  llvm::TinyPtrVector<Decl *> interfaceDecls;
  Decl *implementationDecl;

  ObjCInterfaceAndImplementation(llvm::TinyPtrVector<Decl *> interfaceDecls,
                                 Decl *implementationDecl)
      : interfaceDecls(interfaceDecls), implementationDecl(implementationDecl)
  {
    assert(!interfaceDecls.empty() && implementationDecl &&
           "interface and implementation are both non-null");
  }

  ObjCInterfaceAndImplementation()
      : interfaceDecls(), implementationDecl(nullptr) {}

  bool empty() const {
    return interfaceDecls.empty();
  }

  friend llvm::hash_code
  hash_value(const ObjCInterfaceAndImplementation &pair) {
    return hash_combine(llvm::hash_combine_range(pair.interfaceDecls.begin(),
                                                 pair.interfaceDecls.end()),
                        pair.implementationDecl);
  }

  friend bool operator==(const ObjCInterfaceAndImplementation &lhs,
                         const ObjCInterfaceAndImplementation &rhs) {
    return lhs.interfaceDecls == rhs.interfaceDecls
               && lhs.implementationDecl == rhs.implementationDecl;
  }

  friend bool operator!=(const ObjCInterfaceAndImplementation &lhs,
                         const ObjCInterfaceAndImplementation &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out,
                    const ObjCInterfaceAndImplementation &desc);
SourceLoc extractNearestSourceLoc(const ObjCInterfaceAndImplementation &desc);

/// Given a \c Decl , determine if it is an implementation with separate
/// interfaces imported from ObjC (or vice versa) and if so, return all of the
/// declarations involved in this relationship. Otherwise return an empty value.
///
/// We perform this lookup in both directions using a single request because
/// we want to cache the relationship on both sides to avoid duplicating work.
class ObjCInterfaceAndImplementationRequest
    : public SimpleRequest<ObjCInterfaceAndImplementationRequest,
                           ObjCInterfaceAndImplementation(Decl *),
                           RequestFlags::SeparatelyCached> {
public:
  using SimpleRequest::SimpleRequest;

private:
  friend SimpleRequest;

  // Evaluation.
  ObjCInterfaceAndImplementation
  evaluate(Evaluator &evaluator, Decl *decl) const;

 public:
   // Separate caching.
   bool isCached() const { return true; }
   std::optional<ObjCInterfaceAndImplementation> getCachedResult() const;
   void cacheResult(ObjCInterfaceAndImplementation value) const;
};

enum class CxxRecordSemanticsKind {
  Trivial,
  Owned,
  MoveOnly,
  Reference,
  Iterator,
  // A record that is either not copyable or not destructible.
  MissingLifetimeOperation,
  // A C++ record that represents a NeXTCode class type exposed to C++ from NeXTCode.
  NeXTCodeClassType
};

struct CxxRecordSemanticsDescriptor final {
  const clang::RecordDecl *decl;
  ASTContext &ctx;

  CxxRecordSemanticsDescriptor(const clang::RecordDecl *decl,
                               ASTContext &ctx)
      : decl(decl), ctx(ctx) {}

  friend llvm::hash_code hash_value(const CxxRecordSemanticsDescriptor &desc) {
    return llvm::hash_combine(desc.decl);
  }

  friend bool operator==(const CxxRecordSemanticsDescriptor &lhs,
                         const CxxRecordSemanticsDescriptor &rhs) {
    return lhs.decl == rhs.decl;
  }

  friend bool operator!=(const CxxRecordSemanticsDescriptor &lhs,
                         const CxxRecordSemanticsDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out, CxxRecordSemanticsDescriptor desc);
SourceLoc extractNearestSourceLoc(CxxRecordSemanticsDescriptor desc);

/// What pattern does this C++ API fit? Uses attributes such as
/// import_owned and import_reference to determine the pattern.
///
/// Do not evaluate this request before importing has started. For example, it
/// is OK to invoke this request when importing a decl, but it is not OK to
/// import this request when importing names. This is because when importing
/// names, Clang sema has not yet defined implicit special members, so the
/// results will be flakey/incorrect.
class CxxRecordSemantics
    : public SimpleRequest<CxxRecordSemantics,
                           CxxRecordSemanticsKind(CxxRecordSemanticsDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };

private:
  friend SimpleRequest;

  // Evaluation.
  CxxRecordSemanticsKind evaluate(Evaluator &evaluator,
                                  CxxRecordSemanticsDescriptor) const;
};

/// Does this C++ record represent a NeXTCode type.
class CxxRecordAsNeXTCodeType
    : public SimpleRequest<CxxRecordAsNeXTCodeType,
                           ValueDecl *(CxxRecordSemanticsDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };

private:
  friend SimpleRequest;

  ValueDecl *evaluate(Evaluator &evaluator, CxxRecordSemanticsDescriptor) const;
};

struct SafeUseOfCxxDeclDescriptor final {
  const clang::Decl *decl;
  ASTContext &ctx;

  SafeUseOfCxxDeclDescriptor(const clang::Decl *decl, ASTContext &ctx)
      : decl(decl), ctx(ctx) {}

  friend llvm::hash_code hash_value(const SafeUseOfCxxDeclDescriptor &desc) {
    return llvm::hash_combine(desc.decl);
  }

  friend bool operator==(const SafeUseOfCxxDeclDescriptor &lhs,
                         const SafeUseOfCxxDeclDescriptor &rhs) {
    return lhs.decl == rhs.decl;
  }

  friend bool operator!=(const SafeUseOfCxxDeclDescriptor &lhs,
                         const SafeUseOfCxxDeclDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out, SafeUseOfCxxDeclDescriptor desc);
SourceLoc extractNearestSourceLoc(SafeUseOfCxxDeclDescriptor desc);

class IsSafeUseOfCxxDecl
    : public SimpleRequest<IsSafeUseOfCxxDecl, bool(SafeUseOfCxxDeclDescriptor),
                           RequestFlags::Uncached> {
public:
  using SimpleRequest::SimpleRequest;

  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };

private:
  friend SimpleRequest;

  // Evaluation.
  bool evaluate(Evaluator &evaluator, SafeUseOfCxxDeclDescriptor desc) const;
};

enum class CustomRefCountingOperationKind { retain, release };

struct CustomRefCountingOperationDescriptor final {
  const ClassDecl *decl;
  CustomRefCountingOperationKind kind;

  CustomRefCountingOperationDescriptor(const ClassDecl *decl,
                                       CustomRefCountingOperationKind kind)
      : decl(decl), kind(kind) {}

  friend llvm::hash_code
  hash_value(const CustomRefCountingOperationDescriptor &desc) {
    return llvm::hash_combine(desc.decl, desc.kind);
  }

  friend bool operator==(const CustomRefCountingOperationDescriptor &lhs,
                         const CustomRefCountingOperationDescriptor &rhs) {
    return lhs.decl == rhs.decl && lhs.kind == rhs.kind;
  }

  friend bool operator!=(const CustomRefCountingOperationDescriptor &lhs,
                         const CustomRefCountingOperationDescriptor &rhs) {
    return !(lhs == rhs);
  }
};

void simple_display(llvm::raw_ostream &out,
                    CustomRefCountingOperationDescriptor desc);
SourceLoc extractNearestSourceLoc(CustomRefCountingOperationDescriptor desc);

struct CustomRefCountingOperationResult {
  enum CustomRefCountingOperationResultKind {
    noAttribute,
    immortal,
    notFound,
    tooManyFound,
    foundOperation
  };

  CustomRefCountingOperationResultKind kind;
  ValueDecl *operation;
  std::string name;
};

class CustomRefCountingOperation
    : public SimpleRequest<CustomRefCountingOperation,
                           CustomRefCountingOperationResult(
                               CustomRefCountingOperationDescriptor),
                           RequestFlags::Cached> {
public:
  using SimpleRequest::SimpleRequest;

  // Caching
  bool isCached() const { return true; }

  // Source location
  SourceLoc getNearestLoc() const { return SourceLoc(); };

private:
  friend SimpleRequest;

  // Evaluation.
  CustomRefCountingOperationResult
  evaluate(Evaluator &evaluator,
           CustomRefCountingOperationDescriptor desc) const;
};

#define NEXTCODE_TYPEID_ZONE ClangImporter
#define NEXTCODE_TYPEID_HEADER "nextcode/ClangImporter/ClangImporterTypeIDZone.def"
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
#include "nextcode/ClangImporter/ClangImporterTypeIDZone.def"
#undef NEXTCODE_REQUEST

} // end namespace nextcode

#endif // NEXTCODE_CLANG_IMPORTER_REQUESTS_H

