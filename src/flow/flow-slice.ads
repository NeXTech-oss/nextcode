------------------------------------------------------------------------------
--                                                                          --
--                           GNAT2WHY COMPONENTS                            --
--                                                                          --
--                           F L O W . S L I C E                            --
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

--  This package deals with computing slices (and thus dependency relations)

package Flow.Slice is

   function Dependency
     (FA      : Flow_Analysis_Graphs;
      V_Final : Flow_Graphs.Vertex_Id)
      return Flow_Id_Sets.Set;
   --  Compute all inputs the given vertex depends on. For IPFA please use the
   --  function IPFA_Dependency, which also includes dependencies on called
   --  subprograms.
   --
   --  Complexity is O(N)

   function IPFA_Dependency
     (FA      : Flow_Analysis_Graphs;
      V_Final : Flow_Graphs.Vertex_Id)
      return Vertex_Sets.Set;
   pragma Unreferenced (IPFA_Dependency);
   --  Compute all inputs the given vertex depends on
   --
   --  Complexity is O(N)

   function Compute_Dependency_Relation
     (FA : Flow_Analysis_Graphs)
      return Dependency_Maps.Map;
   --  Computes the actual dependency relation of the given subprogram
   --
   --  Complexity is O(N^2)

   use type Common_Containers.Node_Sets.Set;  --  for the "or" operator

   procedure Compute_Globals
     (FA                    : Flow_Analysis_Graphs;
      Globals               : out Global_Nodes;
      Proof_Calls           : out Node_Sets.Set;
      Definite_Calls        : out Node_Sets.Set;
      Conditional_Calls     : out Node_Sets.Set;
      Local_Definite_Writes : out Node_Sets.Set;
      Local_Packages        : out Node_Sets.Set)
   with Pre  => FA.Generating_Globals and FA.Is_Generative,
        Post => Definite_Calls.Intersection (Conditional_Calls).Is_Empty
                and then Proof_Calls.Intersection
                           (Definite_Calls or Conditional_Calls).Is_Empty
                and then Local_Definite_Writes.Is_Subset
                           (Of_Set => FA.GG.Local_Variables)
                and then (if Ekind (FA.Spec_Entity) = E_Package then
                            (for all Local of Local_Packages =>
                               Ekind (Local) = E_Package)
                          else
                            Local_Definite_Writes.Is_Empty
                              and then Local_Packages.Is_Empty);
   --  Computes globals (and procedure calls) from the given graphs
   --  ??? this name has nothing to do with "computed globals" (aka Yannick's)
   --
   --  Complexity is O(N)
   --  @param FA are the flow graphs for which we compute globals
   --  @param Globals are global variables read exclusively in proof
   --    contexts, read (except for variables read in
   --    proof contexts) and written (they may overlap with
   --    Inputs but not with Inputs_Proof)
   --  @param Proof_Calls are subprograms called exclusively in proof contexts
   --  @param Definite_Calls are subprograms definitely called
   --  @param Conditional_Calls are subprograms conditionally called
   --  @param Local_Definite_Writes are local variables that are definitely
   --    initialized after package elaboration
   --  @param Local_Packages are immediately nested packages whose generated
   --    Initializes contract will contribute to the generated Initializes
   --    contract of their enclosing package

end Flow.Slice;
