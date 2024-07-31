------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                   G N A T 2 W H Y - G E N - T E R M S                    --
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

with Why.Atree.Builders; use Why.Atree.Builders;
with Why.Ids;            use Why.Ids;
with Why.Sinfo;          use Why.Sinfo;
with Why.Types;          use Why.Types;

package Why.Gen.Terms is
   --  Functions that deal with generation of terms

   True_Term  : constant W_Term_Id := New_Literal (Value => EW_True);
   False_Term : constant W_Term_Id := New_Literal (Value => EW_False);

   False_Pred : constant W_Pred_Id := New_Literal (Value => EW_False);
   True_Pred  : constant W_Pred_Id := New_Literal (Value => EW_True);

   function Has_Dereference (W : Why_Node_Id) return Boolean;
   --  Return True if T contains a dereference

   function Has_Dereference_Or_Any_Or_Self (T : W_Term_Id) return Boolean;
   --  Return True if T contains a dereference or "any" constructs, or
   --  reference to "self" object for protected types

   function New_Old (Expr : W_Expr_Id; Domain : EW_Domain) return W_Expr_Id;
   --  Build the expression old Expr

end Why.Gen.Terms;
