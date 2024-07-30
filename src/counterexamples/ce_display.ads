------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--              G N A T 2 W H Y _ C O U N T E R _ E X A M P L E S           --
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

with GNATCOLL.JSON; use GNATCOLL.JSON;
with Types;         use Types;
with VC_Kinds;      use VC_Kinds;

package CE_Display is

   function Create_Pretty_Cntexmp
     (Cntexmp : Cntexample_File_Maps.Map;
      VC_Loc  : Source_Ptr)
      return Cntexample_File_Maps.Map;
   --  Create pretty printed counterexample.
   --  Note that deep copy of Cntexmp is made and thus the content of
   --  Cntexmp is not impacted by pretty printing.
   --  @param Cntexmp the counterexample that is pretty printed
   --  @param VC_Loc the location of the construct that triggers VC
   --  @return pretty printed counterexample.

   function Get_Cntexmp_One_Liner
     (Cntexmp : Cntexample_File_Maps.Map;
      VC_Loc  : Source_Ptr)
      return String;
   --  Get the part of the counterexample corresponding to the location of
   --  the construct that triggers VC.

   function Get_Environment_CE
     (N    : Node_Id;
      K    : VC_Kind;
      Subp : Node_Id)
      return Cntexample_File_Maps.Map;
   --  Get the elements of the evaluation environment used in the construct
   --  that triggers a VC, with N the AST node corresponding to that construct
   --  and Subp the enclosing subprogram.

   function JSON_Get_Opt
     (Val        : JSON_Value;
      Field      : String;
      Opt_Result : JSON_Value)
      return JSON_Value
   is
     (if Has_Field (Val, Field) then Get (Val, Field)
      else Opt_Result);

   function Remap_VC_Info
     (Cntexmp : Cntexample_File_Maps.Map;
      VC_Loc  : Source_Ptr)
      return Cntexample_File_Maps.Map;
   --  Map counterexample information related to the current VC to the
   --  location of the check in the Ada file.
   --  In Cntexmp, this information is mapped to the field "vc_line" of the
   --  JSON object representing the file where the construct is located.

end CE_Display;
