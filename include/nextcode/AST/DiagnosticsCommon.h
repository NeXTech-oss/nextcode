//===--- DiagnosticsCommon.h - Shared Diagnostic Definitions ----*- C++ -*-===//
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
/// \file
/// This file defines common diagnostics for the whole compiler, as well
/// as some diagnostic infrastructure.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_DIAGNOSTICSCOMMON_H
#define NEXTCODE_DIAGNOSTICSCOMMON_H

#include "nextcode/AST/DiagnosticEngine.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Config.h"

namespace nextcode {
  template<typename ...ArgTypes>
  struct Diag;

  namespace detail {
    // These templates are used to help extract the type arguments of the
    // DIAG/ERROR/WARNING/NOTE/REMARK/FIXIT macros.
    template<typename T>
    struct DiagWithArguments;
    
    template<typename ...ArgTypes>
    struct DiagWithArguments<void(ArgTypes...)> {
      typedef Diag<ArgTypes...> type;
    };

    template <typename T>
    struct StructuredFixItWithArguments;

    template <typename... ArgTypes>
    struct StructuredFixItWithArguments<void(ArgTypes...)> {
      typedef StructuredFixIt<ArgTypes...> type;
    };
  } // end namespace detail

  enum class StaticSpellingKind : uint8_t;

  namespace diag {

    enum class RequirementKind : uint8_t;

    using DeclAttribute = const DeclAttribute *;

  // Declare common diagnostics objects with their appropriate types.
#define DIAG(KIND,ID,Options,Text,Signature) \
    extern detail::DiagWithArguments<void Signature>::type ID;
#define FIXIT(ID, Text, Signature) \
    extern detail::StructuredFixItWithArguments<void Signature>::type ID;
#include "DiagnosticsCommon.def"
  } // end namespace diag
} // end namespace nextcode

#endif
