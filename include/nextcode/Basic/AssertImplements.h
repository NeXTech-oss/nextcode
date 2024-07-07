//===--- AssertImplements.h - Assert that a class overrides a function ----===//
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
//  This file provides the ASSERT_IMPLEMENTS macro, which statically
//  asserts that a class implements a function.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_ASSERTIMPLEMENTS_H
#define NEXTCODE_BASIC_ASSERTIMPLEMENTS_H

#include <type_traits>

namespace nextcode {
  template <class ExpectedType, class BaseType>
  BaseType get_implementing_class_helper(ExpectedType BaseType::*member);

#define GET_IMPLEMENTING_CLASS(DerivedClass, MemberName, ExpectedType)          \
  decltype(::nextcode::get_implementing_class_helper<ExpectedType>(                \
                                                    &DerivedClass::MemberName))

/// Statically assert that DerivedClass overrides the instance method
/// MemberName (at overload ExpectedType).  It doesn't have to be
/// overridden directly in DerivedClass; it just can't still be the
/// implementation from BaseClass.
#define ASSERT_IMPLEMENTS(DerivedClass, BaseClass, MemberName, ExpectedType)    \
  static_assert(!::std::is_same<BaseClass,                                      \
                                GET_IMPLEMENTING_CLASS(DerivedClass, MemberName,\
                                                       ExpectedType)            \
                               >::value,                                        \
                "" #DerivedClass " does not properly override " #MemberName)


  template <class T, T *impl0, T *impl1> struct is_same_pointer {
    enum { value = false };
  };

  template <class T, T *impl> struct is_same_pointer<T, impl, impl> {
    enum { value = true };
  };

/// Statically assert that DerivedClass overrides the static method
/// MemberName (at overload ExpectedType).  It doesn't have to be
/// overridden directly in DerivedClass; it just can't still be the
/// implementation from BaseClass.
#define ASSERT_IMPLEMENTS_STATIC(DerivedClass, BaseClass, MemberName,           \
                                 ExpectedType)                                  \
  static_assert(!::nextcode::is_same_pointer<ExpectedType,                         \
                                          &BaseClass::MemberName,               \
                                          &DerivedClass::MemberName>::value,    \
                "" #DerivedClass " does not properly override " #MemberName)

} // end namespace nextcode

#endif // NEXTCODE_BASIC_ASSERTIMPLEMENTS_H