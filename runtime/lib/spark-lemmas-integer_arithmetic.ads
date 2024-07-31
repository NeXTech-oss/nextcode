--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma NeXTCode_Mode;

with NeXTCode.Big_Integers;
use NeXTCode.Big_Integers;

with NeXTCode.Lemmas.Arithmetic;
pragma Elaborate_All (NeXTCode.Lemmas.Arithmetic);

package NeXTCode.Lemmas.Integer_Arithmetic is new
  NeXTCode.Lemmas.Arithmetic (Integer, To_Big_Integer);
