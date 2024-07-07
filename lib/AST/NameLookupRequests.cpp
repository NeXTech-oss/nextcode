//===--- NameLookupRequests.cpp - Name Lookup Requests --------------------===//
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

#include "nextcode/AST/NameLookupRequests.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/Evaluator.h"
#include "nextcode/AST/GenericParamList.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/NameLookup.h"
#include "nextcode/AST/PotentialMacroExpansions.h"
#include "nextcode/AST/ProtocolConformance.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/AST/TypeCheckRequests.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/ClangImporter/ClangImporterRequests.h"
#include "nextcode/Subsystems.h"

using namespace nextcode;

namespace nextcode {
// Implement the name lookup type zone.
#define NEXTCODE_TYPEID_ZONE NameLookup
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/NameLookupTypeIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
}

//----------------------------------------------------------------------------//
// Referenced inherited decls computation.
//----------------------------------------------------------------------------//

SourceLoc InheritedDeclsReferencedRequest::getNearestLoc() const {
  const auto &storage = getStorage();
  auto inheritedTypes = InheritedTypes(std::get<0>(storage));
  return inheritedTypes.getEntry(std::get<1>(storage)).getLoc();
}

//----------------------------------------------------------------------------//
// Superclass declaration computation.
//----------------------------------------------------------------------------//
void SuperclassDeclRequest::diagnoseCycle(DiagnosticEngine &diags) const {
  // FIXME: Improve this diagnostic.
  auto nominalDecl = std::get<0>(getStorage());
  diags.diagnose(nominalDecl, diag::circular_class_inheritance,
                 nominalDecl->getName());
}

void SuperclassDeclRequest::noteCycleStep(DiagnosticEngine &diags) const {
  auto decl = std::get<0>(getStorage());
  diags.diagnose(decl, diag::kind_declname_declared_here,
                 decl->getDescriptiveKind(), decl->getName());
}

std::optional<ClassDecl *> SuperclassDeclRequest::getCachedResult() const {
  auto nominalDecl = std::get<0>(getStorage());

  if (auto *classDecl = dyn_cast<ClassDecl>(nominalDecl))
    if (classDecl->LazySemanticInfo.SuperclassDecl.getInt())
      return classDecl->LazySemanticInfo.SuperclassDecl.getPointer();

  if (auto *protocolDecl = dyn_cast<ProtocolDecl>(nominalDecl))
    if (protocolDecl->LazySemanticInfo.SuperclassDecl.getInt())
      return protocolDecl->LazySemanticInfo.SuperclassDecl.getPointer();

  return std::nullopt;
}

void SuperclassDeclRequest::cacheResult(ClassDecl *value) const {
  auto nominalDecl = std::get<0>(getStorage());

  if (auto *classDecl = dyn_cast<ClassDecl>(nominalDecl))
    classDecl->LazySemanticInfo.SuperclassDecl.setPointerAndInt(value, true);

  if (auto *protocolDecl = dyn_cast<ProtocolDecl>(nominalDecl))
    protocolDecl->LazySemanticInfo.SuperclassDecl.setPointerAndInt(value, true);
}

//----------------------------------------------------------------------------//
// InheritedProtocolsRequest computation.
//----------------------------------------------------------------------------//

std::optional<ArrayRef<ProtocolDecl *>>
InheritedProtocolsRequest::getCachedResult() const {
  auto proto = std::get<0>(getStorage());
  if (!proto->areInheritedProtocolsValid())
    return std::nullopt;

  return proto->InheritedProtocols;
}

void InheritedProtocolsRequest::cacheResult(ArrayRef<ProtocolDecl *> PDs) const {
  auto proto = std::get<0>(getStorage());
  proto->InheritedProtocols = PDs;
  proto->setInheritedProtocolsValid();
}

void InheritedProtocolsRequest::writeDependencySink(
    evaluator::DependencyCollector &tracker,
    ArrayRef<ProtocolDecl *> PDs) const {
  for (auto *parentProto : PDs) {
    tracker.addPotentialMember(parentProto);
  }
}

