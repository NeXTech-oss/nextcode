------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                     C O M M O N _ I T E R A T O R S                      --
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

with Elists; use Elists;

package body Common_Iterators is

   ---------------
   -- ELW_First --
   ---------------

   function ELW_First (L : Elist_Wrapper) return Elist_Wrapper_Cursor
   is
      EL : constant Elist_Id := Elist_Id (L);
   begin
      return
        Elist_Wrapper_Cursor (if Present (EL)
                              then First_Elmt (EL)
                              else No_Elmt);
   end ELW_First;

   --------------
   -- ELW_Next --
   --------------

   function ELW_Next (L : Elist_Wrapper;
                      C : Elist_Wrapper_Cursor)
                      return Elist_Wrapper_Cursor
   is
      pragma Unreferenced (L);
      E : constant Elmt_Id := Elmt_Id (C);
   begin
      return
        Elist_Wrapper_Cursor (if Present (E)
                              then Next_Elmt (E)
                              else No_Elmt);
   end ELW_Next;

   ---------------------
   -- ELW_Has_Element --
   ---------------------

   function ELW_Has_Element (L : Elist_Wrapper;
                             C : Elist_Wrapper_Cursor)
                             return Boolean
   is
      pragma Unreferenced (L);
      E : constant Elmt_Id := Elmt_Id (C);
   begin
      return Present (E);
   end ELW_Has_Element;

   -----------------
   -- ELW_Element --
   -----------------

   function ELW_Element (L : Elist_Wrapper;
                         C : Elist_Wrapper_Cursor)
                         return Node_Or_Entity_Id
   is
      pragma Unreferenced (L);
      E : constant Elmt_Id := Elmt_Id (C);
   begin
      return Node (E);
   end ELW_Element;

end Common_Iterators;
