//===--- DefineDiagnosticMacros.h - Shared Diagnostic Macros ----*- C++ -*-===//
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
//  This file defines macros shared across diagnostic definition files.
//
//===----------------------------------------------------------------------===//

// Define macros
#ifdef DEFINE_DIAGNOSTIC_MACROS

#if !(defined(DIAG) || (defined(ERROR) && defined(WARNING) && defined(NOTE) && \
                        defined(REMARK)))
#error Must define either DIAG or the set {ERROR,WARNING,NOTE,REMARK}
#endif

#ifndef ERROR
#define ERROR(ID, Options, Text, Signature)                                    \
  DIAG(ERROR, ID, Options, Text, Signature)
#endif

#ifndef WARNING
#define WARNING(ID, Options, Text, Signature)                                  \
  DIAG(WARNING, ID, Options, Text, Signature)
#endif

#ifndef NOTE
#define NOTE(ID, Options, Text, Signature)                                     \
  DIAG(NOTE, ID, Options, Text, Signature)
#endif

#ifndef REMARK
#define REMARK(ID, Options, Text, Signature)                                   \
  DIAG(REMARK, ID, Options, Text, Signature)
#endif

#ifndef FIXIT
#define FIXIT(ID, Text, Signature)
#endif

#undef DEFINE_DIAGNOSTIC_MACROS
#endif

// Undefine macros
#ifdef UNDEFINE_DIAGNOSTIC_MACROS

#ifndef DIAG_NO_UNDEF

#if defined(DIAG)
#undef DIAG
#endif

#undef REMARK
#undef NOTE
#undef WARNING
#undef ERROR
#undef FIXIT

#endif

#undef UNDEFINE_DIAGNOSTIC_MACROS
#endif
