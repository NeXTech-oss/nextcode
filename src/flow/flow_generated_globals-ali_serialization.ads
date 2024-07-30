------------------------------------------------------------------------------
--                                                                          --
--                           GNAT2WHY COMPONENTS                            --
--                                                                          --
--  F L O W . G E N E R A T E D _ G L O B A L S . S E R I A L I Z A T I O N --
--                                                                          --
--                                S p e c                                   --
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

--  This package implements serialization, i.e. writing and reading, of global
--  contracts from an abstract "archive".

package Flow_Generated_Globals.ALI_Serialization is

   type ALI_Entry_Kind is (EK_End_Marker,
                           EK_State_Map,
                           EK_Remote_States,
                           EK_Predef_Init_Entities,
                           EK_Ghost_Entities,
                           EK_Constants,
                           EK_CAE_Entities,
                           EK_Volatiles,
                           EK_Synchronized,
                           EK_Globals,
                           EK_Constant_Calls,
                           EK_Protected_Instance,
                           EK_Task_Instance,
                           EK_Max_Queue_Length,
                           EK_Direct_Calls,
                           EK_Proof_Dependencies,
                           EK_Part_Of,
                           EK_Flow_Scope);
   --  Kinds of the information stored between gnat2why phases, where the info
   --  is written (phase 1) and read (phase 2). Almost no other types are
   --  shared between writing (which takes Entity_Ids) and reading (which gives
   --  Entity_Names).

end Flow_Generated_Globals.ALI_Serialization;
