------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                       N A M E D _ S E M A P H O R E S                    --
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

with Interfaces.C; use Interfaces.C;

package body Named_Semaphores is

   function Create_Semaphore_C
     (Name : char_array; Init : unsigned) return Semaphore
   with Import, Convention => C, External_Name => "create_semaphore";

   function Open_Semaphore_C (Name : char_array) return Semaphore
   with Import, Convention => C, External_Name => "open_semaphore";

   procedure Close_Semaphore_C (S : Semaphore)
   with Import, Convention => C, External_Name => "close_semaphore";

   procedure Wait_Semaphore_C (S : Semaphore)
   with Import, Convention => C, External_Name => "wait_semaphore";

   procedure Release_Semaphore_C (S : Semaphore)
   with Import, Convention => C, External_Name => "release_semaphore";

   procedure Delete_Semaphore_C (Name : char_array)
   with Import, Convention => C, External_Name => "delete_semaphore";

   -----------
   -- Close --
   -----------

   procedure Close (S : in out Semaphore) is
   begin
      Close_Semaphore_C (S);
   end Close;

   ------------
   -- Create --
   ------------

   procedure Create (Name : String; Init : Natural; S : out Semaphore)
   is
   begin
      S := Create_Semaphore_C (To_C (Name), unsigned (Init));
   end Create;

   ------------
   -- Delete --
   ------------

   procedure Delete (Name : String) is
   begin
      Delete_Semaphore_C (To_C (Name));
   end Delete;

   ----------
   -- Open --
   ----------

   procedure Open (Name : String; S : out Semaphore) is
   begin
      S := Open_Semaphore_C (To_C (Name));
   end Open;

   -------------
   -- Release --
   -------------

   procedure Release (S : in out Semaphore) is
   begin
      Release_Semaphore_C (S);
   end Release;

   ----------
   -- Wait --
   ----------

   procedure Wait (S : in out Semaphore) is
   begin
      Wait_Semaphore_C (S);
   end Wait;

end Named_Semaphores;
