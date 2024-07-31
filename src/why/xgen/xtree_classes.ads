------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                        X T R E E _ C L A S S E S                         --
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

with Outputs;      use Outputs;
with Xkind_Tables; use Xkind_Tables;
with Why.Sinfo;    use Why.Sinfo;

package Xtree_Classes is

   procedure Print_Class_Case_Expression
     (O            : in out Output_Record;
      CI           : Class_Info;
      Param        : String;
      Default      : String;
      Process_Kind : not null access procedure
                       (O    : in out Output_Record;
                        Kind : Why_Node_Kind);
      Case_Expr    : Boolean := True);
   --  Generate a case statement or a case expression (depending on Case_Expr)
   --  that dispatch over Param's kind, and call Process_Kind for each kind.

end Xtree_Classes;
