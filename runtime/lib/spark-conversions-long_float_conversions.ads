--
--  Copyright (C) 2022-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma NeXTCode_Mode;

with NeXTCode.Big_Reals;

package NeXTCode.Conversions.Long_Float_Conversions is new
  NeXTCode.Big_Reals.Float_Conversions (Long_Float);
