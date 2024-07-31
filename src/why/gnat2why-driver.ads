------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                      G N A T 2 W H Y - D R I V E R                       --
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

--  Main driver for the Ada to Why3 translation. It is called from gnat1.

with Types; use Types;

package Gnat2Why.Driver is

   procedure GNAT_To_Why (GNAT_Root : Node_Id);
   --  Translates an entire GNAT tree for a compilation unit into a set of Why
   --  source files. This callback is called from Gnat1drv.

   function Is_Back_End_Switch (Switch : String) return Boolean;
   --  Returns True if and only if Switch denotes a back-end switch. This is
   --  only used to ignore switches for now.

end Gnat2Why.Driver;
