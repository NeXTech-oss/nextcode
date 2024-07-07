//===--- EnumTraits.h - Traits for densely-packed enums ---------*- C++ -*-===//
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
///
///  This file defines the EnumTraits concept, which can be used to
///  communicate information about an enum type's enumerators that currently
///  can't be recovered from the compiler.
///
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_ENUMTRAITS_H
#define NEXTCODE_BASIC_ENUMTRAITS_H

namespace nextcode {

/// A simple traits concept for recording the number of cases in an enum.
///
///  template <> class EnumTraits<WdigetKind> {
///    static constexpr size_t NumValues = NumWidgetKinds;
///  };
template <class E>
struct EnumTraits;

} // end namespace nextcode

#endif
