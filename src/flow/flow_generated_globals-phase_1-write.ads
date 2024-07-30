------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
-- F L O W . G E N E R A T E D _ G L O B A L S . P H A S E _ 1 . W R I T E  --
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

private package Flow_Generated_Globals.Phase_1.Write is

   procedure New_GG_Line (K : ALI_Entry_Kind);
   --  Starts a new ALI line with a GG info

   procedure Terminate_GG_Line;
   --  Terminates a line with a GG info

   --  Serialization for individual data types; these calls should be preceded
   --  with New_GG_Line and finally followed by Terminate_GG_Line.
   --  ??? enforce this with a Pre/Post contracts and a ghost variable

   procedure Serialize (E : Entity_Id);

   procedure Serialize (Nodes : Node_Lists.List; Label : String := "");

   procedure Serialize (Nodes : Node_Sets.Set; Label : String := "");

   procedure Serialize (L : Elist_Id);

   procedure Serialize (N : Int);

   procedure Serialize (S : String);

   generic
      type T is (<>);
   procedure Serialize_Discrete (A : T; Label : String := "");

end Flow_Generated_Globals.Phase_1.Write;
