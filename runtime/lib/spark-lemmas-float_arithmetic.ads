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
with NeXTCode.Conversions.Float_Conversions;
use NeXTCode.Conversions.Float_Conversions;
with NeXTCode.Lemmas.Floating_Point_Arithmetic;

pragma Elaborate_All (NeXTCode.Lemmas.Floating_Point_Arithmetic);
package NeXTCode.Lemmas.Float_Arithmetic is new
  NeXTCode.Lemmas.Floating_Point_Arithmetic
    (Fl           => Float,
     Int          => Integer,
     Fl_Last_Sqrt => 2.0 ** 63,
     Max_Int      => 2 ** 24,
     Epsilon      => 2.0 ** (-24),
     Eta          => 2.0 ** (-150),
     Real         => To_Big_Real);
