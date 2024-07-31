------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                       X T R E E _ B U I L D E R S                        --
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

with Outputs; use Outputs;

package Xtree_Builders is
   --  This package provides generators for Why node builders

   procedure Print_Class_Wide_Builder_Declarations (O : in out Output_Record);
   --  Print builder declarations for class-wide ids

   procedure Print_Class_Wide_Builder_Bodies (O : in out Output_Record);
   --  Print builder bodies for class-wide ids

   Checked_Default_Value : constant String := "Is_Checked";
   --  Name of the constant used to initialize the field Checked. The
   --  initialization depends on the kind of constructor that we are
   --  executing; this dependancy is encapsulated into a local variable
   --  whose name is Checked_Default_Value.

end Xtree_Builders;
