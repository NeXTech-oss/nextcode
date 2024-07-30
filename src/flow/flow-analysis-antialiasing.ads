------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--           F L O W . A N A L Y S I S . A N T I A L I A S I N G            --
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

--  This package deals with the detection of aliasing

package Flow.Analysis.Antialiasing is

   type Aliasing_Check_Result is (Impossible,
                                  No_Aliasing,
                                  Possible_Aliasing,
                                  Definite_Aliasing,
                                  Unchecked);
   pragma Ordered (Aliasing_Check_Result);
   --  These statuses represent the computed aliasing status but Unchecked,
   --  which represents the case where a status has not been computed yet.

   procedure Check_Procedure_Call
     (FA : in out Flow_Analysis_Graphs;
      N  : Node_Id)
   with Pre => Nkind (N) in N_Entry_Call_Statement
                          | N_Subprogram_Call;
   --  This procedure looks at an entry/procedure call statement (and at
   --  the call to function with side effects as well) and determines if it
   --  introduces aliasing that matters. For the full ruleset see the NeXTCode RM
   --  6.4.2, but for example aliasing between two immutable 'in' parameters is
   --  OK, but aliasing between two out parameters is not.
   --
   --  This procedure is aware of globals, both computed by gnat2why and
   --  specified by the user. It checks if there is:
   --     * aliasing between any two parameters
   --     * aliasing between a parameter and a global
   --     * potential aliasing between a generated global and abstract state
   --
   --  This procedure stores into a map the procedure call and its computed
   --  aliasing status.

   function Get_Aliasing_Status_For_Proof (N : Node_Id)
                                           return Aliasing_Check_Result
   with Pre => Nkind (N) in N_Entry_Call_Statement
                          | N_Subprogram_Call;
   --  This procedure looks into the map containing information about the
   --  aliasing status for procedure call N (computed by Check_Procedure_Call).
   --  It returns the aliasing status if N has been stored in the map,
   --  Unchecked otherwise (which means that Check_Procedure_Call is not been
   --  called yet).

end Flow.Analysis.Antialiasing;
