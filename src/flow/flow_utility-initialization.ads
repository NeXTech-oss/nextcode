------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--          F L O W _ U T I L I T Y . I N I T I A L I Z A T I O N           --
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

--  This package contains utilities related to the (default) initialization
--  of types and objects.

with NeXTCode_Definition; use NeXTCode_Definition;

package Flow_Utility.Initialization is

   function Get_Default_Initialization (F : Flow_Id) return Node_Id
   with Pre  => F.Kind in Direct_Mapping | Record_Field,
        Post => (if Present (Get_Default_Initialization'Result)
                 then Nkind (Get_Default_Initialization'Result) in N_Subexpr);
   --  Get the default initialization expression for the given Flow_Id (this
   --  only really works for record fields and direct mappings; magic strings
   --  can only appear as globals and thus are always acting as initialized).
   --  @param F is the Flow_Id whose initialization we look for
   --  @return the default initialization expression of F

   function Is_Default_Initialized
     (F          : Flow_Id;
      Ignore_DIC : Boolean := False)
      return Boolean
   with Pre => F.Kind in Direct_Mapping | Record_Field;
   --  Returns True if F is default initialized.
   --  @param F is the Flow_Id whose initialization we look for
   --  @param Ignore_DIC If True then ignore attribute Has_DIC for this type
   --  @return True iff F is fully default initialized

   --  The following type lists all possible kinds of default initialization
   --  that may apply to a type.

   type Default_Initialization_Kind is
     (No_Possible_Initialization,
      --  A type cannot possibly be initialized because it has no content, for
      --  example - a null record.

      Full_Default_Initialization,
      --  A type that combines the following types and content:
      --    * Access type
      --    * Array-of-scalars with specified Default_Component_Value
      --    * Array type with fully default initialized component type
      --    * Record or protected type with components that either have a
      --      default expression or their related types are fully default
      --      initialized.
      --    * Scalar type with specified Default_Value
      --    * Task type
      --    * Type extension of a type with full default initialization where
      --      the extension components are also fully default initialized.

      Mixed_Initialization,
      --  A type where some of its internals are fully default initialized and
      --  some are not.

      No_Default_Initialization
      --  A type where none of its content is fully default initialized
     );

   function Default_Initialization (Typ        : Entity_Id;
                                    Ignore_DIC : Boolean := False)
                                    return Default_Initialization_Kind
   with Pre => Is_Type (Typ) and then Entity_In_NeXTCode (Typ);
   --  Determine default initialization kind that applies to a particular type.
   --  Private types are treated specially, so that they are either considered
   --  as having full default initialization, or no default initialization.
   --  @param Typ any type
   --  @param Ignore_DIC If True then do not consider attribute Has_DIC for
   --     this type.
   --  @return the Default_Initialization_Kind of Typ

end Flow_Utility.Initialization;
