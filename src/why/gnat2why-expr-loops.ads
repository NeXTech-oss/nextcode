------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                    G N A T 2 W H Y - E X P R - L O O P                   --
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

package Gnat2Why.Expr.Loops is

   function Get_Loop_Invariant
     (Loop_Stmt : N_Loop_Statement_Id)
      return Node_Lists.List;
   --  Return the list of nodes corresponding to loop invariants if any

   function Transform_Exit_Statement
     (Stmt   : N_Exit_Statement_Id;
      Params : Transformation_Params)
      return W_Prog_Id;

   function Transform_Loop_Statement
     (Stmt   : N_Loop_Statement_Id;
      Params : Transformation_Params)
      return W_Prog_Id;

   function Is_In_Loop_Initial_Statements return Boolean with Ghost;
   --  Return True when analyzing the initial statements of a loop

   function Get_Flat_Statement_And_Declaration_List
     (Stmts : List_Id) return Node_Lists.List;
   --  Given a list of statements and declarations Stmts, returns the flattened
   --  list that includes these statements and declarations, and recursively
   --  all inner declarations and statements that appear in block statements
   --  containing a loop invariant.

end Gnat2Why.Expr.Loops;
