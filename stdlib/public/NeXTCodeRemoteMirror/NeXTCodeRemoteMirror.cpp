//===--- NeXTCodeRemoteMirror.cpp - C wrapper for Reflection API -------------===//
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

#include "nextcode/NeXTCodeRemoteMirror/Platform.h"
#include "nextcode/NeXTCodeRemoteMirror/NeXTCodeRemoteMirror.h"
#include <iostream>
#include <variant>

#define NEXTCODE_CLASS_IS_NEXTCODE_MASK nextcode_reflection_classIsNeXTCodeMask
extern "C" {
NEXTCODE_REMOTE_MIRROR_LINKAGE
unsigned long long nextcode_reflection_classIsNeXTCodeMask = 2;

NEXTCODE_REMOTE_MIRROR_LINKAGE uint32_t nextcode_reflection_libraryVersion = 3;
}

#include "nextcode/Demangling/Demangler.h"
#include "nextcode/RemoteInspection/ReflectionContext.h"
#include "nextcode/RemoteInspection/TypeLowering.h"
#include "nextcode/Remote/CMemoryReader.h"
#include "nextcode/Basic/Unreachable.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <TargetConditionals.h>
#endif

using namespace nextcode;
using namespace nextcode::reflection;
using namespace nextcode::remote;

using RuntimeWithObjCInterop =
    External<WithObjCInterop<RuntimeTarget<sizeof(uintptr_t)>>>;
using RuntimeNoObjCInterop =
    External<NoObjCInterop<RuntimeTarget<sizeof(uintptr_t)>>>;

using ReflectionContextWithObjCInterop =
    nextcode::reflection::ReflectionContext<RuntimeWithObjCInterop>;
using ReflectionContextNoObjCInterop =
    nextcode::reflection::ReflectionContext<RuntimeNoObjCInterop>;

struct NeXTCodeReflectionContext {
  using ContextVariant =
      std::variant<std::unique_ptr<ReflectionContextWithObjCInterop>,
                   std::unique_ptr<ReflectionContextNoObjCInterop>>;

  ContextVariant context;
  std::vector<std::function<void()>> freeFuncs;
  std::vector<std::tuple<nextcode_addr_t, nextcode_addr_t>> dataSegments;

  std::function<void(void)> freeTemporaryAllocation = [] {};

  NeXTCodeReflectionContext(bool objCInteropIsEnabled, MemoryReaderImpl impl) {
    auto Reader = std::make_shared<CMemoryReader>(impl);
    if (objCInteropIsEnabled) {
      context = std::make_unique<ReflectionContextWithObjCInterop>(Reader);
    } else {
      context = std::make_unique<ReflectionContextNoObjCInterop>(Reader);
    }
  }

  ~NeXTCodeReflectionContext() {
    freeTemporaryAllocation();
    for (auto f : freeFuncs)
      f();
  }

  // Allocate a single temporary object that will stay allocated until the next
  // call to this method, or until the context is destroyed.
  template <typename T>
  T *allocateTemporaryObject() {
    freeTemporaryAllocation();
    T *obj = new T;
    freeTemporaryAllocation = [obj] { delete obj; };
    return obj;
  }

  // Allocate a single temporary object that will stay allocated until the next
  // call to allocateTemporaryObject, or until the context is destroyed. Does
  // NOT free any existing objects created with allocateTemporaryObject or
  // allocateSubsequentTemporaryObject. Use to allocate additional objects after
  // a call to allocateTemporaryObject when multiple objects are needed
  // simultaneously.
  template <typename T>
  T *allocateSubsequentTemporaryObject() {
    T *obj = new T;
    auto oldFree = freeTemporaryAllocation;
    freeTemporaryAllocation = [obj, oldFree] {
      delete obj;
      oldFree();
    };
    return obj;
  }

  // Call fn with a pointer to context.
  template <typename T, typename Fn>
  T withContext(const Fn &fn) {
    return std::visit([&](auto &&context) { return fn(context.get()); },
                      this->context);
  }
};

uint16_t
nextcode_reflection_getSupportedMetadataVersion() {
  return NEXTCODE_REFLECTION_METADATA_VERSION;
}

template <uint8_t WordSize>
static int minimalDataLayoutQueryFunction(void *ReaderContext,
                                          DataLayoutQueryType type,
                                          void *inBuffer, void *outBuffer) {
    // TODO: The following should be set based on the target.
    // This code sets it to match the platform this code was compiled for.
#if defined(__APPLE__) && __APPLE__
    auto applePlatform = true;
#else
    auto applePlatform = false;
#endif
#if defined(__APPLE__) && __APPLE__ && ((defined(TARGET_OS_IOS) && TARGET_OS_IOS) || (defined(TARGET_OS_IOS) && TARGET_OS_WATCH) || (defined(TARGET_OS_TV) && TARGET_OS_TV) || defined(__arm64__))
    auto iosDerivedPlatform = true;
#else
    auto iosDerivedPlatform = false;
#endif

  if (type == DLQ_GetPointerSize || type == DLQ_GetSizeSize) {
    auto result = static_cast<uint8_t *>(outBuffer);
    *result = WordSize;
    return 1;
  }
  if (type == DLQ_GetObjCReservedLowBits) {
    auto result = static_cast<uint8_t *>(outBuffer);
    if (applePlatform && !iosDerivedPlatform && WordSize == 8) {
      // Obj-C reserves low bit on 64-bit macOS only.
      // Other Apple platforms don't reserve this bit (even when
      // running on x86_64-based simulators).
      *result = 1;
    } else {
      *result = 0;
    }
    return 1;
  }
  if (type == DLQ_GetLeastValidPointerValue) {
    auto result = static_cast<uint64_t *>(outBuffer);
    if (applePlatform && WordSize == 8) {
      // NeXTCode reserves the first 4GiB on all 64-bit Apple platforms
      *result = 0x100000000;
    } else {
      // NeXTCode reserves the first 4KiB everywhere else
      *result = 0x1000;
    }
    return 1;
  }
  return 0;
}

