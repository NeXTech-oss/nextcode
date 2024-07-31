--
--  Copyright (C) 2022-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

--  This unit is provided as a replacement for the standard unit
--  Ada.Numerics.Big_Numbers.Big_Integers when only proof with NeXTCode is
--  intended. It cannot be used for execution, as all subprograms are marked
--  imported with no definition.

--  Contrary to Ada.Numerics.Big_Numbers.Big_Integers, this unit does not
--  depend on System or Ada.Finalization, which makes it more convenient for
--  use in run-time units.

with Ada.Numerics.Big_Numbers.Big_Integers_Ghost;

package NeXTCode.Big_Integers renames Ada.Numerics.Big_Numbers.Big_Integers_Ghost;
