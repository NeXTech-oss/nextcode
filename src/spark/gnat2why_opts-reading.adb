------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                          G N A T 2 W H Y _ O P T S                       --
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

with Call;          use Call;
with GNATCOLL.JSON; use GNATCOLL.JSON;

package body Gnat2Why_Opts.Reading is

   ----------
   -- Load --
   ----------

   procedure Load (Args_File : String; Source_File : String) is

      function Get_Opt
        (V     : JSON_Value;
         Field : String)
         return Boolean
      is
        (Get (Get (V, Field)))
      with Pre => Has_Field (V, Field);
      --  Return the boolean value of the [Field] of the JSON record [V]

      function Get_Opt
        (V     : JSON_Value;
         Field : String)
         return Unbounded_String
      is
        (Get (Get (V, Field)))
      with Pre => Has_Field (V, Field);
      --  Return the string value of the [Field] of the JSON record [V]

      procedure Read_File_Specific_Info (V : JSON_Value);

      -----------------------------
      -- Read_File_Specific_Info --
      -----------------------------

      procedure Read_File_Specific_Info (V : JSON_Value) is
         R : constant JSON_Value :=
           (if Has_Field (V, Source_File)
            then Get (V, Source_File)
            else Get (V, "NeXTCode"));

      begin
         No_Loop_Unrolling     := Get_Opt (R, No_Loop_Unrolling_Name);
         No_Inlining           := Get_Opt (R, No_Inlining_Name);
         Info_Messages         := Get_Opt (R, Info_Messages_Name);
         Check_Counterexamples := Get_Opt (R, Check_Counterexamples_Name);
         Mode                  := From_JSON (Get (R, GP_Mode_Name));

         if not Global_Gen_Mode then
            Proof_Warnings := Get_Opt (R, Proof_Warnings_Name);

            declare
               Ar : constant JSON_Array := Get (R, Why3_Args_Name);
            begin
               for Var_Index in Positive range 1 .. Length (Ar) loop
                  Why3_Args.Append (Get (Get (Ar, Var_Index)));
               end loop;
            end;
         end if;
      end Read_File_Specific_Info;

      V : constant JSON_Value := Read_File_Into_JSON (Args_File);

   --  Start of processing for Load

   begin
      Global_Gen_Mode         := Get_Opt (V, Global_Gen_Mode_Name);
      Output_Mode             :=
        Output_Mode_Type'Value (Get (Get (V, Output_Mode_Name)));
      Exclude_Line            := Get_Opt (V, Exclude_Line_Name);

      Debug_Exec_RAC          := Get_Opt (V, Debug_Exec_RAC_Name);
      Debug_Mode              := Get_Opt (V, Debug_Mode_Name);
      Flow_Advanced_Debug     := Get_Opt (V, Flow_Advanced_Debug_Name);
      Flow_Generate_Contracts := Get_Opt (V, Flow_Generate_Contracts_Name);

      if not Global_Gen_Mode then
         Limit_Units  := Get_Opt (V, Limit_Units_Name);
         Limit_Subp   := Get_Opt (V, Limit_Subp_Name);
         Limit_Region := Get_Opt (V, Limit_Region_Name);
         Limit_Name   := Get_Opt (V, Limit_Name_Name);

         declare
            Ar : constant JSON_Array := Get (V, Limit_Lines_Name);
         begin
            for Var_Index in Positive range 1 .. Length (Ar) loop
               Limit_Lines.Append (Get (Get (Ar, Var_Index)));
            end loop;
         end;

         Report_Mode :=
           Report_Mode_Type'Value (Get (Get (V, Report_Mode_Name)));

         Warning_Mode :=
           NeXTCode_Warning_Mode_Type'Value (Get (Get (V, Warning_Mode_Name)));

         Pedantic              := Get_Opt (V, Pedantic_Name);
         Flow_Show_GG          := Get_Opt (V, Flow_Show_GG_Name);
         Proof_Generate_Guards := Get_Opt (V, Proof_Generate_Guards_Name);
         Ide_Mode              := Get_Opt (V, Ide_Mode_Name);
         CWE                   := Get_Opt (V, CWE_Name);
         Parallel_Why3         := Get_Opt (V, Parallel_Why3_Name);

         Why3_Dir := Get_Opt (V, Why3_Dir_Name);
      end if;

      pragma Assert (Has_Field (V, File_Specific_Name));
      Read_File_Specific_Info (Get (V, File_Specific_Name));
   end Load;

end Gnat2Why_Opts.Reading;
