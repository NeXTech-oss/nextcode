------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                        X T R E E _ W H Y _ A S T                         --
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

--  This package provides procedures that generate the following code to
--  interface Gnat with Why3:
--
--    1. Ada functions to convert the Xtree Gnat AST to a Json value
--       (procedures Print_Ada_To_Json)
--
--    2. OCaml type definition corresponding to the Xtree Gnat AST as a
--       algebraic datatype
--       (procedures Print_OCaml_*_Type(s))
--
--    3. OCaml functions to convert a Json value to an OCaml Gnat AST
--       (procedures Print_OCaml_*_From_Json)
--
--   The code always deals separately with the enumeration Why_Node, the
--   opaque identifiers, and the enumerations from Why.Sinfo.
--
--   A module in gnatwhy3 converts the AST to the original Why3 AST from
--   Ptree.
--
--      Ada Gnat AST ->(1.) Json ->(3.) OCaml Gnat AST(2.) -> Why3 Ast
--      ^^^^^^ gnat2why ^^^^^ | ^^^^^^^^^^^^^^ gnat2why ^^^^^^^^^^^^^^
--
--  This approach assures that changes in either the Gnat AST or the Why3 Ast

with Outputs; use Outputs;

package Xtree_Why_AST is

   procedure Print_Ada_To_Json (O : in out Output_Record);

   procedure Print_OCaml_Why_Sinfo_Types (O : in out Output_Record);

   procedure Print_OCaml_Why_Node_Type (O : in out Output_Record);

   procedure Print_OCaml_Opaque_Ids (O : in out Output_Record);

   procedure Print_OCaml_Tags (O : in out Output_Record);

   procedure Print_OCaml_Why_Node_From_Json (O : in out Output_Record);

   procedure Print_OCaml_Why_Sinfo_Types_From_Json (O : in out Output_Record);

   procedure Print_OCaml_Opaque_Ids_From_Json (O : in out Output_Record);

   procedure Print_OCaml_Coercions (O : in out Output_Record);

end Xtree_Why_AST;
