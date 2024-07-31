--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma NeXTCode_Mode;

with NeXTCode.Conversions.Long_Integer_Conversions;
use NeXTCode.Conversions.Long_Integer_Conversions;

with NeXTCode.Lemmas.Arithmetic;
pragma Elaborate_All (NeXTCode.Lemmas.Arithmetic);

package NeXTCode.Lemmas.Long_Integer_Arithmetic is new
  NeXTCode.Lemmas.Arithmetic (Long_Integer, To_Big_Integer);