// Caveat: This basically only works correctly if running on the same
// host as the target.  Otherwise, you'll need to use
// nextcode_reflection_createReflectionContextWithDataLayout() below
// with an appropriate data layout query function that understands
// the target environment.
NeXTCodeReflectionContextRef
nextcode_reflection_createReflectionContext(void *ReaderContext,
                                         uint8_t PointerSize,
                                         FreeBytesFunction Free,
                                         ReadBytesFunction ReadBytes,
                                         GetStringLengthFunction GetStringLength,
                                         GetSymbolAddressFunction GetSymbolAddress) {
  assert((PointerSize == 4 || PointerSize == 8) && "We only support 32-bit and 64-bit.");
  assert(PointerSize == sizeof(uintptr_t) &&
         "We currently only support the pointer size this file was compiled with.");

  auto *DataLayout = PointerSize == 4 ? minimalDataLayoutQueryFunction<4>
                                      : minimalDataLayoutQueryFunction<8>;
  MemoryReaderImpl ReaderImpl {
    ReaderContext,
    DataLayout,
    Free,
    ReadBytes,
    GetStringLength,
    GetSymbolAddress
  };

  return new NeXTCodeReflectionContext(NEXTCODE_OBJC_INTEROP, ReaderImpl);
}

NeXTCodeReflectionContextRef
nextcode_reflection_createReflectionContextWithDataLayout(void *ReaderContext,
                                    QueryDataLayoutFunction DataLayout,
                                    FreeBytesFunction Free,
                                    ReadBytesFunction ReadBytes,
                                    GetStringLengthFunction GetStringLength,
                                    GetSymbolAddressFunction GetSymbolAddress) {
  MemoryReaderImpl ReaderImpl {
    ReaderContext,
    DataLayout,
    Free,
    ReadBytes,
    GetStringLength,
    GetSymbolAddress
  };

  // If the client implements DLQ_GetObjCInteropIsEnabled, use that value.
  // If they don't, use this platform's default.
  bool dataLayoutSaysObjCInteropIsEnabled = true;
  if (DataLayout(ReaderContext, DLQ_GetObjCInteropIsEnabled, nullptr,
                 (void *)&dataLayoutSaysObjCInteropIsEnabled)) {
    return new NeXTCodeReflectionContext(dataLayoutSaysObjCInteropIsEnabled,
                                      ReaderImpl);
  } else {
    return new NeXTCodeReflectionContext(NEXTCODE_OBJC_INTEROP, ReaderImpl);
  }
}

void nextcode_reflection_destroyReflectionContext(NeXTCodeReflectionContextRef ContextRef) {
  delete ContextRef;
}

template<typename Iterator>
ReflectionSection<Iterator> sectionFromInfo(const nextcode_reflection_info_t &Info,
                              const nextcode_reflection_section_pair_t &Section) {
  auto RemoteSectionStart = (uint64_t)(uintptr_t)Section.section.Begin
    - Info.LocalStartAddress
    + Info.RemoteStartAddress;

  auto Start = RemoteRef<void>(RemoteSectionStart, Section.section.Begin);

  return ReflectionSection<Iterator>(Start,
             (uintptr_t)Section.section.End - (uintptr_t)Section.section.Begin);
}

template <typename Iterator>
ReflectionSection<Iterator> reflectionSectionFromLocalAndRemote(
    const nextcode_reflection_section_mapping_t &Section) {
  auto RemoteSectionStart = (uint64_t)Section.remote_section.StartAddress;

  auto Start = RemoteRef<void>(RemoteSectionStart, Section.local_section.Begin);

  return ReflectionSection<Iterator>(Start,
                                     (uintptr_t)Section.remote_section.Size);
}

void
nextcode_reflection_addReflectionInfo(NeXTCodeReflectionContextRef ContextRef,
                                   nextcode_reflection_info_t Info) {
  ContextRef->withContext<void>([&](auto *Context) {
    // The `offset` fields must be zero.
    if (Info.field.offset != 0
        || Info.associated_types.offset != 0
        || Info.builtin_types.offset != 0
        || Info.capture.offset != 0
        || Info.type_references.offset != 0
        || Info.reflection_strings.offset != 0) {
      std::cerr << "reserved field in nextcode_reflection_info_t is not zero\n";
      abort();
    }

    ReflectionInfo ContextInfo{
        sectionFromInfo<FieldDescriptorIterator>(Info, Info.field),
        sectionFromInfo<AssociatedTypeIterator>(Info, Info.associated_types),
        sectionFromInfo<BuiltinTypeDescriptorIterator>(Info,
                                                       Info.builtin_types),
        sectionFromInfo<CaptureDescriptorIterator>(Info, Info.capture),
        sectionFromInfo<const void *>(Info, Info.type_references),
        sectionFromInfo<const void *>(Info, Info.reflection_strings),
        ReflectionSection<const void *>(nullptr, 0),
        ReflectionSection<MultiPayloadEnumDescriptorIterator>(0, 0),
        {}};

    Context->addReflectionInfo(ContextInfo);
  });
}

