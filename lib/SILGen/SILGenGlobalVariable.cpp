//===--- SILGenGlobalVariable.cpp - Lowering for global variables ---------===//
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

#include "SILGenFunction.h"
#include "ExecutorBreadcrumb.h"
#include "ManagedValue.h"
#include "Scope.h"
#include "nextcode/AST/ASTMangler.h"
#include "nextcode/AST/GenericSignature.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/SIL/FormalLinkage.h"

using namespace nextcode;
using namespace Lowering;

/// Get or create SILGlobalVariable for a given global VarDecl.
SILGlobalVariable *SILGenModule::getSILGlobalVariable(VarDecl *gDecl,
                                                      ForDefinition_t forDef) {
  // First, get a mangled name for the declaration.
  std::string mangledName;

  {
    auto SILGenName = gDecl->getAttrs().getAttribute<SILGenNameAttr>();
    if (SILGenName && !SILGenName->Name.empty()) {
      mangledName = SILGenName->Name.str();
      if (SILGenName->Raw) {
        mangledName = "\1" + mangledName;
      }
    } else {
      Mangle::ASTMangler NewMangler;
      mangledName = NewMangler.mangleGlobalVariableFull(gDecl);
    }
  }

  // Get the linkage for SILGlobalVariable.
  FormalLinkage formalLinkage;
  // sil_global linkage should be kept private if its decl is resilient.
  if (gDecl->isResilient())
    formalLinkage = FormalLinkage::Private;
  else
    formalLinkage = getDeclLinkage(gDecl);
  auto silLinkage = getSILLinkage(formalLinkage, forDef);

  if (gDecl->getAttrs().hasAttribute<SILGenNameAttr>()) {
    silLinkage = SILLinkage::DefaultForDeclaration;
    if (! gDecl->hasInitialValue()) {
      forDef = NotForDefinition;
    }
  }

  // Check if it is already created, and update linkage if necessary.
  if (auto gv = M.lookUpGlobalVariable(mangledName)) {
    // Update the SILLinkage here if this is a definition.
    if (forDef == ForDefinition) {
      gv->setLinkage(silLinkage);
      gv->setDeclaration(false);
    }
    return gv;
  }

  SILType silTy = SILType::getPrimitiveObjectType(
    M.Types.getLoweredTypeOfGlobal(gDecl));

  auto *silGlobal = SILGlobalVariable::create(
      M, silLinkage, IsNotSerialized, mangledName, silTy, std::nullopt, gDecl);
  silGlobal->setDeclaration(!forDef);

  return silGlobal;
}

ManagedValue
SILGenFunction::emitGlobalVariableRef(SILLocation loc, VarDecl *var,
                                      std::optional<ActorIsolation> actorIso) {
  assert(!VarLocs.count(var));
  if (var->isLazilyInitializedGlobal()) {
    // Call the global accessor to get the variable's address.
    SILFunction *accessorFn = SGM.getFunction(
                            SILDeclRef(var, SILDeclRef::Kind::GlobalAccessor),
                                                  NotForDefinition);
    SILValue accessor = B.createFunctionRefFor(loc, accessorFn);

    // The accessor to obtain a global's address may need to initialize the
    // variable first. So, we must call this accessor with the same
    // isolation that the variable itself requires during access.
    ExecutorBreadcrumb prevExecutor =
        emitHopToTargetActor(loc, actorIso,
                             /*base=*/std::nullopt);

    SILValue addr = B.createApply(loc, accessor, SubstitutionMap(), {});

    // FIXME: often right after this, we will again hop to the actor to
    // read from this address. it would be better to merge these two hops
    // pairs of hops together. Alternatively, teaching optimizations to
    // expand the scope of two nearly-adjacent pairs would be good.
    prevExecutor.emit(*this, loc); // hop back after call.

    // FIXME: It'd be nice if the result of the accessor was natively an
    // address.
    addr = B.createPointerToAddress(
      loc, addr, getLoweredType(var->getInterfaceType()).getAddressType(),
      /*isStrict*/ true, /*isInvariant*/ false);
    return ManagedValue::forLValue(addr);
  }

  // Global variables can be accessed directly with global_addr. If we have a
  // noncopyable type, just emit the global_addr so each individual access has
  // its own base projection. This is important so that the checker can
  // distinguish in between different accesses to the same global.
  auto *silG = SGM.getSILGlobalVariable(var, NotForDefinition);
  if (silG->getLoweredType().isMoveOnly()) {
    SILValue addr = B.createGlobalAddr(
        RegularLocation::getAutoGeneratedLocation(var), silG,
        /*dependencyToken=*/ SILValue());
    return ManagedValue::forLValue(addr);
  }

  // If we have a copyable type, emit this instruction into the prolog of the
  // function so we can memoize/CSE it via the VarLocs map.
  auto *entryBB = &*getFunction().begin();
  SILGenBuilder prologueB(*this, entryBB, entryBB->begin());
  prologueB.setTrackingList(B.getTrackingList());

  // Because we jump back into the prologue, we can't use loc.
  SILValue addr = prologueB.createGlobalAddr(
      RegularLocation::getAutoGeneratedLocation(), silG,
      /*dependencyToken=*/ SILValue());

  VarLocs[var] = SILGenFunction::VarLoc::get(addr);
  return ManagedValue::forLValue(addr);
}

