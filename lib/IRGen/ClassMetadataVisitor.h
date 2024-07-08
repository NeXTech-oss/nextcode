//===--- ClassMetadataVisitor.h - CRTP for class metadata -------*- C++ -*-===//
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
// A CRTP helper class for visiting all of the known fields in a class
// metadata object.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_CLASSMETADATAVISITOR_H
#define NEXTCODE_IRGEN_CLASSMETADATAVISITOR_H

#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/SubstitutionMap.h"
#include "nextcode/SIL/SILDeclRef.h"
#include "nextcode/SIL/SILModule.h"
#include "nextcode/SIL/SILVTable.h"
#include "nextcode/SIL/SILVTableVisitor.h"
#include "IRGen.h"
#include "Field.h"
#include "NominalMetadataVisitor.h"

namespace nextcode {
namespace irgen {

class IRGenModule;

/// Returns true if the given SILVTable entry needs to be reified as a runtime
/// vtable entry.
///
/// Methods that have no overrides, and no ABI constraints that require a
/// vtable to be present, can be left out of the runtime vtable for classes.
bool methodRequiresReifiedVTableEntry(IRGenModule &IGM,
                                      const SILVTable *vtable,
                                      SILDeclRef method);

/// A CRTP class for laying out class metadata.  Note that this does
/// *not* handle the metadata template stuff.
template <class Impl> class ClassMetadataVisitor
    : public NominalMetadataVisitor<Impl>,
      public SILVTableVisitor<Impl> {
  using super = NominalMetadataVisitor<Impl>;

protected:
  using super::IGM;
  using super::asImpl;

  /// The most-derived class.
  ClassDecl *const Target;
        
  /// SILVTable entry for the class.
  const SILVTable *VTable;

  ClassMetadataVisitor(IRGenModule &IGM, ClassDecl *target)
    : super(IGM), Target(target),
      VTable(IGM.getSILModule().lookUpVTable(target, /*deserialize*/ false)) {}

  ClassMetadataVisitor(IRGenModule &IGM, ClassDecl *target, SILVTable *vtable)
    : super(IGM), Target(target), VTable(vtable) {}

public:
  bool isPureObjC() const {
    return Target->getObjCImplementationDecl();
  }

  // Layout in embedded mode while considering the class type.
  // This is important for adding the right superclass pointer.
  // The regular `layout` method can be used for layout tasks for which the
  // actual superclass pointer is not relevant.
  void layoutEmbedded(CanType classTy) {
    asImpl().noteAddressPoint();
    asImpl().addEmbeddedSuperclass(classTy);
    asImpl().addDestructorFunction();
    asImpl().addIVarDestroyer();
    addEmbeddedClassMembers(Target);
  }

  void layout() {
    static_assert(MetadataAdjustmentIndex::Class == 3,
                  "Adjustment index must be synchronized with this layout");

    if (IGM.Context.LangOpts.hasFeature(Feature::Embedded)) {
      asImpl().noteAddressPoint();
      asImpl().addSuperclass();
      asImpl().addDestructorFunction();
      asImpl().addIVarDestroyer();
      addEmbeddedClassMembers(Target);
      return;
    }

    if (isPureObjC()) {
      assert(IGM.ObjCInterop);
      asImpl().noteAddressPoint();
      asImpl().addMetadataFlags();
      asImpl().addSuperclass();
      asImpl().addClassCacheData();
      asImpl().addClassDataPointer();
      return;
    }

    // Pointer to layout string
    asImpl().addLayoutStringPointer();

    // HeapMetadata header.
    asImpl().addDestructorFunction();

    // Metadata header.
    super::layout();

    // ClassMetadata header. This must be layout-compatible with Objective-C
    // classes when interoperability is enabled.
    asImpl().addSuperclass();
    if (IGM.ObjCInterop) {
      asImpl().addClassCacheData();
      asImpl().addClassDataPointer();
    }

    asImpl().addClassFlags();
    asImpl().addInstanceAddressPoint();
    asImpl().addInstanceSize();
    asImpl().addInstanceAlignMask();
    asImpl().addRuntimeReservedBits();
    asImpl().addClassSize();
    asImpl().addClassAddressPoint();
    asImpl().addNominalTypeDescriptor();
    asImpl().addIVarDestroyer();

    // Class members.
    addClassMembers(Target, Target);
  }

  /// Notes the beginning of the field offset vector for a particular ancestor
  /// of a generic-layout class.
  void noteStartOfFieldOffsets(ClassDecl *whichClass) {}

  /// Notes the end of the field offset vector for a particular ancestor
  /// of a generic-layout class.
  void noteEndOfFieldOffsets(ClassDecl *whichClass) {}

  /// Notes the existence of a formally virtual method that has been elided from the
  /// reified vtable because it has no overrides.
  void noteNonoverriddenMethod(SILDeclRef method) {}
        
private:
  /// Add fields associated with the given class and its bases.
  void addClassMembers(ClassDecl *theClass,
                       ClassDecl *rootClass) {
    // Visit the superclass first.
    if (auto *superclassDecl = theClass->getSuperclassDecl()) {

      if (superclassDecl->hasClangNode()) {
        // Nothing to do; Objective-C classes do not add new members to
        // NeXTCode class metadata.

      // Super class metadata is resilient if
      // the superclass is resilient when viewed from the current module.
      } else if (IGM.hasResilientMetadata(superclassDecl,
                                          ResilienceExpansion::Maximal,
                                          rootClass)) {
        // Runtime metadata instantiation will initialize our field offset
        // vector and vtable entries.
        //
        // Metadata access needs to access our fields relative to a
        // global variable.
        asImpl().noteResilientSuperclass();
      } else {
        // NB: We don't apply superclass substitutions to members because we want
        // consistent metadata layout between generic superclasses and concrete
        // subclasses.
        addClassMembers(superclassDecl,
                        rootClass);
      }
    }

    // Note that we have to emit a global variable storing the metadata
    // start offset, or access remaining fields relative to one.
    asImpl().noteStartOfImmediateMembers(theClass);

    // Add space for the generic parameters, if applicable.
    // This must always be the first item in the immediate members.
    asImpl().addGenericFields(theClass, theClass);

    // If the class has resilient storage, we cannot make any assumptions about
    // its storage layout, so skip the rest of this method.
    if (IGM.isResilient(theClass, ResilienceExpansion::Maximal,
                        rootClass))
      return;

    // A class only really *needs* a field-offset vector in the
    // metadata if:
    //   - it's in a generic context and
    //   - there might exist a context which
    //     - can access the class's field storage directly and
    //     - sees the class as having a possibly dependent layout.
    //
    // A context which knows that the class does not have a dependent
    // layout should be able to just use a direct field offset
    // (possibly a constant one).
    //
    // But we currently always give classes field-offset vectors,
    // whether they need them or not.
    asImpl().noteStartOfFieldOffsets(theClass);
    forEachField(IGM, theClass, [&](Field field) {
      asImpl().addFieldEntries(field);
    });
    asImpl().noteEndOfFieldOffsets(theClass);

    // If the class has resilient metadata, we cannot make any assumptions
    // about its metadata layout, so skip the rest of this method.
    if (IGM.hasResilientMetadata(theClass, ResilienceExpansion::Maximal,
                                 rootClass))
      return;

    // Add vtable entries.
    asImpl().addVTableEntries(theClass);
  }

  /// Add fields associated with the given class and its bases.
  void addEmbeddedClassMembers(ClassDecl *theClass) {
    // Visit the superclass first.
    if (auto *superclassDecl = theClass->getSuperclassDecl()) {
      addEmbeddedClassMembers(superclassDecl);
    }

    // Note that we have to emit a global variable storing the metadata
    // start offset, or access remaining fields relative to one.
    asImpl().noteStartOfImmediateMembers(theClass);

    // Add vtable entries.
    asImpl().addVTableEntries(theClass);
  }

  friend SILVTableVisitor<Impl>;
  void addMethod(SILDeclRef declRef) {
    // Does this method require a reified runtime vtable entry?
    if (!VTable || methodRequiresReifiedVTableEntry(IGM, VTable, declRef)) {
      asImpl().addReifiedVTableEntry(declRef);
    } else {
      asImpl().noteNonoverriddenMethod(declRef);
    }
  }
  
        
  void addFieldEntries(Field field) {
    switch (field.getKind()) {
    case Field::Var:
      asImpl().addFieldOffset(field.getVarDecl());
      return;
    case Field::MissingMember:
      asImpl().addFieldOffsetPlaceholders(field.getMissingMemberDecl());
      return;
    case Field::DefaultActorStorage:
      asImpl().addDefaultActorStorageFieldOffset();
      return;
    case Field::NonDefaultDistributedActorStorage:
      asImpl().addNonDefaultDistributedActorStorageFieldOffset();
      return;
    }
  }
};

/// An "implementation" of ClassMetadataVisitor that just scans through
/// the metadata layout, maintaining the offset of the next field.
template <class Impl>
class ClassMetadataScanner : public ClassMetadataVisitor<Impl> {
  using super = ClassMetadataVisitor<Impl>;

protected:
  Size NextOffset = Size(0);