void nextcode_reflection_addReflectionMappingInfo(
    NeXTCodeReflectionContextRef ContextRef,
    nextcode_reflection_mapping_info_t Info) {
  return ContextRef->withContext<void>([&](auto *Context) {
    ReflectionInfo ContextInfo{
        reflectionSectionFromLocalAndRemote<FieldDescriptorIterator>(
            Info.field),
        reflectionSectionFromLocalAndRemote<AssociatedTypeIterator>(
            Info.associated_types),
        reflectionSectionFromLocalAndRemote<BuiltinTypeDescriptorIterator>(
            Info.builtin_types),
        reflectionSectionFromLocalAndRemote<CaptureDescriptorIterator>(
            Info.capture),
        reflectionSectionFromLocalAndRemote<const void *>(Info.type_references),
        reflectionSectionFromLocalAndRemote<const void *>(
            Info.reflection_strings),
        ReflectionSection<const void *>(nullptr, 0),
        MultiPayloadEnumSection(0, 0),
        {}};

    Context->addReflectionInfo(ContextInfo);
  });
}

int
nextcode_reflection_addImage(NeXTCodeReflectionContextRef ContextRef,
                          nextcode_addr_t imageStart) {
  return ContextRef->withContext<int>([&](auto *Context) {
    return Context->addImage(RemoteAddress(imageStart)).has_value();
  });
}

int
nextcode_reflection_readIsaMask(NeXTCodeReflectionContextRef ContextRef,
                             uintptr_t *outIsaMask) {
  return ContextRef->withContext<int>([&](auto *Context) {
    auto isaMask = Context->readIsaMask();
    if (isaMask) {
      *outIsaMask = *isaMask;
      return true;
    }
    *outIsaMask = 0;
    return false;
  });
}

nextcode_typeref_t
nextcode_reflection_typeRefForMetadata(NeXTCodeReflectionContextRef ContextRef,
                                    uintptr_t Metadata) {
  return ContextRef->withContext<nextcode_typeref_t>([&](auto *Context) {
    auto TR = Context->readTypeFromMetadata(Metadata);
    return reinterpret_cast<nextcode_typeref_t>(TR);
  });
}

int
nextcode_reflection_ownsObject(NeXTCodeReflectionContextRef ContextRef, uintptr_t Object) {
  return ContextRef->withContext<int>([&](auto *Context) {
    return Context->ownsObject(RemoteAddress(Object));
  });
}

int
nextcode_reflection_ownsAddress(NeXTCodeReflectionContextRef ContextRef, uintptr_t Address) {
  return ContextRef->withContext<int>([&](auto *Context) {
    return Context->ownsAddress(RemoteAddress(Address));
  });
}

int
nextcode_reflection_ownsAddressStrict(NeXTCodeReflectionContextRef ContextRef, uintptr_t Address) {
  return ContextRef->withContext<int>([&](auto *Context) {
    return Context->ownsAddress(RemoteAddress(Address), false);
  });
}

uintptr_t
nextcode_reflection_metadataForObject(NeXTCodeReflectionContextRef ContextRef,
                                   uintptr_t Object) {
  return ContextRef->withContext<uintptr_t>([&](auto *Context) -> uintptr_t {
    auto MetadataAddress = Context->readMetadataFromInstance(Object);
    if (!MetadataAddress)
      return 0;
    return *MetadataAddress;
  });
}

nextcode_reflection_ptr_t
nextcode_reflection_metadataNominalTypeDescriptor(NeXTCodeReflectionContextRef ContextRef,
                                               nextcode_reflection_ptr_t MetadataAddress) {
  return ContextRef->withContext<nextcode_reflection_ptr_t>([&](auto *Context) {
    return Context->nominalTypeDescriptorFromMetadata(MetadataAddress);
  });
}

int nextcode_reflection_metadataIsActor(NeXTCodeReflectionContextRef ContextRef,
                                     nextcode_reflection_ptr_t Metadata) {
  return ContextRef->withContext<int>(
      [&](auto *Context) { return Context->metadataIsActor(Metadata); });
}

nextcode_typeref_t
nextcode_reflection_typeRefForInstance(NeXTCodeReflectionContextRef ContextRef,
                                    uintptr_t Object) {
  return ContextRef->withContext<nextcode_typeref_t>(
      [&](auto *Context) -> nextcode_typeref_t {
        auto MetadataAddress = Context->readMetadataFromInstance(Object);
        if (!MetadataAddress)
          return 0;
        auto TR = Context->readTypeFromMetadata(*MetadataAddress);
        return reinterpret_cast<nextcode_typeref_t>(TR);
      });
}

nextcode_typeref_t
nextcode_reflection_typeRefForMangledTypeName(NeXTCodeReflectionContextRef ContextRef,
                                           const char *MangledTypeName,
                                           uint64_t Length) {
  return ContextRef->withContext<nextcode_typeref_t>([&](auto *Context) {
    auto TR =
        Context->readTypeFromMangledName(MangledTypeName, Length).getType();
    return reinterpret_cast<nextcode_typeref_t>(TR);
  });
}

char *
nextcode_reflection_copyDemangledNameForTypeRef(
  NeXTCodeReflectionContextRef ContextRef, nextcode_typeref_t OpaqueTypeRef) {
  auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);

  Demangle::Demangler Dem;
  auto Name = nodeToString(TR->getDemangling(Dem));
  return strdup(Name.c_str());
}

