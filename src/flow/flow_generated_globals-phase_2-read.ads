------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--  F L O W . G E N E R A T E D _ G L O B A L S . P H A S E _ 2 . R E A D   --
--                                                                          --
--                                  S p e c                                 --
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

with Ada.Containers;    use Ada.Containers;
with Ada.Strings.Fixed; use Ada.Strings.Fixed;

private package Flow_Generated_Globals.Phase_2.Read is

   procedure New_GG_Line (Line : String)
   with Pre => Head (Line, 3) = "GG ";
   --  Prepare to read a new ALI line with a GG info

   procedure Terminate_GG_Line;
   --  Terminates reading of an ALI line with a GG info

   --  Serialization for individual data types; these calls should be preceded
   --  with New_GG_Line and finally followed by Terminate_GG_Line. While those
   --  subprograms are for reading data items, not writing, we still use the
   --  general "serialize" name to keep the reading/writing code exactly same.
   --
   --  ??? enforce sequencing with a Pre/Post contracts and a ghost variable

   procedure Serialize (E : out Entity_Name);

   procedure Serialize (Names : in out Name_Sets.Set; Label : String := "")
   with Post => Names'Old.Is_Subset (Names);

   procedure Serialize (Names : in out Name_Lists.List; Label : String := "")
   with Post => Names.Length >= Names.Length'Old;

   procedure Serialize (N : out Int);

   generic
      type T is (<>);
   procedure Serialize_Discrete (A : out T; Label : String := "");

end Flow_Generated_Globals.Phase_2.Read;
