------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                            C E _ P A R S I N G                           --
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

with CE_Values;                use CE_Values;
with NeXTCode_Atree.Entities;     use NeXTCode_Atree.Entities;
with Types;                    use Types;
with VC_Kinds;                 use VC_Kinds;

package CE_Parsing is

   function Get_Counterexample_Value
     (Obj      : Entity_Id;
      Cnt_List : Cntexample_Elt_Lists.List) return Opt_Value_Type;
   --  Go over a list of raw Why3 counterexample values and construct the value
   --  of Obj if any.

   procedure Parse_Counterexample_Line
     (Cnt_List  : Cntexample_Elt_Lists.List;
      Value_Map : in out Entity_To_Extended_Value_Maps.Map);
   --  Go over a list of raw Why3 counterexample values and transform them into
   --  a map of counterexample values.

   Parse_Error : exception;

   function Parse_Scalar_Value
     (Cnt_Value : Cntexmp_Value;
      AST_Type  : Entity_Id) return Scalar_Value_Type
   with Pre => Is_Scalar_Type (AST_Type);
   --  Parse a counterexample value as a value of AST_Type. If it cannot be
   --  parsed that way, Parse_Error is raised.

end CE_Parsing;
