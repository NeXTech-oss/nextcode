------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--          G N A T 2 W H Y - D A T A _ D E C O M P O S I T I O N           --
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

with Ada.Strings.Hash;
with Snames; use Snames;
with Types;  use Types;
with Uintp;  use Uintp;

package Gnat2Why.Data_Decomposition is

   subtype Size_Attribute_Id is Attribute_Id with
     Static_Predicate => Size_Attribute_Id in Attribute_Size
                                            | Attribute_Value_Size
                                            | Attribute_Object_Size;

   subtype Repr_Attribute_Id is Attribute_Id with
     Static_Predicate => Repr_Attribute_Id in Attribute_Alignment
                                            | Size_Attribute_Id;

   function Get_Attribute_Value
     (E       : Entity_Id;
      Attr_Id : Repr_Attribute_Id) return Uint;
   --  Return the value of E'Attr_Id or No_Uint if not known

   procedure Read_Data_Decomposition_JSON_File;
   --  Read a data decomposition JSON file and extract relevant information in
   --  global map Data_Decomposition_Table.

end Gnat2Why.Data_Decomposition;
