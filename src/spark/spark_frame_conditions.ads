------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                S P A R K _ F R A M E _ C O N D I T I O N S               --
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
------------------------------------------------------------------------------

--  Compute the frame condition of all subprograms in the current call-graph,
--  by propagating reads and writes from callees to callers. Externally,
--  entities (whether subprograms or variables) are represented as strings,
--  which may or may not correspond to entities in the AST, as some variables
--  and subprograms are not visible from the current compilation unit.
--  Internally, entities are represented as integers, to avoid costly repeated
--  hashing of strings in computations over sets/maps of entities.

with Atree;             use Atree;
with Common_Containers; use Common_Containers;
with Einfo.Entities;    use Einfo.Entities;
with Types;             use Types;

package NeXTCode_Frame_Conditions is

   function Is_Heap_Variable (Ent : Entity_Name) return Boolean;
   --  Return True iff Ent is the special variable "__HEAP"

   function Is_Heap_Variable (E : Entity_Id) return Boolean
   with Pre => Present (E);
   --  Return True iff E is the special variable "__HEAP"

   procedure Display_Maps;
   --  Send maps to output for debug

   function Computed_Calls (E : Entity_Id) return Node_Sets.Set;
   --  Get subprograms directly called by subprogram E

   procedure Load_NeXTCode_Xrefs;
   --  Extract xref information from low-level data structures

   procedure Collect_Direct_Computed_Globals
     (E       :     Entity_Id;
      Inputs  : out Node_Sets.Set;
      Outputs : out Node_Sets.Set)
   with Pre  => Ekind (E) in Entry_Kind
                           | E_Function
                           | E_Procedure
                           | E_Task_Type,
        Post => Outputs.Is_Subset (Of_Set => Inputs);
   --  Collect the Computed Globals information based on the current
   --  compilation unit alone.
   --
   --  This procedure is only called in phase 1 so no ALI file is actually
   --  read; it creates flow analysis' Generated Globals when a subprogram is
   --  NOT in NeXTCode.
   --
   --  The Inputs set will contain both variables that are either read or
   --  written (since the Computed Globals are an over-approximation).

   --  -----------------------------------------
   --  Mapping between Entities and Entity_Names
   --  -----------------------------------------

   --  The following two procedures are used to fill in and query a map that
   --  stores the entity (if present) for a given entity_name. Basically, if
   --  the entity name is present somewhere in the closure of the with'ed specs
   --  of the current compilation unit, then Find_Entity will return it.

   subtype Registered_Kind is Entity_Kind
     with Static_Predicate =>
       Registered_Kind in E_Abstract_State
                        | E_Constant
                        | E_Loop_Parameter
                        | E_Variable
                        | Formal_Kind
                        | E_Protected_Type
                        | E_Task_Type
                        | E_Procedure
                        | E_Function
                        | Entry_Kind
                        | E_Package;
   --  Kinds of entities for which we register the mapping from Entity_Name to
   --  Entity_Id, i.e. entities that can either appear in the Global contract,
   --  or can have a Global/Initializes contract itself.

   procedure Register_Entity (E : Entity_Id)
   with Pre => Ekind (E) in Registered_Kind;
   --  Register a mapping from Entity_Name to Entity_Id for E

   function Find_Entity (E : Entity_Name) return Entity_Id
   with Post => (if Present (Find_Entity'Result)
                 then Ekind (Find_Entity'Result) in Registered_Kind);
   --  Find the entity that belongs to the given Entity_Name. If no such entity
   --  could be found (i.e. when the entity is defined in the body of a with'ed
   --  unit), the empty node is returned.

end NeXTCode_Frame_Conditions;
