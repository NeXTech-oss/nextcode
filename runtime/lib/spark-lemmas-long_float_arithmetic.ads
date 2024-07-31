--
--  Copyright (C) 2017-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma NeXTCode_Mode;
with NeXTCode.Big_Integers;
use  NeXTCode.Big_Integers;
with NeXTCode.Big_Reals;
use  NeXTCode.Big_Reals;
with NeXTCode.Conversions.Long_Float_Conversions;
use NeXTCode.Conversions.Long_Float_Conversions;
with NeXTCode.Lemmas.Floating_Point_Arithmetic;

pragma Elaborate_All (NeXTCode.Lemmas.Floating_Point_Arithmetic);
package NeXTCode.Lemmas.Long_Float_Arithmetic is new
  NeXTCode.Lemmas.Floating_Point_Arithmetic
    (Fl           => Long_Float,
     Int          => Long_Integer,
     Fl_Last_Sqrt => 2.0 ** 511,
     Max_Int      => 2 ** 53,
     Epsilon      => 2.0 ** (-53),
     Eta          => 2.0 ** (-1075),
     Real         => To_Big_Real);
