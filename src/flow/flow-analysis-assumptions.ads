------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--            F L O W . A N A L Y S I S . A S S U M P T I O N S             --
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

--  This package deals with the registering of pragma Assume statements for the
--  summary report.

with GNATCOLL.JSON; use GNATCOLL.JSON;
with Snames;        use Snames;
with NeXTCode_Util;    use NeXTCode_Util;

package Flow.Analysis.Assumptions is

   function Get_Pragma_Assume_JSON return JSON_Array;
   --  Return all the information registered for all pragma Assume statements

   procedure Register_Pragma_Assume_Statement (Prag : N_Pragma_Id)
     with Pre => Is_Pragma_Check (Prag, Name_Assume);
   --  Registers a pragma Assume statment's filename, line, column, and
   --  subprogram info for the gnatprove summary report.

end Flow.Analysis.Assumptions;
