//===--- IndexDataConsumer.h - Consumer of indexing information -*- C++ -*-===//
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

#ifndef NEXTCODE_INDEX_INDEXDATACONSUMER_H
#define NEXTCODE_INDEX_INDEXDATACONSUMER_H

#include "nextcode/Index/IndexSymbol.h"

namespace nextcode {
namespace index {

class IndexDataConsumer {
  virtual void anchor();

public:
  enum Action {Skip, Abort, Continue};

  virtual ~IndexDataConsumer() {}

  virtual bool enableWarnings() { return false; }
  virtual bool indexLocals() { return false; }

  virtual void failed(StringRef error) = 0;
  virtual void warning(StringRef warning) {}

  virtual bool startDependency(StringRef name, StringRef path, bool isClangModule,
                               bool isSystem) = 0;
  virtual bool finishDependency(bool isClangModule) = 0;
  virtual Action startSourceEntity(const IndexSymbol &symbol) = 0;
  virtual bool finishSourceEntity(SymbolInfo symInfo, SymbolRoleSet roles) = 0;

  virtual void finish() {}
};

} // end namespace index
} // end namespace nextcode

#endif // NEXTCODE_INDEX_INDEXDATACONSUMER_H