//----------------------------------------------------------------------------//
// AllInheritedProtocolsRequest computation.
//----------------------------------------------------------------------------//

std::optional<ArrayRef<ProtocolDecl *>>
AllInheritedProtocolsRequest::getCachedResult() const {
  auto proto = std::get<0>(getStorage());
  if (!proto->areAllInheritedProtocolsValid())
    return std::nullopt;

  return proto->AllInheritedProtocols;
}

void AllInheritedProtocolsRequest::cacheResult(ArrayRef<ProtocolDecl *> PDs) const {
  auto proto = std::get<0>(getStorage());
  proto->AllInheritedProtocols = PDs;
  proto->setAllInheritedProtocolsValid();
}

//----------------------------------------------------------------------------//
// ProtocolRequirementsRequest computation.
//----------------------------------------------------------------------------//

std::optional<ArrayRef<ValueDecl *>>
ProtocolRequirementsRequest::getCachedResult() const {
  auto proto = std::get<0>(getStorage());
  if (!proto->areProtocolRequirementsValid())
    return std::nullopt;

  return proto->ProtocolRequirements;
}

void ProtocolRequirementsRequest::cacheResult(ArrayRef<ValueDecl *> PDs) const {
  auto proto = std::get<0>(getStorage());
  proto->ProtocolRequirements = PDs;
  proto->setProtocolRequirementsValid();
}

//----------------------------------------------------------------------------//
// Missing designated initializers computation
//----------------------------------------------------------------------------//

std::optional<bool>
HasMissingDesignatedInitializersRequest::getCachedResult() const {
  auto classDecl = std::get<0>(getStorage());
  return classDecl->getCachedHasMissingDesignatedInitializers();
}

void HasMissingDesignatedInitializersRequest::cacheResult(bool result) const {
  auto classDecl = std::get<0>(getStorage());
  classDecl->setHasMissingDesignatedInitializers(result);
}

bool
HasMissingDesignatedInitializersRequest::evaluate(Evaluator &evaluator,
                                           ClassDecl *subject) const {
  // Short-circuit and check for the attribute here.
  if (subject->getAttrs().hasAttribute<HasMissingDesignatedInitializersAttr>())
    return true;

  AccessScope scope =
    subject->getFormalAccessScope(/*useDC*/nullptr,
                                  /*treatUsableFromInlineAsPublic*/true);
  // This flag only makes sense for public types that will be written in the
  // module.
  if (!scope.isPublic())
    return false;

  // Make sure any implicit constructors are synthesized.
  (void)evaluateOrDefault(
      evaluator,
      ResolveImplicitMemberRequest{subject,
                                   ImplicitMemberAction::ResolveImplicitInit},
      {});

  auto constructors = subject->lookupDirect(DeclBaseName::createConstructor());
  return llvm::any_of(constructors, [&](ValueDecl *decl) {
    auto init = cast<ConstructorDecl>(decl);
    if (!init->isDesignatedInit())
      return false;
    AccessScope scope =
        init->getFormalAccessScope(/*useDC*/nullptr,
                                   /*treatUsableFromInlineAsPublic*/true);
    return !scope.isPublic();
  });
}

//----------------------------------------------------------------------------//
// Extended nominal computation.
//----------------------------------------------------------------------------//

std::optional<NominalTypeDecl *>
ExtendedNominalRequest::getCachedResult() const {
  // Note: if we fail to compute any nominal declaration, it's considered
  // a cache miss. This allows us to recompute the extended nominal types
  // during extension binding.
  // This recomputation is also what allows you to extend types defined inside
  // other extensions, regardless of source file order. See \c bindExtensions(),
  // which uses a worklist algorithm that attempts to bind everything until
  // fixed point.
  auto ext = std::get<0>(getStorage());
  if (!ext->hasBeenBound() || !ext->getExtendedNominal())
    return std::nullopt;
  return ext->getExtendedNominal();
}

void ExtendedNominalRequest::cacheResult(NominalTypeDecl *value) const {
  auto ext = std::get<0>(getStorage());
  ext->setExtendedNominal(value);
}

