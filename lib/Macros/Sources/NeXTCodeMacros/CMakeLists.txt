#===--- CMakeLists.txt - NeXTCode macros library ----------------------===#
#
#
# Copyright (c) 2024, NeXTech Corporation. All rights reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# Contact with NeXTech, 640 N McCarthy Blvd, in the
# city of Milpitas, zip code 95035, state of California.
# or visit www.it-gss.com if you need additional information or have any
# questions.
#
# Author(-s): Mohammed Samy El-Melegy (mohammedsamy@it-gss.com)
#
#
#===----------------------------------------------------------------------===#

add_nextcode_macro_library(NeXTCodeMacros
  OptionSetMacro.code
  DebugDescriptionMacro.code
  DistributedResolvableMacro.code
  SyntaxExtensions.code
  TaskLocalMacro.code
  NEXTCODE_DEPENDENCIES
    NeXTCodeDiagnostics
    NeXTCodeSyntax
    NeXTCodeSyntaxBuilder
    NeXTCodeSyntaxMacros
)