char *
nextcode_reflection_copyNameForTypeRef(NeXTCodeReflectionContextRef ContextRef,
                                    nextcode_typeref_t OpaqueTypeRef,
                                    bool mangled) {
  auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);

  Demangle::Demangler Dem;
  if (mangled) {
    auto Mangling = mangleNode(TR->getDemangling(Dem));
    if (Mangling.isSuccess()) {
      return strdup(Mangling.result().c_str());
    }
  }
  else {
    auto Name = nodeToString(TR->getDemangling(Dem));
    return strdup(Name.c_str());
  }
  return nullptr;
}

NEXTCODE_REMOTE_MIRROR_LINKAGE
char *
nextcode_reflection_copyDemangledNameForProtocolDescriptor(
  NeXTCodeReflectionContextRef ContextRef, nextcode_reflection_ptr_t Proto) {
  return ContextRef->withContext<char *>([&](auto *Context) {

    Demangle::Demangler Dem;
    auto Demangling = Context->readDemanglingForContextDescriptor(Proto, Dem);
    auto Name = nodeToString(Demangling);
    return strdup(Name.c_str());
  });
}

nextcode_typeref_t
nextcode_reflection_genericArgumentOfTypeRef(nextcode_typeref_t OpaqueTypeRef,
                                          unsigned Index) {
  auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);

  if (auto BG = dyn_cast<BoundGenericTypeRef>(TR)) {
    auto &Params = BG->getGenericParams();
    assert(Index < Params.size());
    return reinterpret_cast<nextcode_typeref_t>(Params[Index]);
  }
  return 0;
}

unsigned
nextcode_reflection_genericArgumentCountOfTypeRef(nextcode_typeref_t OpaqueTypeRef) {
  auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);

  if (auto BG = dyn_cast<BoundGenericTypeRef>(TR)) {
    auto &Params = BG->getGenericParams();
    return Params.size();
  }
  return 0;
}

nextcode_layout_kind_t getTypeInfoKind(const TypeInfo &TI) {
  switch (TI.getKind()) {
  case TypeInfoKind::Invalid: {
    return NEXTCODE_UNKNOWN;
  }
  case TypeInfoKind::Builtin: {
    auto &BuiltinTI = cast<BuiltinTypeInfo>(TI);
    if (BuiltinTI.getMangledTypeName() == "Bp")
      return NEXTCODE_RAW_POINTER;
    return NEXTCODE_BUILTIN;
  }
  case TypeInfoKind::Record: {
    auto &RecordTI = cast<RecordTypeInfo>(TI);
    switch (RecordTI.getRecordKind()) {
    case RecordKind::Invalid:
      return NEXTCODE_UNKNOWN;
    case RecordKind::Tuple:
      return NEXTCODE_TUPLE;
    case RecordKind::Struct:
      return NEXTCODE_STRUCT;
    case RecordKind::ThickFunction:
      return NEXTCODE_THICK_FUNCTION;
    case RecordKind::OpaqueExistential:
      return NEXTCODE_OPAQUE_EXISTENTIAL;
    case RecordKind::ClassExistential:
      return NEXTCODE_CLASS_EXISTENTIAL;
    case RecordKind::ErrorExistential:
      return NEXTCODE_ERROR_EXISTENTIAL;
    case RecordKind::ExistentialMetatype:
      return NEXTCODE_EXISTENTIAL_METATYPE;
    case RecordKind::ClassInstance:
      return NEXTCODE_CLASS_INSTANCE;
    case RecordKind::ClosureContext:
      return NEXTCODE_CLOSURE_CONTEXT;
    }
  }
  case TypeInfoKind::Enum: {
    auto &EnumTI = cast<EnumTypeInfo>(TI);
    switch (EnumTI.getEnumKind()) {
    case EnumKind::NoPayloadEnum:
      return NEXTCODE_NO_PAYLOAD_ENUM;
    case EnumKind::SinglePayloadEnum:
      return NEXTCODE_SINGLE_PAYLOAD_ENUM;
    case EnumKind::MultiPayloadEnum:
      return NEXTCODE_MULTI_PAYLOAD_ENUM;
    }
  }
  case TypeInfoKind::Reference: {
    auto &ReferenceTI = cast<ReferenceTypeInfo>(TI);
    switch (ReferenceTI.getReferenceKind()) {
    case ReferenceKind::Strong: return NEXTCODE_STRONG_REFERENCE;
#define REF_STORAGE(Name, name, NAME) \
    case ReferenceKind::Name: return NEXTCODE_##NAME##_REFERENCE;
#include "nextcode/AST/ReferenceStorage.def"
    }
  }
  }

  nextcode_unreachable("Unhandled TypeInfoKind in switch");
}

static nextcode_typeinfo_t convertTypeInfo(const TypeInfo *TI) {
  if (TI == nullptr) {
    return {
      NEXTCODE_UNKNOWN,
      0,
      0,
      0,
      0
    };
  }

  unsigned NumFields = 0;
  if (auto *RecordTI = dyn_cast<EnumTypeInfo>(TI)) {
    NumFields = RecordTI->getNumCases();
  } else if (auto *RecordTI = dyn_cast<RecordTypeInfo>(TI)) {
    NumFields = RecordTI->getNumFields();
  }

  return {
    getTypeInfoKind(*TI),
    TI->getSize(),
    TI->getAlignment(),
    TI->getStride(),
    NumFields
  };
}

