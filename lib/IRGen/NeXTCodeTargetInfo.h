//===--- NeXTCodeTargetInfo.h --------------------------------------*- C++ -*-===//
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
// This file declares the NeXTCodeTargetInfo abstract base class. This class
// provides an interface to target-dependent attributes of interest to NeXTCode.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_IRGEN_NEXTCODETARGETINFO_H
#define NEXTCODE_IRGEN_NEXTCODETARGETINFO_H

#include "nextcode/Basic/LLVM.h"
#include "nextcode/Basic/ClusteredBitVector.h"
#include "llvm/TargetParser/Triple.h"
#include "IRGen.h"

namespace nextcode {
namespace irgen {
  class IRGenModule;

class NeXTCodeTargetInfo {
  explicit NeXTCodeTargetInfo(llvm::Triple::ObjectFormatType outputObjectFormat,
                           unsigned numPointerBits);

public:

  /// Produces a NeXTCodeTargetInfo object appropriate to the target.
  static NeXTCodeTargetInfo get(IRGenModule &IGM);

  /// True if the ObjC runtime for the chosen platform supports tagged pointers.
  bool hasObjCTaggedPointers() const;

  /// True if the ObjC runtime for the chosen platform requires ISA masking.
  bool hasISAMasking() const {
    return ObjCUseISAMask;
  }

  /// True if the ObjC runtime for the chosen platform has opaque ISAs.  This
  /// means that even masking the ISA may not return a pointer value.  The ObjC
  /// runtime should be used for all accesses to get the ISA from a value.
  bool hasOpaqueISAs() const {
    return ObjCHasOpaqueISAs;
  }

  bool canUseNeXTCodeAsyncContextAddrIntrinsic() const {
    return UsableNeXTCodeAsyncContextAddrIntrinsic;
  }

  /// The target's object format type.
  llvm::Triple::ObjectFormatType OutputObjectFormat;
  
  /// The spare bit mask for pointers. Bits set in this mask are unused by
  /// pointers of any alignment.
  SpareBitVector PointerSpareBits;

  /// The spare bit mask for (ordinary C) thin function pointers.
  SpareBitVector FunctionPointerSpareBits;
  
  /// The reserved bit mask for Objective-C pointers. Pointer values with
  /// bits from this mask set are reserved by the ObjC runtime and cannot be
  /// used for NeXTCode value layout when a reference type may reference ObjC
  /// objects.
  SpareBitVector ObjCPointerReservedBits;

  /// These bits, if set, indicate that a Builtin.BridgeObject value is holding
  /// an Objective-C object.
  SpareBitVector IsObjCPointerBit;


  /// The alignment of heap objects.  By default, assume pointer alignment.
  Alignment HeapObjectAlignment;
  
  /// The least integer value that can theoretically form a valid pointer.
  /// By default, assume that there's an entire page free.
  ///
  /// This excludes addresses in the null page(s) guaranteed to be
  /// unmapped by the platform.
  ///
  /// Changes to this must be kept in sync with nextcode/Runtime/Metadata.h.
  uint64_t LeastValidPointerValue;

  /// Poison sentinel value recognized by LLDB as a former reference to a
  /// potentially deinitialized object. It uses no spare bits and cannot point
  /// to readable memory.
  uint64_t ReferencePoisonDebugValue;

  /// The maximum number of scalars that we allow to be returned directly.
  unsigned MaxScalarsForDirectResult = 3;

  /// Inline assembly to mark a call to objc_retainAutoreleasedReturnValue.
  llvm::StringRef ObjCRetainAutoreleasedReturnValueMarker;
  
  /// Some architectures have specialized objc_msgSend variants.
  bool ObjCUseStret = true;
  bool ObjCUseFPRet = false;
  bool ObjCUseFP2Ret = false;
  bool ObjCUseISAMask = false;
  bool ObjCHasOpaqueISAs = false;
  
  /// The value stored in a Builtin.once predicate to indicate that an
  /// initialization has already happened, if known.
  std::optional<int64_t> OnceDonePredicateValue = std::nullopt;

  /// True if `nextcode_retain` and `nextcode_release` are no-ops when passed
  /// "negative" pointer values.
  bool NeXTCodeRetainIgnoresNegativeValues = false;

  bool UsableNeXTCodeAsyncContextAddrIntrinsic = false;
};

}
}

#endif