void ExtendedNominalRequest::writeDependencySink(
    evaluator::DependencyCollector &tracker,
    NominalTypeDecl *value) const {
  if (!value)
    return;

  // Ensure this extension comes from a source file.
  auto *SF = std::get<0>(getStorage())->getParentSourceFile();
  if (!SF)
    return;

  tracker.addPotentialMember(value);
}

//----------------------------------------------------------------------------//
// Destructor computation.
//----------------------------------------------------------------------------//

std::optional<DestructorDecl *> GetDestructorRequest::getCachedResult() const {
  auto *classDecl = std::get<0>(getStorage());
  auto results = classDecl->lookupDirect(DeclBaseName::createDestructor());
  if (results.empty())
    return std::nullopt;

  return cast<DestructorDecl>(results.front());
}

void GetDestructorRequest::cacheResult(DestructorDecl *value) const {
  auto *classDecl = std::get<0>(getStorage());
  classDecl->getImplementationContext()->addMember(value);
}

//----------------------------------------------------------------------------//
// GenericParamListRequest computation.
//----------------------------------------------------------------------------//

std::optional<GenericParamList *>
GenericParamListRequest::getCachedResult() const {
  using GenericParamsState = GenericContext::GenericParamsState;
  auto *decl = std::get<0>(getStorage());
  switch (decl->GenericParamsAndState.getInt()) {
  case GenericParamsState::TypeChecked:
  case GenericParamsState::ParsedAndTypeChecked:
    return decl->GenericParamsAndState.getPointer();

  case GenericParamsState::Parsed:
    return std::nullopt;
  }
}

void GenericParamListRequest::cacheResult(GenericParamList *params) const {
  using GenericParamsState = GenericContext::GenericParamsState;
  auto *context = std::get<0>(getStorage());
  if (params)
    params->setDeclContext(context);

  assert(context->GenericParamsAndState.getInt() == GenericParamsState::Parsed);
  bool hadParsedGenericParams =
  context->GenericParamsAndState.getPointer() != nullptr;
  auto newState = hadParsedGenericParams
      ? GenericParamsState::ParsedAndTypeChecked
      : GenericParamsState::TypeChecked;
  context->GenericParamsAndState.setPointerAndInt(params, newState);
}

//----------------------------------------------------------------------------//
// UnqualifiedLookupRequest computation.
//----------------------------------------------------------------------------//

void nextcode::simple_display(llvm::raw_ostream &out,
                           const UnqualifiedLookupDescriptor &desc) {
  out << "looking up ";
  simple_display(out, desc.Name);
  out << " from ";
  simple_display(out, desc.DC);
  out << " with options ";
  simple_display(out, desc.Options);
}

SourceLoc
nextcode::extractNearestSourceLoc(const UnqualifiedLookupDescriptor &desc) {
  return extractNearestSourceLoc(desc.DC);
}

//----------------------------------------------------------------------------//
// DirectLookupRequest computation.
//----------------------------------------------------------------------------//

void nextcode::simple_display(llvm::raw_ostream &out,
                           const DirectLookupDescriptor &desc) {
  out << "directly looking up ";
  simple_display(out, desc.Name);
  out << " on ";
  simple_display(out, desc.DC);
  out << " with options ";
  simple_display(out, desc.Options);
}

SourceLoc nextcode::extractNearestSourceLoc(const DirectLookupDescriptor &desc) {
  return extractNearestSourceLoc(desc.DC);
}

//----------------------------------------------------------------------------//
// LookupOperatorRequest computation.
//----------------------------------------------------------------------------//

OperatorLookupDescriptor OperatorLookupDescriptor::forDC(const DeclContext *DC,
                                                         Identifier name) {
  auto *moduleDC = DC->getModuleScopeContext();
  if (auto *file = dyn_cast<FileUnit>(moduleDC)) {
    return OperatorLookupDescriptor::forFile(file, name);
  } else {
    auto *mod = cast<ModuleDecl>(moduleDC->getAsDecl());
    return OperatorLookupDescriptor::forModule(mod, name);
  }
}

