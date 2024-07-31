------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                           W H Y - I M A G E S                            --
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

with Common_Containers; use Common_Containers;
with GNATCOLL.Symbols;  use GNATCOLL.Symbols;
with Outputs;           use Outputs;
with Types;             use Types;
with Uintp;             use Uintp;
with Urealp;            use Urealp;
with Why.Sinfo;         use Why.Sinfo;
with Why.Types;         use Why.Types;

package Why.Images is

   function Can_Be_Printed_In_Decimal_Notation (N : Nat) return Boolean;
   --  Returns whether number N is a multiple of 2 and 5 only. If this is
   --  the case, that means that a fraction whose denominator is a power of
   --  N can be written exactly in decimal notation. Otherwise, the fraction
   --  may not always be written exactly in decimal notation (e.g. 1/3).

   --  Image functions for the basic entities used in Why's AST.
   --  These output the string image into O.

   procedure P (O : Output_Id; Name : Symbol);

   procedure P (O : Output_Id; Node : Node_Id);

   procedure P (O : Output_Id; Node : Why_Node_Id);

   procedure P (O : Output_Id; Node : Why_Node_Set);

   procedure P (O : Output_Id; Value : Uint);

   procedure P (O : Output_Id; Value : Ureal);

   procedure P (O : Output_Id; Value : Boolean);

   procedure P (O : Output_Id; Value : EW_Type);

   procedure P (O : Output_Id; Value : EW_Assert_Kind);

   procedure P (O : Output_Id; Value : EW_Axiom_Dep_Kind);

   procedure P
     (O      : Output_Id;
      Value  : Source_Ptr;
      Marker : Symbol := No_Symbol);

   procedure P (O : Output_Id; Value : Symbol_Set);

   procedure P (O : Output_Id; Value : String_Sets.Set);

   procedure P (O : Output_Id; Value : EW_Literal);

   procedure P (O : Output_Id; Value : EW_Connector);

   procedure P (O : Output_Id; Value : EW_Domain);

   procedure P (O : Output_Id; Value : EW_Subst_Type);

   procedure P (O : Output_Id; Value : EW_Clone_Type);

   procedure P (O : Output_Id; Value : EW_Theory_Type);
   --  Print either "module" or "theory" depending on theory type

   function Img (Name : Symbol) return String;
   --  Return the String represented by the symbol

   function Img (Node : Why_Node_Set) return String;
   --  Return an image of a Node Id (with no leading space)

   function Img (Node : Node_Id) return String;

   function Img (Ty : EW_Type) return String;

end Why.Images;
