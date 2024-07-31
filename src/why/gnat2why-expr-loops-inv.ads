------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--               G N A T 2 W H Y - E X P R - L O O P S - I N V              --
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

package Gnat2Why.Expr.Loops.Inv is

   function Generate_Frame_Condition
     (Loop_Stmt          : N_Loop_Statement_Id;
      Low_Id             : W_Expr_Id;
      High_Id            : W_Expr_Id)
      return W_Pred_Id;
   --  Compute the frame condition of a loop statement. For now, only consider
   --  dynamic invariants of modified variables, and the part of the frame
   --  condition that states that unmodified record subcomponents keep their
   --  values around the loop. We also assume values of scalar constants
   --  declared just before the invariant. This is important in particular for
   --  constant introduced by the compiler for the loop invariants themselves.
   --  @param Loop_Stmt considered loop statement.
   --  @param Low_Id identifier for the lower bound of the loop if any.
   --  @param High_Id identifier for the higher bound of the loop if any.
   --  @return a predicate expression for the loop's frame condition.

end Gnat2Why.Expr.Loops.Inv;