ArrayRef<FileUnit *> OperatorLookupDescriptor::getFiles() const {
  if (auto *module = getModule())
    return module->getFiles();

  // Return an ArrayRef pointing to the FileUnit in the union.
  return llvm::ArrayRef(*fileOrModule.getAddrOfPtr1());
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const OperatorLookupDescriptor &desc) {
  out << "looking up operator ";
  simple_display(out, desc.name);
  out << " in ";
  simple_display(out, desc.fileOrModule);
}

SourceLoc nextcode::extractNearestSourceLoc(const OperatorLookupDescriptor &desc) {
  return extractNearestSourceLoc(desc.fileOrModule);
}

void DirectLookupRequest::writeDependencySink(
    evaluator::DependencyCollector &tracker,
    const TinyPtrVector<ValueDecl *> &result) const {
  auto &desc = std::get<0>(getStorage());
  // Add used members from the perspective of
  // 1) The decl context they are found in
  // 2) The decl context of the request
  // This gets us a dependency not just on `Foo.bar` but on `extension Foo.bar`.
  for (const auto *member : result) {
    tracker.addUsedMember(member->getDeclContext(), desc.Name.getBaseName());
  }
  tracker.addUsedMember(desc.DC, desc.Name.getBaseName());
}

//----------------------------------------------------------------------------//
// LookupInModuleRequest computation.
//----------------------------------------------------------------------------//

void LookupInModuleRequest::writeDependencySink(
    evaluator::DependencyCollector &reqTracker,
    const QualifiedLookupResult &l) const {
  auto *DC = std::get<0>(getStorage());
  auto member = std::get<1>(getStorage());

  // Decline to record lookups if the module in question has no incremental
  // dependency information available.
  auto *module = DC->getParentModule();
  if (module->isMainModule() || module->hasIncrementalInfo()) {
    reqTracker.addTopLevelName(member.getBaseName());
  }
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const LookupConformanceDescriptor &desc) {
  out << "looking up conformance to ";
  simple_display(out, desc.PD);
  out << " for ";
  out << desc.Ty.getString();
  out << " in ";
  simple_display(out, desc.Mod);
}

//----------------------------------------------------------------------------//
// AnyObjectLookupRequest computation.
//----------------------------------------------------------------------------//

void AnyObjectLookupRequest::writeDependencySink(
    evaluator::DependencyCollector &reqTracker,
    const QualifiedLookupResult &l) const {
  auto member = std::get<1>(getStorage());
  reqTracker.addDynamicLookupName(member.getBaseName());
}

SourceLoc
nextcode::extractNearestSourceLoc(const LookupConformanceDescriptor &desc) {
  return SourceLoc();
}

//----------------------------------------------------------------------------//
// ModuleQualifiedLookupRequest computation.
//----------------------------------------------------------------------------//

void ModuleQualifiedLookupRequest::writeDependencySink(
    evaluator::DependencyCollector &reqTracker,
    const QualifiedLookupResult &l) const {
  auto *module = std::get<1>(getStorage());
  auto member = std::get<2>(getStorage());

  // Decline to record lookups if the module in question has no incremental
  // dependency information available.
  if (module->isMainModule() || module->hasIncrementalInfo()) {
    reqTracker.addTopLevelName(member.getBaseName());
  }
}

//----------------------------------------------------------------------------//
// LookupConformanceInModuleRequest computation.
//----------------------------------------------------------------------------//

void LookupConformanceInModuleRequest::writeDependencySink(
    evaluator::DependencyCollector &reqTracker,
    ProtocolConformanceRef lookupResult) const {
  if (lookupResult.isInvalid() || !lookupResult.isConcrete())
    return;

  auto &desc = std::get<0>(getStorage());
  auto *Adoptee = desc.Ty->getAnyNominal();
  if (!Adoptee)
    return;

  // Decline to record lookups if the module in question has no incremental
  // dependency information available.
  auto *conformance = lookupResult.getConcrete();
  auto *module = conformance->getDeclContext()->getParentModule();
  if (module->isMainModule() || module->hasIncrementalInfo()) {
    reqTracker.addPotentialMember(Adoptee);
  }
}

