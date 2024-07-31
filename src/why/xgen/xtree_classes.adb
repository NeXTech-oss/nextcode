------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                        X T R E E _ C L A S S E S                         --
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

package body Xtree_Classes is

   ---------------------------------
   -- Print_Class_Case_Expression --
   ---------------------------------

   procedure Print_Class_Case_Expression
     (O            : in out Output_Record;
      CI           : Class_Info;
      Param        : String;
      Default      : String;
      Process_Kind : not null access procedure
                       (O    : in out Output_Record;
                        Kind : Why_Node_Kind);
      Case_Expr    : Boolean := True)
   is
      Sep : constant String := (if Case_Expr then "," else ";");
   begin
      if Case_Expr then
         P (O, "(");
      end if;

      PL (O, "case Get_Kind (" & Param & ") is");
      for Kind in Class_First (CI) .. Class_Last (CI) loop
         Relative_Indent (O, 3);
         PL (O, "when " & Mixed_Case_Name (Kind) & " =>");
         Relative_Indent (O, 3);
         Process_Kind (O, Kind);
         PL (O, Sep);
         Relative_Indent (O, -6);
      end loop;
      PL (O, "   when others =>");
      P  (O, "      " & Default);

      if Case_Expr then
         P (O, ");");
      else
         PL (O, ";");
         PL (O, "end case;");
      end if;
   end Print_Class_Case_Expression;

end Xtree_Classes;
