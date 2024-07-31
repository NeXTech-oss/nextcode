------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                    G N A T 2 W H Y - A S S U M P T I O N S               --
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
with Ada.Containers;                     use Ada.Containers;
with Ada.Containers.Doubly_Linked_Lists;
with Ada.Containers.Hashed_Sets;
with Assumptions;                        use Assumptions;
with GNATCOLL.JSON;
with Types;                              use Types;

package Gnat2Why.Assumptions is

   type Claim is record
      Kind : Claim_Kind;
      E    : Entity_Id;
   end record;

   function Hash_Claim (C : Claim) return Ada.Containers.Hash_Type;

   ----------------
   -- Hash_Claim --
   ----------------

   function Hash_Claim (C : Claim) return Ada.Containers.Hash_Type is
     (Hash_Type (Claim_Kind'Pos (C.Kind)) + 4 * Hash_Type (C.E));

   package Claim_Sets is new Ada.Containers.Hashed_Sets
     (Element_Type        => Claim,
      Hash                => Hash_Claim,
      Equivalent_Elements => "=",
      "="                 => "=");

   package Claim_Lists is new Ada.Containers.Doubly_Linked_Lists
     (Element_Type => Claim);

   procedure Register_Claim (C : Claim);
   --  This registers that the claim [C] has been established

   procedure Assume_For_Claim
     (C      : Claim;
      Assume : Claim_Lists.List);
   --  Both procedures register that the claim C depends on the assumption(s)
   --  provided. Calling these procedures does not mean that claim C has been
   --  established.

   procedure Register_Assumptions_For_Call (Caller, Callee : Entity_Id);
   --  For the call (caller -> callee), register the assumptions

   procedure Register_Proof_Claims (E : Entity_Id);
   --  For the entity E, all VCs have been proved, emit the appropriate claims

   function Get_Assume_JSON return GNATCOLL.JSON.JSON_Value;
   --  Save assumptions output to file "unit.assum"

end Gnat2Why.Assumptions;
