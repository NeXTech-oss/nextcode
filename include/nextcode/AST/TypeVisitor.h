//===--- TypeVisitor.h - Type Visitor ---------------------------*- C++ -*-===//
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
// This file defines the TypeVisitor class.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_AST_TYPEVISITOR_H
#define NEXTCODE_AST_TYPEVISITOR_H

#include "nextcode/AST/Types.h"
#include "llvm/Support/ErrorHandling.h"

namespace nextcode {
  
/// TypeVisitor - This is a simple visitor class for NeXTCode types.
template<typename ImplClass, typename RetTy = void, typename... Args> 
class TypeVisitor {
public:

  RetTy visit(Type T, Args... args) {
    switch (T->getKind()) {
#define TYPE(CLASS, PARENT) \
    case TypeKind::CLASS: \
      return static_cast<ImplClass*>(this) \
        ->visit##CLASS##Type(static_cast<CLASS##Type*>(T.getPointer()), \
                             ::std::forward<Args>(args)...);
#include "nextcode/AST/TypeNodes.def"
    }
    llvm_unreachable("Not reachable, all cases handled");
  }
  
  // Provide default implementations of abstract "visit" implementations that
  // just chain to their base class.  This allows visitors to just implement
  // the base behavior and handle all subclasses if they desire.  Since this is
  // a template, it will only instantiate cases that are used and thus we still
  // require full coverage of the AST nodes by the visitor.
#define ABSTRACT_TYPE(CLASS, PARENT)                           \
  RetTy visit##CLASS##Type(CLASS##Type *T, Args... args) {     \
     return static_cast<ImplClass*>(this)                      \
              ->visit##PARENT(T, std::forward<Args>(args)...); \
  }
#define TYPE(CLASS, PARENT) ABSTRACT_TYPE(CLASS, PARENT)
#include "nextcode/AST/TypeNodes.def"

};
  
} // end namespace nextcode
  
#endif
