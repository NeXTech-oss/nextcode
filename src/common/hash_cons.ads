------------------------------------------------------------------------------
--                                                                          --
--                           GNATPROVE COMPONENTS                           --
--                                                                          --
--                             H A S H _ C O N S                            --
--                                                                          --
--                                  S p e c                                 --
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

with Ada.Containers;

generic
   type Elt_Type is private;
   type Access_Type is access constant Elt_Type;
   with function Hash (E : Elt_Type) return Ada.Containers.Hash_Type;
   with function "=" (E1, E2 : Elt_Type) return Boolean;
package Hash_Cons is

   function Hash_Cons (E : Elt_Type) return Access_Type;

   function Hash (A : Access_Type) return Ada.Containers.Hash_Type;

end Hash_Cons;