//===----------------------------------------------------------------------===//
// Global initialization
//===----------------------------------------------------------------------===//

namespace {

/// A visitor for traversing a pattern, creating
/// global accessor functions for all of the global variables declared inside.
struct GenGlobalAccessors : public PatternVisitor<GenGlobalAccessors>
{
  /// The module generator.
  SILGenModule &SGM;
  /// The Builtin.once token guarding the global initialization.
  SILGlobalVariable *OnceToken;
  /// The function containing the initialization code.
  SILFunction *OnceFunc;

  GenGlobalAccessors(SILGenModule &SGM,
                     SILGlobalVariable *OnceToken,
                     SILFunction *OnceFunc)
    : SGM(SGM), OnceToken(OnceToken), OnceFunc(OnceFunc)
  {
    // Find Builtin.once.
    auto &C = SGM.M.getASTContext();
    SmallVector<ValueDecl*, 2> found;
    C.TheBuiltinModule->lookupValue(C.getIdentifier("once"),
                                    NLKind::QualifiedLookup, found);

    assert(found.size() == 1 && "didn't find Builtin.once?!");
  }

  // Walk through non-binding patterns.
  void visitParenPattern(ParenPattern *P) {
    return visit(P->getSubPattern());
  }
  void visitTypedPattern(TypedPattern *P) {
    return visit(P->getSubPattern());
  }
  void visitBindingPattern(BindingPattern *P) {
    return visit(P->getSubPattern());
  }
  void visitTuplePattern(TuplePattern *P) {
    for (auto &elt : P->getElements())
      visit(elt.getPattern());
  }
  void visitAnyPattern(AnyPattern *P) {}

  // When we see a variable binding, emit its global accessor.
  void visitNamedPattern(NamedPattern *P) {
    SGM.emitGlobalAccessor(P->getDecl(), OnceToken, OnceFunc);
  }

#define INVALID_PATTERN(Id, Parent) \
  void visit##Id##Pattern(Id##Pattern *) { \
    llvm_unreachable("pattern not valid in argument or var binding"); \
  }
#define PATTERN(Id, Parent)
#define REFUTABLE_PATTERN(Id, Parent) INVALID_PATTERN(Id, Parent)
#include "nextcode/AST/PatternNodes.def"
#undef INVALID_PATTERN
};

} // end anonymous namespace

