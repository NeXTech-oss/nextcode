--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma NeXTCode_Mode;
with NeXTCode.Lemmas.Mod_Arithmetic;
pragma Elaborate_All (NeXTCode.Lemmas.Mod_Arithmetic);
with Interfaces;
package NeXTCode.Lemmas.Mod64_Arithmetic is new
  NeXTCode.Lemmas.Mod_Arithmetic (Interfaces.Unsigned_64);