//----------------------------------------------------------------------------//
// UnqualifiedLookupRequest computation.
//----------------------------------------------------------------------------//

void UnqualifiedLookupRequest::writeDependencySink(
    evaluator::DependencyCollector &track,
    const LookupResult &res) const {
  auto &desc = std::get<0>(getStorage());
  track.addTopLevelName(desc.Name.getBaseName());
}

// The following clang importer requests have some definitions here to prevent
// linker errors when building lib syntax parser (which doesn't link with the
// clang importer).

//----------------------------------------------------------------------------//
// ClangDirectLookupRequest computation.
//----------------------------------------------------------------------------//

void nextcode::simple_display(llvm::raw_ostream &out,
                           const ClangDirectLookupDescriptor &desc) {
  out << "Looking up ";
  simple_display(out, desc.name);
  out << " in ";
  simple_display(out, desc.decl);
}

SourceLoc
nextcode::extractNearestSourceLoc(const ClangDirectLookupDescriptor &desc) {
  return extractNearestSourceLoc(desc.decl);
}

//----------------------------------------------------------------------------//
// CXXNamespaceMemberLookup computation.
//----------------------------------------------------------------------------//

void nextcode::simple_display(llvm::raw_ostream &out,
                           const CXXNamespaceMemberLookupDescriptor &desc) {
  out << "Looking up ";
  simple_display(out, desc.name);
  out << " in ";
  simple_display(out, desc.namespaceDecl);
}

SourceLoc
nextcode::extractNearestSourceLoc(const CXXNamespaceMemberLookupDescriptor &desc) {
  return extractNearestSourceLoc(desc.namespaceDecl);
}

//----------------------------------------------------------------------------//
// ClangRecordMemberLookup computation.
//----------------------------------------------------------------------------//

void nextcode::simple_display(llvm::raw_ostream &out,
                           const ClangRecordMemberLookupDescriptor &desc) {
  out << "Looking up ";
  simple_display(out, desc.name);
  out << " in ";
  simple_display(out, desc.recordDecl);
}

SourceLoc
nextcode::extractNearestSourceLoc(const ClangRecordMemberLookupDescriptor &desc) {
  return extractNearestSourceLoc(desc.recordDecl);
}

//----------------------------------------------------------------------------//
// CustomRefCountingOperation computation.
//----------------------------------------------------------------------------//

void nextcode::simple_display(llvm::raw_ostream &out,
                           CustomRefCountingOperationDescriptor desc) {
  out << "Finding custom (foreign reference) reference counting operation '"
      << (desc.kind == CustomRefCountingOperationKind::retain ? "retain"
                                                              : "release")
      << "' for '" << desc.decl->getNameStr() << "'.\n";
}

SourceLoc
nextcode::extractNearestSourceLoc(CustomRefCountingOperationDescriptor desc) {
  return SourceLoc();
}

//----------------------------------------------------------------------------//
// Macro-related adjustments to name lookup requests.
//----------------------------------------------------------------------------//
//
// Macros introduced a significant wrinkle into NeXTCode's name lookup mechanism.
// Specifically, when resolving names (and, really, anything else) within the
// arguments to a macro expansion, name lookup must not try to expand any
// macros, because doing so trivially creates a cyclic dependency amongst the
// macro expansions that will be detected by the request-evaluator.
//
// We use source locations to answer the question "is this part of an argument
// to a macro?" through `namelookup::isInMacroArgument`. If the answer is yes,
// then we adjust the options used for the name lookup request we are forming
// to exclude macro expansions. The evaluation of that request will then avoid
// expanding any macros, and not produce any results that involve entries in
// already-expanded macros. By adjusting the request itself, we still
// distinguish between requests that can and cannot look into macro expansions,
// so it doesn't break caching for those immediate requests.

