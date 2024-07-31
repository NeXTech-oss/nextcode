------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                                U T I L S                                 --
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

package body Utils is

   ------------------
   -- Strip_Prefix --
   ------------------

   function Strip_Prefix (Name : String) return String is
      Start : Integer := Name'First;
   begin
      for J in Name'Range loop
         if Name (J) = '_' then
            Start := J + 1;
            exit;
         end if;
      end loop;

      return Name (Start .. Name'Last);
   end Strip_Prefix;

   ------------------
   -- Strip_Suffix --
   ------------------

   function Strip_Suffix (Name : String) return String is
      Stop : Integer := Name'Last;
   begin
      for J in reverse Name'Range loop
         if Name (J) = '_' then
            Stop := J - 1;
            exit;
         end if;
      end loop;

      return Name (Name'First .. Stop);
   end Strip_Suffix;

   ------------
   -- Suffix --
   ------------

   function Suffix (Name : String) return String is
      Stop : Integer := Name'Last;
   begin
      for J in reverse Name'Range loop
         if Name (J) = '_' then
            Stop := J;
            exit;
         end if;
      end loop;

      return Name (Stop + 1 .. Name'Last);
   end Suffix;

end Utils;