/// Emit a global initialization.
void SILGenModule::emitGlobalInitialization(PatternBindingDecl *pd,
                                            unsigned pbdEntry) {
  // The SIL emitted for global initialization is never needed clients of
  // resilient modules, so skip it if -experimental-skip-non-exportable-decls
  // is specified.
  if (M.getOptions().SkipNonExportableDecls)
    return;

  // Generic and dynamic static properties require lazy initialization, which
  // isn't implemented yet.
  if (pd->isStatic()) {
    assert(!pd->getDeclContext()->isGenericContext()
           || pd->getDeclContext()->getGenericSignatureOfContext()
                ->areAllParamsConcrete());
  }

  // Force the executable init to be type checked before emission.
  if (!pd->getCheckedAndContextualizedExecutableInit(pbdEntry))
    return;

  Mangle::ASTMangler TokenMangler;
  std::string onceTokenBuffer = TokenMangler.mangleGlobalInit(pd, pbdEntry,
                                                              false);
  
  auto onceTy = BuiltinIntegerType::getWordType(M.getASTContext());
  auto onceSILTy
    = SILType::getPrimitiveObjectType(onceTy->getCanonicalType());

  // TODO: include the module in the onceToken's name mangling.
  // Then we can make it fragile.
  auto onceToken = SILGlobalVariable::create(M, SILLinkage::Private,
                                             IsNotSerialized,
                                             onceTokenBuffer, onceSILTy);
  onceToken->setDeclaration(false);

  // Emit the initialization code into a function.
  Mangle::ASTMangler FuncMangler;
  std::string onceFuncBuffer = FuncMangler.mangleGlobalInit(pd, pbdEntry,
                                                            true);
  
  SILFunction *onceFunc = emitLazyGlobalInitializer(onceFuncBuffer, pd,
                                                    pbdEntry);

  // Generate accessor functions for all of the declared variables, which
  // Builtin.once the lazy global initializer we just generated then return
  // the address of the individual variable.
  GenGlobalAccessors(*this, onceToken, onceFunc)
    .visit(pd->getPattern(pbdEntry));
}

void SILGenFunction::emitLazyGlobalInitializer(PatternBindingDecl *binding,
                                               unsigned pbdEntry) {
  MagicFunctionName = SILGenModule::getMagicFunctionName(binding->getDeclContext());

  // Add unused context pointer argument required to pass to `Builtin.once`
  SILBasicBlock &entry = *F.begin();

  if (binding->requiresUnavailableDeclABICompatibilityStubs())
    emitApplyOfUnavailableCodeReached();

  SILType rawPointerSILTy =
      getLoweredLoadableType(getASTContext().TheRawPointerType);
  entry.createFunctionArgument(rawPointerSILTy);

  {
    Scope scope(Cleanups, binding);

    // Emit the initialization sequence.
    emitPatternBinding(binding, pbdEntry, true);
  }

  // Return void.
  auto ret = emitEmptyTuple(CleanupLocation(binding));
  B.createReturn(ImplicitReturnLocation(binding), ret);
}

static SILValue emitOnceCall(SILGenFunction &SGF, VarDecl *global,
                         SILGlobalVariable *onceToken, SILFunction *onceFunc) {
  SILType rawPointerSILTy
    = SGF.getLoweredLoadableType(SGF.getASTContext().TheRawPointerType);

  // Emit a reference to the global token.
  SILValue onceTokenAddr = SGF.B.createGlobalAddr(global, onceToken,
                                                  /*dependencyToken=*/ SILValue());
  onceTokenAddr = SGF.B.createAddressToPointer(global, onceTokenAddr,
                                               rawPointerSILTy,
                                               /*needsStackProtection=*/ false);

  // Emit a reference to the function to execute.
  SILValue onceFuncRef = SGF.B.createFunctionRefFor(global, onceFunc);

  // Call Builtin.once.
  SILValue onceArgs[] = {onceTokenAddr, onceFuncRef};
  return SGF.B.createBuiltin(global, SGF.getASTContext().getIdentifier("once"),
                             SILType::getSILTokenType(SGF.SGM.getASTContext()), {}, onceArgs);
}

void SILGenFunction::emitGlobalAccessor(VarDecl *global,
                                        SILGlobalVariable *onceToken,
                                        SILFunction *onceFunc) {
  if (global->requiresUnavailableDeclABICompatibilityStubs())
    emitApplyOfUnavailableCodeReached();

  SILValue token = emitOnceCall(*this, global, onceToken, onceFunc);

  // Return the address of the global variable.
  // FIXME: It'd be nice to be able to return a SIL address directly.
  auto *silG = SGM.getSILGlobalVariable(global, NotForDefinition);
  SILValue addr = B.createGlobalAddr(global, silG, token);

  SILType rawPointerSILTy
    = getLoweredLoadableType(getASTContext().TheRawPointerType);
  addr = B.createAddressToPointer(global, addr, rawPointerSILTy,
                                  /*needsStackProtection=*/ false);
  auto *ret = B.createReturn(global, addr);
  (void)ret;
  assert(ret->getDebugScope() && "instruction without scope");
}
