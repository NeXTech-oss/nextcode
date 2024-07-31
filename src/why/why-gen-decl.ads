------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                        W H Y - G E N - D E C L                           --
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

with Why.Ids;               use Why.Ids;
with Why.Gen.Binders;       use Why.Gen.Binders;
with Gnat2Why.Util;         use Gnat2Why.Util;

package Why.Gen.Decl is
   --  This package contains all subprograms that are used to build Why
   --  toplevel declarations.

   function New_Type_Decl (Name : String) return W_Declaration_Id;
   --  @param  Name name of the new type
   --  @return Declaration of an abstract logic type named Name

   function New_Type_Decl
     (Name  : W_Name_Id;
      Alias : W_Type_Id) return W_Declaration_Id;
   --  @param  Name name of the new type
   --  @param  Alias type that we want to copy
   --  @return Declaration of a logic type named Name which is transparently
   --      equal to Alias.

   function New_Havoc_Declaration (Name : W_Name_Id) return W_Declaration_Id;
   --  @param  Name name of the type for which we want a havoc function
   --  @return Definition of a val havocing its only argument of type name__ref

   procedure Emit
     (Th   : Theory_UC;
      Decl : W_Declaration_Id)
   with Pre => not Th.Finished;
   --  Append Decl to the list of declarations from the current theory in S
   --  @param S section of the Why file where the declaration will be emitted
   --  @param Decl declaration to emit

   procedure Emit_Projection_Metas (Th : Theory_UC; Projection_Fun : String);
   --  Emit metas that mark a function as a projection function and disables
   --  inlining of this function in Why3.
   --  @param Section section of the Why file where the declaration will be
   --      emitted.
   --  @param Projection_Fun the name of the function that will be marked as
   --      projection.

   procedure Emit_Record_Declaration
     (Th           : Theory_UC;
      Name         : W_Name_Id;
      Binders      : Binder_Array;
      NeXTCode_Record : Boolean := False);
   --  Emit declaration of a Why3 record type and counterexample projections
   --  for this record type. The projections project values of the record type
   --  to  fields of this type.
   --  @param Theory the theory where the record declaration will be emitted.
   --  @param Name the name of the record type.
   --  @param Binders the fields of the record type.
   --  @param NeXTCode_Record if equal to True, it will be assumed that the
   --      generated record type corresponds to NeXTCode record type. That is, the
   --      Binders correspond to fields of NeXTCode record type.
   --      In that case, generated projections from this record type to fields
   --      of this record type will append the string "." and the NeXTCode source
   --      name of the field to the variable being projected.

   procedure Emit_Ref_Type_Definition
     (Th   : Theory_UC;
      Name : W_Name_Id);
   --  Emit definition of a record type with one mutable field of type Name and
   --  counterexample projection from this type to this field.
   --  For more information about counterexample projections see documentation
   --  of Emit_Record_Declaration.
   --  @param  Theory the theory where the reference type will be emitted
   --  @param  Name name of the type for which we want a reference

end Why.Gen.Decl;
