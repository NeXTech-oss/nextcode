//===--- BlockList.cpp - BlockList utilities ------------------------------===//
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
//
//===----------------------------------------------------------------------===//

#include "llvm/ADT/STLExtras.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/YAMLParser.h"
#include "llvm/Support/YAMLTraits.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/BlockList.h"
#include "nextcode/Basic/SourceManager.h"

struct nextcode::BlockListStore::Implementation {
  SourceManager SM;
  llvm::StringMap<std::vector<BlockListAction>> ModuleActionDict;
  llvm::StringMap<std::vector<BlockListAction>> ProjectActionDict;
  void addConfigureFilePath(StringRef path);
  bool hasBlockListAction(StringRef key, BlockListKeyKind keyKind,
                          BlockListAction action);
  void collectBlockList(llvm::yaml::Node *N, BlockListAction action);

  llvm::StringMap<std::vector<BlockListAction>> *getDictToUse(BlockListKeyKind kind) {
    switch (kind) {
    case BlockListKeyKind::ModuleName:
      return &ModuleActionDict;
    case BlockListKeyKind::ProjectName:
      return &ProjectActionDict;
    case BlockListKeyKind::Undefined:
      return nullptr;
    }
  }
  static std::string getScalaString(llvm::yaml::Node *N) {
    llvm::SmallString<64> Buffer;
    if (auto *scala = dyn_cast<llvm::yaml::ScalarNode>(N)) {
      return scala->getValue(Buffer).str();
    }
    return std::string();
  }
};

nextcode::BlockListStore::BlockListStore(): Impl(*new Implementation()) {}

nextcode::BlockListStore::~BlockListStore() { delete &Impl; }

bool nextcode::BlockListStore::hasBlockListAction(StringRef key,
    BlockListKeyKind keyKind, BlockListAction action) {
  return Impl.hasBlockListAction(key, keyKind, action);
}

void nextcode::BlockListStore::addConfigureFilePath(StringRef path) {
  Impl.addConfigureFilePath(path);
}

bool nextcode::BlockListStore::Implementation::hasBlockListAction(StringRef key,
    BlockListKeyKind keyKind, BlockListAction action) {
  auto *dict = getDictToUse(keyKind);
  assert(dict);
  auto it = dict->find(key);
  if (it == dict->end())
    return false;
  return llvm::is_contained(it->second, action);
}

void nextcode::BlockListStore::Implementation::collectBlockList(llvm::yaml::Node *N,
                                                      BlockListAction action) {
  namespace yaml = llvm::yaml;
  auto *pair = dyn_cast<yaml::KeyValueNode>(N);
  if (!pair)
    return;
  std::string rawKey = getScalaString(pair->getKey());
  auto keyKind = llvm::StringSwitch<BlockListKeyKind>(rawKey)
#define CASE(X) .Case(#X, BlockListKeyKind::X)
    CASE(ModuleName)
    CASE(ProjectName)
#undef CASE
    .Default(BlockListKeyKind::Undefined);
  if (keyKind == BlockListKeyKind::Undefined)
    return;
  auto *dictToUse = getDictToUse(keyKind);
  assert(dictToUse);
  auto *seq = dyn_cast<yaml::SequenceNode>(pair->getValue());
  if (!seq)
    return;
  for (auto &node: *seq) {
    std::string name = getScalaString(&node);
    dictToUse->insert({name, std::vector<BlockListAction>()})
      .first->second.push_back(action);
  }
}

void nextcode::BlockListStore::Implementation::addConfigureFilePath(StringRef path) {
  namespace yaml = llvm::yaml;

  // Load the input file.
  llvm::ErrorOr<std::unique_ptr<llvm::MemoryBuffer>> FileBufOrErr =
    vfs::getFileOrSTDIN(*SM.getFileSystem(), path,
                        /*FileSize*/-1, /*RequiresNullTerminator*/true,
                        /*IsVolatile*/false, /*RetryCount*/30);
  if (!FileBufOrErr) {
    return;
  }
  StringRef Buffer = FileBufOrErr->get()->getBuffer();
  yaml::Stream Stream(llvm::MemoryBufferRef(Buffer, path),
                      SM.getLLVMSourceMgr());
  for (auto DI = Stream.begin(); DI != Stream.end(); ++ DI) {
    assert(DI != Stream.end() && "Failed to read a document");
    yaml::Node *N = DI->getRoot();
    for (auto &pair: *dyn_cast<yaml::MappingNode>(N)) {
      std::string key = getScalaString(pair.getKey());
      auto action = llvm::StringSwitch<BlockListAction>(key)
#define BLOCKLIST_ACTION(X) .Case(#X, BlockListAction::X)
#include "nextcode/Basic/BlockListAction.def"
        .Default(BlockListAction::Undefined);
      if (action == BlockListAction::Undefined)
        continue;
      auto *map = dyn_cast<yaml::MappingNode>(pair.getValue());
      if (!map)
        continue;
      for (auto &innerPair: *map) {
        collectBlockList(&innerPair, action);
      }
    }
  }
}