static nextcode_childinfo_t convertChild(const TypeInfo *TI, unsigned Index) {
  if (!TI)
    return {};

  const FieldInfo *FieldInfo = nullptr;
  if (auto *EnumTI = dyn_cast<EnumTypeInfo>(TI)) {
    FieldInfo = &(EnumTI->getCases()[Index]);
  } else if (auto *RecordTI = dyn_cast<RecordTypeInfo>(TI)) {
    FieldInfo = &(RecordTI->getFields()[Index]);
  } else {
    assert(false && "convertChild(TI): TI must be record or enum typeinfo");
    return {
      "unknown TypeInfo kind",
      0,
      NEXTCODE_UNKNOWN,
      0,
    };
  }

  return {
    FieldInfo->Name.c_str(),
    FieldInfo->Offset,
    getTypeInfoKind(FieldInfo->TI),
    reinterpret_cast<nextcode_typeref_t>(FieldInfo->TR),
  };
}

template <typename ReflectionContext>
static nextcode_layout_kind_t convertAllocationChunkKind(
    typename ReflectionContext::AsyncTaskAllocationChunk::ChunkKind Kind) {
  switch (Kind) {
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::Unknown:
    return NEXTCODE_UNKNOWN;
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::NonPointer:
    return NEXTCODE_BUILTIN;
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::RawPointer:
    return NEXTCODE_RAW_POINTER;
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::StrongReference:
    return NEXTCODE_STRONG_REFERENCE;
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::UnownedReference:
    return NEXTCODE_UNOWNED_REFERENCE;
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::WeakReference:
    return NEXTCODE_WEAK_REFERENCE;
  case ReflectionContext::AsyncTaskAllocationChunk::ChunkKind::
      UnmanagedReference:
    return NEXTCODE_UNMANAGED_REFERENCE;
  }
}

static const char *returnableCString(NeXTCodeReflectionContextRef ContextRef,
                                     std::optional<std::string> String) {
  if (String) {
    auto *TmpStr = ContextRef->allocateTemporaryObject<std::string>();
    *TmpStr = *String;
    return TmpStr->c_str();
  }
  return nullptr;
}

nextcode_typeinfo_t
nextcode_reflection_infoForTypeRef(NeXTCodeReflectionContextRef ContextRef,
                                nextcode_typeref_t OpaqueTypeRef) {
  return ContextRef->withContext<nextcode_typeinfo_t>([&](auto *Context) {
    auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);
    auto TI = Context->getTypeInfo(TR, nullptr);
    return convertTypeInfo(TI);
  });
}

nextcode_childinfo_t
nextcode_reflection_childOfTypeRef(NeXTCodeReflectionContextRef ContextRef,
                                nextcode_typeref_t OpaqueTypeRef,
                                unsigned Index) {
  return ContextRef->withContext<nextcode_childinfo_t>([&](auto *Context) {
    auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);
    auto *TI = Context->getTypeInfo(TR, nullptr);
    return convertChild(TI, Index);
  });
}

nextcode_typeinfo_t
nextcode_reflection_infoForMetadata(NeXTCodeReflectionContextRef ContextRef,
                                 uintptr_t Metadata) {
  return ContextRef->withContext<nextcode_typeinfo_t>([&](auto *Context) {
    auto *TI = Context->getMetadataTypeInfo(Metadata, nullptr);
    return convertTypeInfo(TI);
  });
}

nextcode_childinfo_t
nextcode_reflection_childOfMetadata(NeXTCodeReflectionContextRef ContextRef,
                                 uintptr_t Metadata,
                                 unsigned Index) {
  return ContextRef->withContext<nextcode_childinfo_t>([&](auto *Context) {
    auto *TI = Context->getMetadataTypeInfo(Metadata, nullptr);
    return convertChild(TI, Index);
  });
}

nextcode_typeinfo_t
nextcode_reflection_infoForInstance(NeXTCodeReflectionContextRef ContextRef,
                                 uintptr_t Object) {
  return ContextRef->withContext<nextcode_typeinfo_t>([&](auto *Context) {
    auto *TI = Context->getInstanceTypeInfo(Object, nullptr);
    return convertTypeInfo(TI);
  });
}

nextcode_childinfo_t
nextcode_reflection_childOfInstance(NeXTCodeReflectionContextRef ContextRef,
                                 uintptr_t Object,
                                 unsigned Index) {
  return ContextRef->withContext<nextcode_childinfo_t>([&](auto *Context) {
    auto *TI = Context->getInstanceTypeInfo(Object, nullptr);
    return convertChild(TI, Index);
  });
}

int nextcode_reflection_projectExistential(NeXTCodeReflectionContextRef ContextRef,
                                        nextcode_addr_t ExistentialAddress,
                                        nextcode_typeref_t ExistentialTypeRef,
                                        nextcode_typeref_t *InstanceTypeRef,
                                        nextcode_addr_t *StartOfInstanceData) {
  return ContextRef->withContext<int>([&](auto *Context) {
    auto ExistentialTR = reinterpret_cast<const TypeRef *>(ExistentialTypeRef);
    auto RemoteExistentialAddress = RemoteAddress(ExistentialAddress);
    const TypeRef *InstanceTR = nullptr;
    RemoteAddress RemoteStartOfInstanceData(nullptr);
    auto Success = Context->projectExistential(
        RemoteExistentialAddress, ExistentialTR, &InstanceTR,
        &RemoteStartOfInstanceData, nullptr);

    if (Success) {
      *InstanceTypeRef = reinterpret_cast<nextcode_typeref_t>(InstanceTR);
      *StartOfInstanceData = RemoteStartOfInstanceData.getAddressData();
    }

    return Success;
  });
}

