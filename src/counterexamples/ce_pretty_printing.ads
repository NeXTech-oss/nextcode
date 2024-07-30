------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                   C E _ P R E T T Y _ P R I N T I N G                    --
--                                                                          --
--                                 S p e c                                  --
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

with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with CE_Values;             use CE_Values;
with VC_Kinds;              use VC_Kinds;

package CE_Pretty_Printing is

   Dont_Display : constant CNT_Unbounded_String :=
     (Str   => To_Unbounded_String ("@not_display"),
      Count => 0,
      Elems => S_String_List.Empty);
   --  Value in a counterexample that should not be displayed

   function Make_CNT_Unbounded_String
     (Str : Unbounded_String;
      Cnt : Natural := 1;
      Els : S_String_List.List := S_String_List.Empty)
      return CNT_Unbounded_String
   with Pre => Cnt >= Natural (Els.Length);
   --  Create a CNT_Unbounded_String. Removes "trivial" counterexamples.

   procedure Print_Value_And_Attributes
     (Name        : Unbounded_String;
      Value       : Value_Type;
      Pretty_Line : in out Cntexample_Elt_Lists.List);
   --  Add the value and its attributes to Attributes

   function Print_Value (Value : Value_Type) return CNT_Unbounded_String;
   --  Return a string for a counterexample value. Attributes are ignored.

end CE_Pretty_Printing;
