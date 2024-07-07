//===--- APIGen.cpp - NeXTCode API Generation --------------------------------===//
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
//
//  This file implements the entrypoints into API file generation.
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/JSON.h"
#include "llvm/Support/raw_ostream.h"

#include "APIGen.h"

namespace nextcode {
namespace apigen {

void API::addSymbol(StringRef symbol, APILoc loc, APILinkage linkage,
                    APIFlags flags, APIAccess access,
                    APIAvailability availability) {
  auto *global = new (allocator) GlobalRecord(
      symbol, loc, linkage, flags, access, GVKind::Function, availability);
  globals.push_back(global);
}

ObjCInterfaceRecord *API::addObjCClass(StringRef name, APILinkage linkage,
                                       APILoc loc, APIAccess access,
                                       APIAvailability availability,
                                       StringRef superClassName) {
  auto *interface = new (allocator) ObjCInterfaceRecord(
      name, linkage, loc, access, availability, superClassName);
  interfaces.push_back(interface);
  return interface;
}

ObjCCategoryRecord *API::addObjCCategory(StringRef name, APILinkage linkage,
                                         APILoc loc, APIAccess access,
                                         APIAvailability availability,
                                         StringRef interface) {
  auto *category = new (allocator)
      ObjCCategoryRecord(name, linkage, loc, access, availability, interface);
  categories.push_back(category);
  return category;
}

void API::addObjCMethod(ObjCContainerRecord *record, StringRef name, APILoc loc,
                        APIAccess access, bool isInstanceMethod,
                        bool isOptional, APIAvailability availability) {
  auto method = new (allocator) ObjCMethodRecord(
      name, loc, access, isInstanceMethod, isOptional, availability);
  record->methods.push_back(method);
}

static void serialize(llvm::json::OStream &OS, APIAccess access) {
  switch (access) {
  case APIAccess::Public:
    OS.attribute("access", "public");
    break;
  case APIAccess::Private:
    OS.attribute("access", "private");
    break;
  case APIAccess::Project:
    OS.attribute("access", "project");
    break;
  case APIAccess::Unknown:
    break;
  }
}

static void serialize(llvm::json::OStream &OS, APIAvailability availability) {
  if (availability.empty())
    return;
  if (!availability.introduced.empty())
    OS.attribute("introduced", availability.introduced);
  if (!availability.obsoleted.empty())
    OS.attribute("obsoleted", availability.obsoleted);
  if (availability.unavailable)
    OS.attribute("unavailable", availability.unavailable);
}

static void serialize(llvm::json::OStream &OS, APILinkage linkage) {
  switch (linkage) {
  case APILinkage::Exported:
    OS.attribute("linkage", "exported");
    break;
  case APILinkage::Reexported:
    OS.attribute("linkage", "re-exported");
    break;
  case APILinkage::Internal:
    OS.attribute("linkage", "internal");
    break;
  case APILinkage::External:
    OS.attribute("linkage", "external");
    break;
  case APILinkage::Unknown:
    // do nothing;
    break;
  }
}

static void serialize(llvm::json::OStream &OS, APILoc loc) {
  OS.attribute("file", loc.getFilename());
}

static void serialize(llvm::json::OStream &OS, const GlobalRecord &record) {
  OS.object([&]() {
    OS.attribute("name", record.name);
    serialize(OS, record.access);
    serialize(OS, record.loc);
    serialize(OS, record.linkage);
    serialize(OS, record.availability);
  });
}

static void serialize(llvm::json::OStream &OS, const ObjCMethodRecord &record) {
  OS.object([&]() {
    OS.attribute("name", record.name);
    serialize(OS, record.access);
    serialize(OS, record.loc);
    serialize(OS, record.linkage);
    serialize(OS, record.availability);
    if (record.isOptional)
      OS.attribute("optional", record.isOptional);
  });
}

static bool sortAPIRecords(const APIRecord *base, const APIRecord *compare) {
  return base->name < compare->name;
}

static void serialize(llvm::json::OStream &OS,
                      const ObjCInterfaceRecord &record) {
  OS.object([&]() {
    OS.attribute("name", record.name);
    serialize(OS, record.access);
    serialize(OS, record.loc);
    serialize(OS, record.linkage);
    serialize(OS, record.availability);
    OS.attribute("super", record.superClassName);
    OS.attributeArray("instanceMethods", [&]() {
      for (auto &method : record.methods) {
        if (method->isInstanceMethod)
          serialize(OS, *method);
      }
    });
    OS.attributeArray("classMethods", [&]() {
      for (auto &method : record.methods) {
        if (!method->isInstanceMethod)
          serialize(OS, *method);
      }
    });
  });
}

static void serialize(llvm::json::OStream &OS,
                      const ObjCCategoryRecord &record) {
  OS.object([&]() {
    OS.attribute("name", record.name);
    serialize(OS, record.access);
    serialize(OS, record.loc);
    serialize(OS, record.linkage);
    serialize(OS, record.availability);
    OS.attribute("interface", record.interface);
    OS.attributeArray("instanceMethods", [&]() {
      for (auto &method : record.methods) {
        if (method->isInstanceMethod)
          serialize(OS, *method);
      }
    });
    OS.attributeArray("classMethods", [&]() {
      for (auto &method : record.methods) {
        if (!method->isInstanceMethod)
          serialize(OS, *method);
      }
    });
  });
}

void API::writeAPIJSONFile(llvm::raw_ostream &os, bool PrettyPrint) {
  unsigned indentSize = PrettyPrint ? 2 : 0;
  llvm::json::OStream JSON(os, indentSize);

  JSON.object([&]() {
    JSON.attribute("target", target.str());
    JSON.attributeArray("globals", [&]() {
      llvm::sort(globals, sortAPIRecords);
      for (const auto *g : globals)
        serialize(JSON, *g);
    });
    JSON.attributeArray("interfaces", [&]() {
      llvm::sort(interfaces, sortAPIRecords);
      for (const auto *i : interfaces)
        serialize(JSON, *i);
    });
    JSON.attributeArray("categories", [&]() {
      llvm::sort(categories, sortAPIRecords);
      for (const auto *c : categories)
        serialize(JSON, *c);
    });
    JSON.attribute("version", "1.0");
  });
}

} // end namespace apigen
} // end namespace nextcode