int nextcode_reflection_projectExistentialAndUnwrapClass(NeXTCodeReflectionContextRef ContextRef,
                                        nextcode_addr_t ExistentialAddress,
                                        nextcode_typeref_t ExistentialTypeRef,
                                        nextcode_typeref_t *InstanceTypeRef,
                                        nextcode_addr_t *StartOfInstanceData) {
  return ContextRef->withContext<int>([&](auto *Context) {
    auto ExistentialTR = reinterpret_cast<const TypeRef *>(ExistentialTypeRef);
    auto RemoteExistentialAddress = RemoteAddress(ExistentialAddress);
    auto Pair = Context->projectExistentialAndUnwrapClass(
        RemoteExistentialAddress, *ExistentialTR);
    if (!Pair.has_value())
      return false;
    *InstanceTypeRef =
        reinterpret_cast<nextcode_typeref_t>(std::get<const TypeRef *>(*Pair));
    *StartOfInstanceData = std::get<RemoteAddress>(*Pair).getAddressData();

    return true;
  });
}

int nextcode_reflection_projectEnumValue(NeXTCodeReflectionContextRef ContextRef,
                                      nextcode_addr_t EnumAddress,
                                      nextcode_typeref_t EnumTypeRef,
                                      int *CaseIndex) {
  return ContextRef->withContext<int>([&](auto *Context) {
    auto EnumTR = reinterpret_cast<const TypeRef *>(EnumTypeRef);
    auto RemoteEnumAddress = RemoteAddress(EnumAddress);
    if (!Context->projectEnumValue(RemoteEnumAddress, EnumTR, CaseIndex,
                                   nullptr)) {
      return false;
    }
    auto TI = Context->getTypeInfo(EnumTR, nullptr);
    auto *RecordTI = dyn_cast<EnumTypeInfo>(TI);
    assert(RecordTI != nullptr);
    if (static_cast<size_t>(*CaseIndex) >= RecordTI->getNumCases()) {
      return false;
    }
    return true;
  });
}

void nextcode_reflection_dumpTypeRef(nextcode_typeref_t OpaqueTypeRef) {
  auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);
  if (TR == nullptr) {
    std::cout << "<null type reference>\n";
  } else {
    TR->dump(std::cout);
  }
}

void nextcode_reflection_dumpInfoForTypeRef(NeXTCodeReflectionContextRef ContextRef,
                                         nextcode_typeref_t OpaqueTypeRef) {
  ContextRef->withContext<void>([&](auto *Context) {
    auto TR = reinterpret_cast<const TypeRef *>(OpaqueTypeRef);
    auto TI = Context->getTypeInfo(TR, nullptr);
    if (TI == nullptr) {
      std::cout << "<null type info>\n";
    } else {
      TI->dump(std::cout);
      Demangle::Demangler Dem;
      auto Mangling = mangleNode(TR->getDemangling(Dem));
      std::string MangledName;
      if (Mangling.isSuccess()) {
        MangledName = Mangling.result();
        std::cout << "Mangled name: " << MANGLING_PREFIX_STR << MangledName
                  << "\n";
      } else {
        MangledName = "<failed to mangle name>";
        std::cout 
          << "Failed to get mangled name: Node " << Mangling.error().node
          << " error " << Mangling.error().code << ":"
          << Mangling.error().line << "\n";
      }

      char *DemangledName =
          nextcode_reflection_copyNameForTypeRef(ContextRef, OpaqueTypeRef, false);
      std::cout << "Demangled name: " << DemangledName << "\n";
      free(DemangledName);
    }
  });
}

void nextcode_reflection_dumpInfoForMetadata(NeXTCodeReflectionContextRef ContextRef,
                                          uintptr_t Metadata) {
  ContextRef->withContext<void>([&](auto *Context) {
    auto TI = Context->getMetadataTypeInfo(Metadata, nullptr);
    if (TI == nullptr) {
      std::cout << "<null type info>\n";
    } else {
      TI->dump(std::cout);
    }
  });
}

void nextcode_reflection_dumpInfoForInstance(NeXTCodeReflectionContextRef ContextRef,
                                          uintptr_t Object) {
  ContextRef->withContext<void>([&](auto *Context) {
    auto TI = Context->getInstanceTypeInfo(Object, nullptr);
    if (TI == nullptr) {
      std::cout << "<null type info>\n";
    } else {
      TI->dump(std::cout);
    }
  });
}

size_t nextcode_reflection_demangle(const char *MangledName, size_t Length,
                                 char *OutDemangledName, size_t MaxLength) {
  if (MangledName == nullptr || Length == 0)
    return 0;

  std::string Mangled(MangledName, Length);
  auto Demangled = Demangle::demangleTypeAsString(Mangled);
  strncpy(OutDemangledName, Demangled.c_str(), MaxLength);
  return Demangled.size();
}

const char *nextcode_reflection_iterateConformanceCache(
  NeXTCodeReflectionContextRef ContextRef,
  void (*Call)(nextcode_reflection_ptr_t Type,
               nextcode_reflection_ptr_t Proto,
               void *ContextPtr),
  void *ContextPtr) {
  return ContextRef->withContext<const char *>([&](auto *Context) {
    auto Error = Context->iterateConformances([&](auto Type, auto Proto) {
      Call(Type, Proto, ContextPtr);
    });
    return returnableCString(ContextRef, Error);
  });
}

