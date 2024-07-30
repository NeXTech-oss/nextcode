------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--            F L O W . A N A L Y S I S . A S S U M P T I O N S             --
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

with Assumption_Types; use Assumption_Types;
with Flow_Refinement;  use Flow_Refinement;
with Sinput;           use Sinput;

package body Flow.Analysis.Assumptions is

   Pragma_Assume_Msgs : JSON_Array;

   ----------------------------
   -- Get_Pragma_Assume_JSON --
   ----------------------------

   function Get_Pragma_Assume_JSON return JSON_Array is (Pragma_Assume_Msgs);

   --------------------------------------
   -- Register_Pragma_Assume_Statement --
   --------------------------------------

   procedure Register_Pragma_Assume_Statement (Prag : N_Pragma_Id) is
      Result : constant JSON_Value := Create_Object;
      Slc    : constant Source_Ptr := Sloc (Prag);
      File   : constant String     := File_Name (Slc);
      Line   : constant Positive   :=
        Positive (Get_Physical_Line_Number (Slc));
      Col    : constant Positive   :=
        Positive (Get_Column_Number (Slc));
   begin
      Set_Field (Result, "file", File);
      Set_Field (Result, "line", Line);
      Set_Field (Result, "col", Col);
      Set_Field (Result, "entity",
                 To_JSON (Entity_To_Subp_Assumption
                   (Get_Flow_Scope (Prag).Ent)));
      Append (Pragma_Assume_Msgs, Result);
   end Register_Pragma_Assume_Statement;

end Flow.Analysis.Assumptions;
