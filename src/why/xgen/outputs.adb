------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                              O U T P U T S                               --
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

package body Outputs is

   procedure New_Line (F : File_Type);
   procedure Put (F : File_Type; S : String);
   --  Local version of New_Line and Put ensures Unix style line endings

   --------------
   -- New_Line --
   --------------

   procedure New_Line (F : File_Type) is
   begin
      Character'Write (Stream (F), ASCII.LF);
   end New_Line;

   ---------
   -- Put --
   ---------

   procedure Put (F : File_Type; S : String) is
   begin
      String'Write (Stream (F), S);
   end Put;

   procedure I (O : in out Output_Record);
   --  If a new line has just been created, print as many spaces
   --  as the indentation level requires.

   --------------------
   -- Adjust_Columns --
   --------------------

   procedure Adjust_Columns
     (O        : in out Output_Record;
      Name_Len : Positive;
      Max_Len  : Positive) is
   begin
      for J in Name_Len .. Max_Len loop
         P (O, " ");
      end loop;
   end Adjust_Columns;

   ------------------
   -- Close_Output --
   ------------------

   procedure Close_Output (O : in out Output_Record) is
   begin
      Close (O.File);
      O.Indent := 0;
      O.New_Line := False;
   end Close_Output;

   -------
   -- I --
   -------

   procedure I (O : in out Output_Record) is
   begin
      if O.New_Line then
         for J in 1 .. O.Indent loop
            Put (O.File, " ");
         end loop;
         O.New_Line := False;
      end if;
   end I;

   --------
   -- NL --
   --------

   procedure NL (O : in out Output_Record) is
   begin
      New_Line (O.File);
      O.New_Line := True;
   end NL;

   -----------------
   -- Open_Output --
   -----------------

   procedure Open_Output (O : in out Output_Record; Filename : String) is
   begin
      Create (O.File, Out_File, Filename);
      O.Indent := 0;
      O.New_Line := False;
   end Open_Output;

   -------
   -- P --
   -------

   procedure P (O : in out Output_Record; S : String) is
   begin
      I (O);
      Put (O.File, S);
   end P;

   --------
   -- PL --
   --------

   procedure PL (O : in out Output_Record; S : String) is
   begin
      I (O);
      Put (O.File, S);
      New_Line (O.File);
      O.New_Line := True;
   end PL;

   ---------------
   -- Print_Box --
   ---------------

   procedure Print_Box
     (O               : in out Output_Record;
      Subprogram_Name : String)
   is
      procedure Print_Line;

      procedure Print_Line is
      begin
         P (O, "---");
         for J in Subprogram_Name'Range loop
            P (O, "-");
         end loop;
         PL (O, "---");
      end Print_Line;

   begin
      Print_Line;
      PL (O, "-- " & Subprogram_Name & " --");
      Print_Line;
   end Print_Box;

   ---------------------
   -- Relative_Indent --
   ---------------------

   procedure Relative_Indent
     (O    : in out Output_Record;
      Diff : Integer) is
   begin
      O.Indent := Natural (O.Indent + Diff);
   end Relative_Indent;

   ---------------------
   -- Absolute_Indent --
   ---------------------

   procedure Absolute_Indent
     (O     : in out Output_Record;
      Level : Natural) is
   begin
      O.Indent := Level;
   end Absolute_Indent;

end Outputs;
