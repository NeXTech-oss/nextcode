------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                            W H Y - T Y P E S                             --
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

with Ada.Containers.Doubly_Linked_Lists;
with Ada.Containers.Hashed_Maps;
with Ada.Containers.Ordered_Sets;
with Common_Containers; use Common_Containers;
package Why.Types is

   --  This package hierarchy provides basic types for Why syntax trees

   type Why_Int is range -2 ** 31 .. +2 ** 31 - 1;

   type Why_Node_Set is new Why_Int range 0 .. Why_Int'Last;

   type Why_Node_Id is new Why_Node_Set range 0 .. Why_Node_Set'Last / 2;

   type Why_Node_List is new Why_Node_Set
     range Why_Node_Set (Why_Node_Id'Last + 1) .. Why_Node_Set'Last;

   Why_Empty : constant := 0;

   function No (N : Why_Node_Id) return Boolean is (N = Why_Empty);
   --  Returns True if N is the empty node, False otherwise

   function Present (N : Why_Node_Id) return Boolean is (N /= Why_Empty);
   --  Returns True if N is not the empty node, False otherwise

   subtype Symbol_Set is Symbol_Sets.Set;

   function Why_Node_Hash (X : Why_Node_Id) return Ada.Containers.Hash_Type is
     (Ada.Containers.Hash_Type (X));

   package Why_Node_Maps is new Ada.Containers.Hashed_Maps
     (Key_Type        => Why_Node_Id,
      Element_Type    => Why_Node_Id,
      Hash            => Why_Node_Hash,
      Equivalent_Keys => "=");

   package Why_Node_Maps_Lists is new Ada.Containers.Doubly_Linked_Lists
     (Element_Type => Why_Node_Maps.Map,
      "="          => Why_Node_Maps."=");

   package Why_Node_Sets is new Ada.Containers.Ordered_Sets
     (Element_Type => Why_Node_Id);

   package Why_Node_Graphs is new Ada.Containers.Hashed_Maps
     (Key_Type        => Why_Node_Id,
      Element_Type    => Why_Node_Sets.Set,
      Hash            => Why_Node_Hash,
      Equivalent_Keys => "=",
      "="             => Why_Node_Sets."=");

end Why.Types;
