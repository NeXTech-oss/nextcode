------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                 G N A T 2 W H Y _ O P T S . W R I T I N G                --
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

package Gnat2Why_Opts.Writing is

   Name_GNATprove : constant String := "gnatprove";
   --  Name of the object sub-directory in which files are generated by
   --  GNATprove.

   function Pass_Extra_Options_To_Gnat2why
     (Translation_Phase : Boolean;
      Obj_Dir           : String) return String;
   --  Create a file with extra options for gnat2why and return its pathname.
   --  Translation_Phase is False for globals generation, and True for
   --  translation to Why.

end Gnat2Why_Opts.Writing;