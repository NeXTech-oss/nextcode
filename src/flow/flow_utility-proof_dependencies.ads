------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--      F L O W _ U T I L I T Y . P R O O F _ D E P E N D E N C I E S       --
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

--  This package contains utilities related to proof dependencies.

with Flow_Classwide;
with NeXTCode_Util.Types;          use NeXTCode_Util.Types;

with Namet;                     use Namet;

package Flow_Utility.Proof_Dependencies is

   procedure Process_Access_To_Subprogram_Contracts
     (Typ                : Type_Kind_Id;
      Scop               : Flow_Scope;
      Proof_Dependencies : in out Node_Sets.Set;
      Generating_Globals : Boolean)
   with Pre  => Is_Access_Subprogram_Type (Typ)
                  and then No (Parent_Retysp (Typ)),
       Post =>
         Proof_Dependencies'Old.Is_Subset (Of_Set => Proof_Dependencies);
   --  Fill Proof_Dependencies by analyzing the Pre and Post contracts from
   --  an access-to-subprogram type definition.

   procedure Process_Access_Attribute
     (N                  : Node_Id;
      Proof_Dependencies : in out Node_Sets.Set)
   with Pre  => Nkind (N) in N_Attribute_Reference
                  and then Attribute_Name (N) = Name_Access,
        Post =>
          Proof_Dependencies'Old.Is_Subset (Of_Set => Proof_Dependencies);
   --  Fill Proof_Dependencies when the 'Access attribute references a
   --  subprogram.

   procedure Process_Dispatching_Call
     (N                  : Node_Id;
      Proof_Dependencies : in out Node_Sets.Set)
   with Pre  => Flow_Classwide.Is_Dispatching_Call (N),
        Post =>
          Proof_Dependencies'Old.Is_Subset (Of_Set => Proof_Dependencies);
   --  Fill Proof_Dependencies with all possible callees for dispatching
   --  call N.

   procedure Process_Iterable_For_Proof_Annotation
     (N                  : Node_Id;
      Proof_Dependencies : in out Node_Sets.Set)
   with Pre  => Nkind (N) in N_Iterator_Specification,
        Post =>
          Proof_Dependencies'Old.Is_Subset (Of_Set => Proof_Dependencies);
   --  Fill Proof_Dependencies by analyzing the potential Iterable_For_Proof
   --  annotations associated to N.

   procedure Process_Predicate_And_Invariant
     (N                  : Node_Or_Entity_Id;
      Scop               : Flow_Scope;
      Include_Invariant  : Boolean;
      Proof_Dependencies : in out Node_Sets.Set)
     with Pre  => N in N_Has_Etype_Id,
          Post => Proof_Dependencies'Old.Is_Subset
                    (Of_Set => Proof_Dependencies);
   --  Fill Proof_Dependencies by analyzing predicate and invariant
   --  expressions that apply to the type of N. Include_Invariant is used to
   --  determine whether a type invariant is pulled.

   procedure Process_Reclamation_Functions
     (Typ                : Type_Kind_Id;
      Proof_Dependencies : in out Node_Sets.Set)
   with Post =>
          Proof_Dependencies'Old.Is_Subset (Of_Set => Proof_Dependencies);
   --  Fill Proof_Dependencies with the reclamation functions associated to
   --  all components of Typ.

   function Subprogram_Proof_Dependencies (E : Entity_Id) return Node_Sets.Set
   with Pre  => Ekind (E) in Entry_Kind
                           | E_Function
                           | E_Procedure;
   --  Return proof dependencies that we can extract from the declaration of
   --  a subprogram E, i.e. in its Pre, Post, Contract_Cases, type of formals
   --  and return type when present.

end Flow_Utility.Proof_Dependencies;
