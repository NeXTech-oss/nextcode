------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                        A T R E E - T O _ J S O N                         --
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

with Outputs; use Outputs;

package Why.Atree.To_Json is

   procedure Why_Node_To_Json (O : Output_Id; Node : Why_Node)
     with Pre => Node.Checked;

   procedure Why_Node_Lists_List_To_Json (O : Output_Id;
                                          L : Why_Node_Lists.List);
end Why.Atree.To_Json;
