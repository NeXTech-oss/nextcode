------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                      X T R E E _ T R A V E R S A L                       --
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

package Xtree_Traversal is
   --  This package provides generators for Why node accessors

   procedure Print_Traversal_Op_Declarations (O : in out Output_Record);
   --  Print kind-specific traversal operations

   procedure Print_Traverse_Body (O : in out Output_Record);
   --  Print a body implementing a recursive traversal of the Why syntax tree

   procedure Print_Traversal_Op_Stub_Declarations (O : in out Output_Record);
   --  Print stub declarations for traversal operations

   procedure Print_Traversal_Op_Stub_Bodies (O : in out Output_Record);
   --  Print bodies for traversal operations

   procedure Print_Treepr_Traversal_Op_Declarations (O : in out Output_Record);
   --  Print declarations of kind-specific traversal ops for tree printing

   procedure Print_Treepr_Traversal_Op_Bodies (O : in out Output_Record);
   --  Print bodies of traversal ops for tree printing

end Xtree_Traversal;
