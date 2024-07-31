------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                            X K I N D _ I D S                             --
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

with Xkind_Tables; use Xkind_Tables;

package body Xkind_Ids is

   procedure Print_Subtypes (O : in out Output_Record; Kind : Id_Kind);
   --  Same as the three Print_*_Subtypes method, Kind being the
   --  Id Kind of the subtypes to generate.

   -------------------------
   -- Print_Derived_Types --
   -------------------------

   procedure Print_Derived_Types (O : in out Output_Record) is
   begin
      Print_Subtypes (O, Derived);
   end Print_Derived_Types;

   ---------------------------
   -- Print_Opaque_Subtypes --
   ---------------------------

   procedure Print_Opaque_Subtypes (O : in out Output_Record) is
   begin
      Print_Subtypes (O, Opaque);
   end Print_Opaque_Subtypes;

   ----------------------------
   -- Print_Regular_Subtypes --
   ----------------------------

   procedure Print_Regular_Subtypes (O : in out Output_Record) is
   begin
      Print_Subtypes (O, Regular);
   end Print_Regular_Subtypes;

   --------------------
   -- Print_Subtypes --
   --------------------

   procedure Print_Subtypes (O : in out Output_Record; Kind : Id_Kind) is
      use String_Lists;
      use Class_Lists;

      procedure Process_One_Node_Kind (Position : String_Lists.Cursor);
      --  Same as Print_Subtypes, but only for the kind
      --  pointed by Position.

      procedure Process_One_Class_Kind (Position : Class_Lists.Cursor);
      --  Same as Print_Subtypes, but only for the class
      --  pointed by Position.

      procedure Print_Subtypes (Prefix : String);
      --  Print subtypes for a given node kind whose prefix
      --  is passed as parameter.

      ----------------------------
      -- Process_One_Class_Kind --
      ----------------------------

      procedure Process_One_Class_Kind (Position : Class_Lists.Cursor) is
         CI : constant Class_Info := Class_Lists.Element (Position);
      begin
         Print_Subtypes (Class_Name (CI));

         if Position /= Classes.Last then
            NL (O);
         end if;
      end Process_One_Class_Kind;

      ---------------------------
      -- Process_One_Node_Kind --
      ---------------------------

      procedure Process_One_Node_Kind (Position : String_Lists.Cursor) is
         S : constant String_Access := String_Lists.Element (Position);
      begin
         Print_Subtypes (S.all);

         if Position /= Kinds.Last then
            NL (O);
         end if;
      end Process_One_Node_Kind;

      --------------------
      -- Print_Subtypes --
      --------------------

      procedure Print_Subtypes (Prefix : String) is
      begin
         for Multiplicity in Id_Multiplicity'Range loop
            if Kind = Derived and then Multiplicity = Id_One then
               PL (O, "subtype " & Id_Subtype (Prefix, Kind, Multiplicity)
                   & " is");
               PL (O, "  " & Id_Subtype (Prefix, Kind, Id_Lone) & ";");

            elsif Kind = Derived and then Multiplicity = Id_Some then
               PL (O, "subtype " & Id_Subtype (Prefix, Kind, Multiplicity)
                   & " is");
               PL (O, "  " & Id_Subtype (Prefix, Kind, Id_Set) & ";");

            else
               if Kind = Derived then
                  P (O, "type "
                     & Id_Subtype (Prefix, Kind, Multiplicity)
                     & " is new");
               else
                  P (O, "subtype "
                     & Id_Subtype (Prefix, Kind, Multiplicity)
                     & " is");
               end if;

               if Kind = Opaque then
                  P (O, " ");
               else
                  NL (O);
                  P (O, "  ");
               end if;

               P (O, Base_Id_Subtype (Prefix, Kind, Multiplicity));

               if Kind in Unchecked .. Regular then
                  PL (O, " with");
                  PL (O, "Predicate =>");

                  if Kind = Unchecked then
                     PL (O, Kind_Check (Prefix, Multiplicity));
                  else
                     PL (O, Tree_Check (Prefix, Multiplicity));
                  end if;

                  PL (O, "   (" & Id_Subtype (Prefix, Kind, Multiplicity)
                      & ");");
               else
                  PL (O, ";");
               end if;
            end if;

            if Kind in Regular .. Derived and then Multiplicity = Id_One then
               NL (O);
               PL (O, "type " & Arr_Type (Prefix, Kind)
                   & " is array (Positive range <>)");
               PL (O, "  of " & Id_Subtype (Prefix, Kind, Multiplicity)
                   & ";");
            end if;

            if Multiplicity /= Id_Multiplicity'Last then
               NL (O);
            end if;
         end loop;
      end Print_Subtypes;

   --  Start of processing for Print_Subtypes

   begin
      Kinds.Iterate (Process_One_Node_Kind'Access);
      NL (O);
      Classes.Iterate (Process_One_Class_Kind'Access);
   end Print_Subtypes;

   ------------------------------
   -- Print_Unchecked_Subtypes --
   ------------------------------

   procedure Print_Unchecked_Subtypes (O : in out Output_Record) is
   begin
      Print_Subtypes (O, Unchecked);
   end Print_Unchecked_Subtypes;

end Xkind_Ids;
