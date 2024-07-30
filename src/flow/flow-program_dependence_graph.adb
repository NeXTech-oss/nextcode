------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--        F L O W . P R O G R A M _ D E P E N D E N C E _ G R A P H         --
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

package body Flow.Program_Dependence_Graph is

   ------------
   -- Create --
   ------------

   procedure Create
     (FA : in out Flow_Analysis_Graphs) is
   begin
      --  Initialize the PDG with a copy of the vertices of the CFG,
      --  but not the edges
      FA.PDG := FA.CFG.Create;

      --  Edges in the PDG are the union of edges from the CDG, DDG and TDG
      FA.PDG.Copy_Edges (FA.CDG);
      FA.PDG.Copy_Edges (FA.DDG);
      FA.PDG.Copy_Edges (FA.TDG);
   end Create;

end Flow.Program_Dependence_Graph;
