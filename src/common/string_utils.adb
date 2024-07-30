------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                         S T R I N G - U T I L S                          --
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

with Ada.Characters.Handling;
with GNATCOLL.Utils;        use GNATCOLL.Utils;
with GNAT.Case_Util;

package body String_Utils is

   ----------------------
   -- Capitalize_First --
   ----------------------

   function Capitalize_First (S : String) return String is
      T : String := S;
   begin
      Capitalize_First (T);
      return T;
   end Capitalize_First;

   procedure Capitalize_First (S : in out String) is
   begin
      S (S'First) := Ada.Characters.Handling.To_Upper (S (S'First));
   end Capitalize_First;

   ---------------------------
   -- Case_Insensitive_Find --
   ---------------------------

   function Case_Insensitive_Find (SL : String_Lists.List; Item : String)
                                   return String_Lists.Cursor is
      use String_Lists;
   begin
      for C in SL.Iterate loop
         if Case_Insensitive_Equal (SL (C), Item) then
            return C;
         end if;
      end loop;
      return String_Lists.No_Element;
   end Case_Insensitive_Find;

   ----------------
   -- Hash_Image --
   ----------------

   function Hash_Image (N : Ada.Containers.Hash_Type) return String is
      Result : constant String := Ada.Containers.Hash_Type'Image (N);
   begin
      return Result (Result'First + 1 .. Result'Last);
   end Hash_Image;

   ----------------------
   -- Lower_Case_First --
   ----------------------

   procedure Lower_Case_First (S : in out String) is
   begin
      S (S'First) := Ada.Characters.Handling.To_Lower (S (S'First));
   end Lower_Case_First;

   function Lower_Case_First (S : String) return String is
      T : String := S;
   begin
      Lower_Case_First (T);
      return T;
   end Lower_Case_First;

   -----------------------
   -- Standard_Ada_Case --
   -----------------------

   function Standard_Ada_Case (S : String) return String is
      T : String := S;
   begin
      GNAT.Case_Util.To_Mixed (T);
      return T;
   end Standard_Ada_Case;

   -----------
   -- Trimi --
   -----------

   function Trimi (S : String; C : Character) return String is
      R : String (1 .. S'Last);
      J : Natural := 0;
   begin
      for I of S loop
         if I /= C then
            J := J + 1;
            R (J) := I;
         end if;
      end loop;
      return R (1 .. J);
   end Trimi;

end String_Utils;
