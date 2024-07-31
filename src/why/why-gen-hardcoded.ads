------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                    W H Y - G E N - H A R D C O D E D                     --
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

with Gnat2Why.Util;        use Gnat2Why.Util;
with NeXTCode_Atree;          use NeXTCode_Atree;
with NeXTCode_Atree.Entities; use NeXTCode_Atree.Entities;
with NeXTCode_Util;           use NeXTCode_Util;
with NeXTCode_Util.Hardcoded; use NeXTCode_Util.Hardcoded;
with Types;                use Types;
with Why.Ids;              use Why.Ids;
with Why.Sinfo;            use Why.Sinfo;

package Why.Gen.Hardcoded is

   procedure Emit_Hardcoded_Type_Declaration (Th : Theory_UC; E : Entity_Id)
   with
     Pre => Is_Type (E) and then Is_Hardcoded_Entity (E);
   --  Emit declaration of a Why3 type whose representative type is
   --  hardcoded.
   --  @param Theory the theory where the declaration will be emitted.
   --  @param Entity corresponding to the type declaration.

   function Hardcoded_Equality_Symbol
     (Typ    : Entity_Id;
      Domain : EW_Domain)
      return W_Identifier_Id
   with
     Pre => Is_Type (Typ) and then Is_Hardcoded_Entity (Typ);
   --  Return the equality symbol for type Typ

   function Is_Hardcoded_Comparison (Subp : Entity_Id) return Boolean;
   --  Return True if Subp is a comparison operator and gains to be translated
   --  in the predicate domain.

   function Is_Hardcoded_Operation (Op          : N_Binary_Op;
                                    Left, Right : Type_Kind_Id)
                                    return Boolean;
   --  Return True if the binary operator is hardcoded.

   function Transform_Hardcoded_Function_Call
     (Subp     : Entity_Id;
      Args     : W_Expr_Array;
      Domain   : EW_Domain;
      Ada_Node : Node_Id)
      return W_Expr_Id
   with
     Pre => Is_Subprogram (Subp) and then Is_Hardcoded_Entity (Subp);
   --  Transform a hardcoded function call

   function Transform_Hardcoded_Literal
     (Call   : Node_Id;
      Domain : EW_Domain) return W_Expr_Id
   with
     Pre => Nkind (Call) = N_Function_Call
       and then Is_Hardcoded_Entity (Get_Called_Entity_For_Proof (Call))
       and then Is_Literal_Function (Get_Called_Entity_For_Proof (Call));
   --  Transform a literal of an hardcoded type in a precise way
   --  whenever possible. If no precise translation was achieved, return
   --  Why_Empty;

   function Transform_Hardcoded_Operation
     (Op                  : N_Binary_Op;
      Lty, Rty, Expr_Type : Type_Kind_Id;
      LT, RT              : W_Expr_Id;
      Domain              : EW_Domain;
      Ada_Node            : Node_Id)
      return W_Expr_Id
     with Pre => Is_Hardcoded_Operation (Op, Lty, Rty);
   --  Transform an operation "LT op RT" if "op" is hardcoded.

end Why.Gen.Hardcoded;