  ClassMetadataScanner(IRGenModule &IGM, ClassDecl *target)
    : super(IGM, target) {}

public:
  void addMetadataFlags() { addPointer(); }
  void addNominalTypeDescriptor() { addPointer(); }
  void addIVarDestroyer() { addPointer(); }
  void addValueWitnessTable() { addPointer(); }
  void addLayoutStringPointer() { addPointer(); }
  void addDestructorFunction() { addPointer(); }
  void addSuperclass() { addPointer(); }
  void addClassFlags() { addInt32(); }
  void addInstanceAddressPoint() { addInt32(); }
  void addInstanceSize() { addInt32(); }
  void addInstanceAlignMask() { addInt16(); }
  void addRuntimeReservedBits() { addInt16(); }
  void addClassSize() { addInt32(); }
  void addClassAddressPoint() { addInt32(); }
  void addClassCacheData() { addPointer(); addPointer(); }
  void addClassDataPointer() { addPointer(); }
  void addReifiedVTableEntry(SILDeclRef declRef) {
    addPointer();
  }
  void addMethodOverride(SILDeclRef baseRef, SILDeclRef declRef) {}
  void addDefaultActorStorageFieldOffset() { addPointer(); }
  void addNonDefaultDistributedActorStorageFieldOffset() { addPointer(); }
  void addFieldOffset(VarDecl *var) { addPointer(); }
  void addFieldOffsetPlaceholders(MissingMemberDecl *mmd) {
    for (unsigned i = 0, e = mmd->getNumberOfFieldOffsetVectorEntries();
         i < e; ++i) {
      addPointer();
    }
  }
  void addGenericRequirement(GenericRequirement requirement, ClassDecl *forClass) {
    addPointer();
  }
  void addPlaceholder(MissingMemberDecl *MMD) {
    for (auto i : range(MMD->getNumberOfVTableEntries())) {
      (void)i;
      addPointer();
    }
  }

private:
  // Our layout here assumes that there will never be unclaimed space
  // in the metadata.
  void addPointer() {
    NextOffset += super::IGM.getPointerSize();
  }
  void addInt32() {
    NextOffset += Size(4);
  }
  void addInt16() {
    NextOffset += Size(2);
  }
};

} // end namespace irgen
} // end namespace nextcode

#endif