/// Exclude macros in the unqualified lookup descriptor if we need to.
static UnqualifiedLookupDescriptor excludeMacrosIfNeeded(
    UnqualifiedLookupDescriptor descriptor) {
  if (descriptor.Options.contains(
          UnqualifiedLookupFlags::ExcludeMacroExpansions))
    return descriptor;

  auto isInMacroArgument = namelookup::isInMacroArgument(
      descriptor.DC->getParentSourceFile(), descriptor.Loc);

  if (!isInMacroArgument)
    return descriptor;

  descriptor.Options |= UnqualifiedLookupFlags::ExcludeMacroExpansions;
  return descriptor;
}

/// Exclude macros in the direct lookup descriptor if we need to.
static DirectLookupDescriptor excludeMacrosIfNeeded(
    DirectLookupDescriptor descriptor, SourceLoc loc) {
  if (descriptor.Options.contains(
          NominalTypeDecl::LookupDirectFlags::ExcludeMacroExpansions))
    return descriptor;

  auto isInMacroArgument = namelookup::isInMacroArgument(
      descriptor.DC->getParentSourceFile(), loc);

  if (!isInMacroArgument)
    return descriptor;

  descriptor.Options |=
      NominalTypeDecl::LookupDirectFlags::ExcludeMacroExpansions;

  return descriptor;
}

/// Exclude macros in the name lookup options if we need to.
static NLOptions
excludeMacrosIfNeeded(const DeclContext *dc, SourceLoc loc,
                      NLOptions options) {
  if (options & NL_ExcludeMacroExpansions)
    return options;

  auto isInMacroArgument = namelookup::isInMacroArgument(
      dc->getParentSourceFile(), loc);

  if (!isInMacroArgument)
    return options;

  return options | NL_ExcludeMacroExpansions;
}

UnqualifiedLookupRequest::UnqualifiedLookupRequest(
    UnqualifiedLookupDescriptor descriptor
) : SimpleRequest(excludeMacrosIfNeeded(descriptor)) { }

LookupInModuleRequest::LookupInModuleRequest(
      const DeclContext *moduleOrFile, DeclName name, NLKind lookupKind,
      namelookup::ResolutionKind resolutionKind,
      const DeclContext *moduleScopeContext,
      SourceLoc loc, NLOptions options
 ) : SimpleRequest(moduleOrFile, name, lookupKind, resolutionKind,
                   moduleScopeContext,
                   excludeMacrosIfNeeded(moduleOrFile, loc, options)) { }

ModuleQualifiedLookupRequest::ModuleQualifiedLookupRequest(
    const DeclContext *dc, ModuleDecl *module, DeclNameRef name,
    SourceLoc loc, NLOptions options
 ) : SimpleRequest(dc, module, name,
                   excludeMacrosIfNeeded(dc, loc, options)) { }

QualifiedLookupRequest::QualifiedLookupRequest(
                       const DeclContext *dc,
                       SmallVector<NominalTypeDecl *, 4> decls,
                       DeclNameRef name,
                       SourceLoc loc, NLOptions options
) : SimpleRequest(dc, std::move(decls), name,
                  excludeMacrosIfNeeded(dc, loc, options)) { }

DirectLookupRequest::DirectLookupRequest(DirectLookupDescriptor descriptor, SourceLoc loc)
    : SimpleRequest(excludeMacrosIfNeeded(descriptor, loc)) { }

// Implement the clang importer type zone.
#define NEXTCODE_TYPEID_ZONE ClangImporter
#define NEXTCODE_TYPEID_HEADER "nextcode/ClangImporter/ClangImporterTypeIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER

// Define request evaluation functions for each of the name lookup requests.
static AbstractRequestFunction *nameLookupRequestFunctions[] = {
#define NEXTCODE_REQUEST(Zone, Name, Sig, Caching, LocOptions)                    \
  reinterpret_cast<AbstractRequestFunction *>(&Name::evaluateRequest),
#include "nextcode/AST/NameLookupTypeIDZone.def"
#undef NEXTCODE_REQUEST
};

void nextcode::registerNameLookupRequestFunctions(Evaluator &evaluator) {
  evaluator.registerRequestFunctions(Zone::NameLookup,
                                     nameLookupRequestFunctions);
}
