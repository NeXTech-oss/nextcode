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

with Ada.Containers.Hashed_Sets;

package body Hash_Cons is

   function Equivalent_Elements (Left, Right : Access_Type) return Boolean;

   package Cons_Sets is new
     Ada.Containers.Hashed_Sets
       (Element_Type        => Access_Type,
        Hash                => Hash,
        Equivalent_Elements => Equivalent_Elements,
        "="                 => "=");

   Cons_Table : Cons_Sets.Set := Cons_Sets.Empty_Set;

   -------------------------
   -- Equivalent_Elements --
   -------------------------

   function Equivalent_Elements (Left, Right : Access_Type) return Boolean is
   begin
      return Left.all = Right.all;
   end Equivalent_Elements;

   ----------
   -- Hash --
   ----------

   function Hash (A : Access_Type) return Ada.Containers.Hash_Type is
   begin
      if A /= null then
         return Hash (A.all);
      else
         return 0;
      end if;
   end Hash;

   ---------------
   -- Hash_Cons --
   ---------------

   function Hash_Cons (E : Elt_Type) return Access_Type is
      use Cons_Sets;
      C : constant Cursor := Cons_Table.Find (E'Unrestricted_Access);
   begin
      if Has_Element (C) then
         return Element (C);
      else
         declare
            N_Ptr : constant Access_Type := new Elt_Type'(E);
         begin
            Cons_Table.Insert (N_Ptr);
            return N_Ptr;
         end;
      end if;
   end Hash_Cons;

end Hash_Cons;
