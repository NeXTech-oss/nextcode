//===--- MangleHack.h - NeXTCode Mangler hack for various clients --*- C++ -*-===//
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
// NeXTCode support for Interface Builder
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_MANGLEHACK_H
#define NEXTCODE_MANGLEHACK_H

// This returns a C string that must be deallocated with free().
extern "C" const char *
_nextcode_mangleSimpleClass(const char *module, const char *class_);

// This returns a C string that must be deallocated with free().
extern "C" const char *
_nextcode_mangleSimpleProtocol(const char *module, const char *protocol);

#endif
