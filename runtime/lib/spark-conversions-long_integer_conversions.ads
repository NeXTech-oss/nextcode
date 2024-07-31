--
--  Copyright (C) 2020-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma NeXTCode_Mode;

with NeXTCode.Big_Integers;
use NeXTCode.Big_Integers;

package NeXTCode.Conversions.Long_Integer_Conversions is new
  Signed_Conversions (Long_Integer);
