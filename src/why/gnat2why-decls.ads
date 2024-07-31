------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                         G N A T 2 W H Y - D E C L S                      --
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

with Common_Containers;          use Common_Containers;
with NeXTCode_Atree;                use NeXTCode_Atree;
with NeXTCode_Atree.Entities;       use NeXTCode_Atree.Entities;
with NeXTCode_Definition;           use NeXTCode_Definition;
with NeXTCode_Util;                 use NeXTCode_Util;

package Gnat2Why.Decls is

   procedure Translate_Constant (E : Object_Kind_Id)
   with Pre => Ekind (E) in E_Constant
                          | E_Discriminant
                          | E_In_Parameter
                 or else
               Is_Quantified_Loop_Param (E);
   --  Generate a function declaration for objects that appear as constant
   --  in Why.

   procedure Translate_Constant_Value (E : E_Constant_Id);
   --  Possibly generate an axiom to define the value of the function
   --  previously declared by a call to Translate_Constant for a constant
   --  object.

   procedure Translate_External_Object (E : Entity_Name);
   --  Generate Why declarations for an object E that is only known to proof
   --  as a generated Global of some subprogram.
   --
   --  This object is either a variable which is not in NeXTCode, or an abstract
   --  state which represents its hidden constituents. It is never a constant,
   --  because those in NeXTCode are translated elsewhere and those not in NeXTCode
   --  do not appear in Global for they are considered as without variable
   --  input. Other objects, e.g. parameters or loop variables, can only appear
   --  in Global contract of subprograms nested inside their scopes, but if
   --  such an object is not in NeXTCode then such a nested subprogram is also not
   --  in NeXTCode (and so it ignored in proof).

   procedure Translate_Loop_Entity (E : E_Loop_Id);

   procedure Translate_Variable (E : Object_Kind_Id)
   with Pre => Entity_In_NeXTCode (E);
   --  Generate Why declarations that correspond to an Ada top-level object
   --  declaration.

   procedure Translate_Exceptions;
   --  Generate a Why3 module for all the visible exceptions

end Gnat2Why.Decls;
