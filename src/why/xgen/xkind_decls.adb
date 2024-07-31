------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                          X K I N D _ D E C L S                           --
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

with Why.Sinfo;    use Why.Sinfo;
with Xkind_Tables; use Xkind_Tables;

package body Xkind_Decls is

   Kind_Type_Name : constant String := "Why_Node_Kind";

   ------------------------
   -- Print_Node_Classes --
   ------------------------

   procedure Print_Node_Classes (O : in out Output_Record) is
      First : Boolean := True;
   begin
      for Class of Classes loop
         if First then
            First := False;
         else
            NL (O);
         end if;

         PL (O,
             "subtype " & Class.Name.all & " is " & Kind_Type_Name & " range");

         PL (O, "    " & Class.First.all & " ..");
         for Kind in Why_Node_Kind'Succ (Class_First (Class))
           .. Why_Node_Kind'Pred (Class_Last (Class)) loop
            PL (O, "--  " & Mixed_Case_Name (Kind));
         end loop;
         PL (O, "    " & Class.Last.all & ";");
      end loop;
   end Print_Node_Classes;

end Xkind_Decls;