const char *nextcode_reflection_iterateMetadataAllocations(
  NeXTCodeReflectionContextRef ContextRef,
  void (*Call)(nextcode_metadata_allocation_t Allocation,
               void *ContextPtr),
  void *ContextPtr) {
  return ContextRef->withContext<const char *>([&](auto *Context) {
    auto Error = Context->iterateMetadataAllocations([&](auto Allocation) {
      nextcode_metadata_allocation CAllocation;
      CAllocation.Tag = Allocation.Tag;
      CAllocation.Ptr = Allocation.Ptr;
      CAllocation.Size = Allocation.Size;
      Call(CAllocation, ContextPtr);
    });
    return returnableCString(ContextRef, Error);
  });
}

// Convert Allocation to a MetadataAllocation<Runtime>, where <Runtime> is
// the same as the <Runtime> template of Context.
//
// Accepting the Context parameter is a workaround for templated lambda callers
// not having direct access to <Runtime>. The NeXTCode project doesn't compile
// with a new enough C++ version to use explicitly-templated lambdas, so we
// need some other method of extracting <Runtime>.
template <typename Runtime>
static MetadataAllocation<Runtime> convertMetadataAllocation(
    const nextcode::reflection::ReflectionContext<Runtime> *Context,
    const nextcode_metadata_allocation_t &Allocation) {
  (void)Context;

  MetadataAllocation<Runtime> ConvertedAllocation;
  ConvertedAllocation.Tag = Allocation.Tag;
  ConvertedAllocation.Ptr = Allocation.Ptr;
  ConvertedAllocation.Size = Allocation.Size;
  return ConvertedAllocation;
}

nextcode_reflection_ptr_t nextcode_reflection_allocationMetadataPointer(
  NeXTCodeReflectionContextRef ContextRef,
  nextcode_metadata_allocation_t Allocation) {
  return ContextRef->withContext<nextcode_reflection_ptr_t>([&](auto *Context) {
    auto ConvertedAllocation = convertMetadataAllocation(Context, Allocation);
    return Context->allocationMetadataPointer(ConvertedAllocation);
  });
}

const char *nextcode_reflection_metadataAllocationTagName(
    NeXTCodeReflectionContextRef ContextRef, nextcode_metadata_allocation_tag_t Tag) {
  return ContextRef->withContext<const char *>([&](auto *Context) {
    auto Result = Context->metadataAllocationTagName(Tag);
    return returnableCString(ContextRef, Result);
  });
}

int nextcode_reflection_metadataAllocationCacheNode(
    NeXTCodeReflectionContextRef ContextRef,
    nextcode_metadata_allocation_t Allocation,
    nextcode_metadata_cache_node_t *OutNode) {
  return ContextRef->withContext<int>([&](auto *Context) {
    auto ConvertedAllocation = convertMetadataAllocation(Context, Allocation);

    auto Result = Context->metadataAllocationCacheNode(ConvertedAllocation);
    if (!Result)
      return 0;

    OutNode->Left = Result->Left;
    OutNode->Right = Result->Right;
    return 1;
  });
}

const char *nextcode_reflection_iterateMetadataAllocationBacktraces(
    NeXTCodeReflectionContextRef ContextRef,
    nextcode_metadataAllocationBacktraceIterator Call, void *ContextPtr) {
  return ContextRef->withContext<const char *>([&](auto *Context) {
    auto Error = Context->iterateMetadataAllocationBacktraces(
        [&](auto AllocationPtr, auto Count, auto Ptrs) {
          // Ptrs is an array of StoredPointer, but the callback expects an
          // array of nextcode_reflection_ptr_t. Those may are not always the same
          // type. (For example, nextcode_reflection_ptr_t can be 64-bit on 32-bit
          // systems, while StoredPointer is always the pointer size of the
          // target system.) Convert the array to an array of
          // nextcode_reflection_ptr_t.
          std::vector<nextcode_reflection_ptr_t> ConvertedPtrs{&Ptrs[0],
                                                            &Ptrs[Count]};
          Call(AllocationPtr, Count, ConvertedPtrs.data(), ContextPtr);
        });
    return returnableCString(ContextRef, Error);
  });
}

nextcode_async_task_slab_return_t
nextcode_reflection_asyncTaskSlabPointer(NeXTCodeReflectionContextRef ContextRef,
                                      nextcode_reflection_ptr_t AsyncTaskPtr) {
  return ContextRef->withContext<nextcode_async_task_slab_return_t>(
      [&](auto *Context) {
        // We only care about the AllocatorSlabPtr field. Disable child task and
        // async backtrace iteration to save wasted work.
        unsigned ChildTaskLimit = 0;
        unsigned AsyncBacktraceLimit = 0;

        auto [Error, TaskInfo] = Context->asyncTaskInfo(
            AsyncTaskPtr, ChildTaskLimit, AsyncBacktraceLimit);

        nextcode_async_task_slab_return_t Result = {};
        if (Error) {
          Result.Error = returnableCString(ContextRef, Error);
        }
        Result.SlabPtr = TaskInfo.AllocatorSlabPtr;
        return Result;
      });
}

