------------------------------------------------------------------------------
--                                                                          --
--                           GNAT2WHY COMPONENTS                            --
--                                                                          --
--       F L O W . G E N E R A T E D _ G L O B A L S . P H A S E _ 2        --
--                            V I S I B I L I T Y                           --
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

private package Flow_Generated_Globals.Phase_2.Visibility is

   type Name_Scope is record
      Ent  : Any_Entity_Name;
      Part : Any_Declarative_Part;
   end record;
   --  Just like Flow_Scope, but for Entity_Names

   type Name_Info_T is record
      Is_Package      : Boolean;
      Is_Private      : Boolean;

      Parent          : Any_Entity_Name;
      Instance_Parent : Any_Entity_Name;
      Template        : Any_Entity_Name;
      Container       : Name_Scope;
   end record;
   --  A minimal description of a name location within the code hierarchy

   procedure Register_Name_Scope (E : Entity_Name; Info : Name_Info_T);
   --  Add vertices for E to name visibility graph

   procedure Connect_Name_Scopes;
   --  Creates edges in the visibility graph

   ----------------------------------------------------------------------------
   --  Utilities
   ----------------------------------------------------------------------------

   function State_Refinement_Is_Visible
     (State : Entity_Name;
      From  : Name_Scope)
      return Boolean
   with Pre => GG_Is_Abstract_State (State);

   function Part_Of_Is_Visible
     (State : Entity_Name;
      From  : Name_Scope)
      return Boolean
   with Pre => GG_Is_Abstract_State (State);

   function Child_Packages
     (Parent_Package : Entity_Name)
      return Name_Sets.Set;
   --  Returns known child packages of Parent_Package as derived in phase 1, if
   --  any. ??? Add a precondition to ensure the Parent_Package is actually a
   --  package.

end Flow_Generated_Globals.Phase_2.Visibility;
