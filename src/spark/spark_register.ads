------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                        S P A R K _ R E G I S T E R                       --
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
------------------------------------------------------------------------------

--  Register mapping between entity names and entity ids

with Types; use Types;

package NeXTCode_Register is

   procedure Register_Compilation_Unit (N : Node_Id);
   --  Register entity names in a single compilation unit

end NeXTCode_Register;