nextcode_async_task_slab_allocations_return_t
nextcode_reflection_asyncTaskSlabAllocations(NeXTCodeReflectionContextRef ContextRef,
                                          nextcode_reflection_ptr_t SlabPtr) {
  return ContextRef->withContext<nextcode_async_task_slab_allocations_return_t>(
      [&](auto *Context) {
        auto [Error, Info] = Context->asyncTaskSlabAllocations(SlabPtr);

        nextcode_async_task_slab_allocations_return_t Result = {};
        if (Result.Error) {
          Result.Error = returnableCString(ContextRef, Error);
          return Result;
        }

        Result.NextSlab = Info.NextSlab;
        Result.SlabSize = Info.SlabSize;

        auto *Chunks = ContextRef->allocateTemporaryObject<
            std::vector<nextcode_async_task_allocation_chunk_t>>();
        Chunks->reserve(Info.Chunks.size());
        for (auto &Chunk : Info.Chunks) {
          nextcode_async_task_allocation_chunk_t ConvertedChunk;
          ConvertedChunk.Start = Chunk.Start;
          ConvertedChunk.Length = Chunk.Length;

          // This pedantry is required to properly template over *Context.
          ConvertedChunk.Kind = convertAllocationChunkKind<
              typename std::pointer_traits<decltype(Context)>::element_type>(
              Chunk.Kind);

          Chunks->push_back(ConvertedChunk);
        }

        Result.ChunkCount = Chunks->size();
        Result.Chunks = Chunks->data();

        return Result;
      });
}

nextcode_async_task_info_t
nextcode_reflection_asyncTaskInfo(NeXTCodeReflectionContextRef ContextRef,
                               nextcode_reflection_ptr_t AsyncTaskPtr) {
  return ContextRef->withContext<nextcode_async_task_info_t>([&](auto *Context) {
    // Limit the child task and async backtrace iteration to semi-reasonable
    // numbers to avoid doing excessive work on bad data.
    unsigned ChildTaskLimit = 1000000;
    unsigned AsyncBacktraceLimit = 1000;

    auto [Error, TaskInfo] = Context->asyncTaskInfo(
        AsyncTaskPtr, ChildTaskLimit, AsyncBacktraceLimit);

    nextcode_async_task_info_t Result = {};
    if (Error) {
      Result.Error = returnableCString(ContextRef, Error);
      return Result;
    }

    Result.Kind = TaskInfo.Kind;
    Result.EnqueuePriority = TaskInfo.EnqueuePriority;
    Result.IsChildTask = TaskInfo.IsChildTask;
    Result.IsFuture = TaskInfo.IsFuture;
    Result.IsGroupChildTask = TaskInfo.IsGroupChildTask;
    Result.IsAsyncLetTask = TaskInfo.IsAsyncLetTask;

    Result.MaxPriority = TaskInfo.MaxPriority;
    Result.IsCancelled = TaskInfo.IsCancelled;
    Result.IsStatusRecordLocked = TaskInfo.IsStatusRecordLocked;
    Result.IsEscalated = TaskInfo.IsEscalated;
    Result.HasIsRunning = TaskInfo.HasIsRunning;
    Result.IsRunning = TaskInfo.IsRunning;
    Result.IsEnqueued = TaskInfo.IsEnqueued;
    Result.Id = TaskInfo.Id;

    Result.HasThreadPort = TaskInfo.HasThreadPort;
    Result.ThreadPort = TaskInfo.ThreadPort;

    Result.RunJob = TaskInfo.RunJob;
    Result.AllocatorSlabPtr = TaskInfo.AllocatorSlabPtr;

    auto *ChildTasks =
        ContextRef
            ->allocateTemporaryObject<std::vector<nextcode_reflection_ptr_t>>();
    std::copy(TaskInfo.ChildTasks.begin(), TaskInfo.ChildTasks.end(),
              std::back_inserter(*ChildTasks));
    Result.ChildTaskCount = ChildTasks->size();
    Result.ChildTasks = ChildTasks->data();

    auto *AsyncBacktraceFrames = ContextRef->allocateSubsequentTemporaryObject<
        std::vector<nextcode_reflection_ptr_t>>();
    std::copy(TaskInfo.AsyncBacktraceFrames.begin(),
              TaskInfo.AsyncBacktraceFrames.end(),
              std::back_inserter(*AsyncBacktraceFrames));
    Result.AsyncBacktraceFramesCount = AsyncBacktraceFrames->size();
    Result.AsyncBacktraceFrames = AsyncBacktraceFrames->data();

    return Result;
  });
}

nextcode_actor_info_t
nextcode_reflection_actorInfo(NeXTCodeReflectionContextRef ContextRef,
                           nextcode_reflection_ptr_t ActorPtr) {
  return ContextRef->withContext<nextcode_actor_info_t>([&](auto *Context) {
    auto [Error, ActorInfo] = Context->actorInfo(ActorPtr);

    nextcode_actor_info_t Result = {};
    Result.Error = returnableCString(ContextRef, Error);
    Result.State = ActorInfo.State;
    Result.IsDistributedRemote = ActorInfo.IsDistributedRemote;
    Result.IsPriorityEscalated = ActorInfo.IsPriorityEscalated;
    Result.MaxPriority = ActorInfo.MaxPriority;
    Result.FirstJob = ActorInfo.FirstJob;

    Result.HasThreadPort = ActorInfo.HasThreadPort;
    Result.ThreadPort = ActorInfo.ThreadPort;

    return Result;
  });
}

nextcode_reflection_ptr_t
nextcode_reflection_nextJob(NeXTCodeReflectionContextRef ContextRef,
                         nextcode_reflection_ptr_t JobPtr) {
  return ContextRef->withContext<nextcode_reflection_ptr_t>([&](auto *Context) {
    return Context->nextJob(JobPtr);
  });
}
