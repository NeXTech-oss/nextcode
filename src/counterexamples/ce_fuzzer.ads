------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                    G N A T 2 W H Y _ C E _ F U Z Z E R                   --
--                                                                          --
--                                 B o d y                                  --
--                                                                          --
-------------------------------------------------------------------------------
--
-- Copyright (c) 2024, NeXTech Corporation. All rights reserved.
-- DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
--
-- This code is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
-- version 2 for more details (a copy is included in the LICENSE file that
-- accompanied this code).
--
-- Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
--             Tural Ghuliev (turalquliyev@it-gss.com)
--
-------------------------------------------------------------------------------
------------------------------------------------------------------------------

with Ada.Numerics.Big_Numbers.Big_Integers;
use  Ada.Numerics.Big_Numbers.Big_Integers;
with CE_Values;   use CE_Values;
with Einfo.Utils; use Einfo.Utils;
with Types;       use Types;

package CE_Fuzzer is

   function Fuzz_Integer_Value (Ty : Entity_Id) return Value_Type
     with Pre => Is_Integer_Type (Ty);
   --  Return a Value_Type in the range of the Rep_Ty type randomly chosen
   --  among a set of values known to often lead to errors.

   function Fuzz_Record_Value (Ty : Entity_Id) return Value_Type
     with Pre => Is_Record_Type (Ty);
   --  Return a Value_Type of record kind with fields filled with fuzzed
   --  values.

end CE_Fuzzer;
