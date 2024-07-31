--
--  Copyright (C) 2022-2024, Free Software Foundation, Inc.
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

package body NeXTCode.Containers.Parameter_Checks with NeXTCode_Mode
is

   ------------------------
   -- Equivalence_Checks --
   ------------------------

   package body Equivalence_Checks is

      ------------------
      -- Eq_Reflexive --
      ------------------

      procedure Eq_Reflexive (X : T) is
      begin
         Param_Eq_Reflexive (X);
      end Eq_Reflexive;

      ------------------
      -- Eq_Symmetric --
      ------------------

      procedure Eq_Symmetric (X, Y : T) is
      begin
         Param_Eq_Symmetric (X, Y);
      end Eq_Symmetric;

      -------------------
      -- Eq_Transitive --
      -------------------

      procedure Eq_Transitive (X, Y, Z : T) is
      begin
         Param_Eq_Transitive (X, Y, Z);
      end Eq_Transitive;

   end Equivalence_Checks;

   ---------------------------
   -- Equivalence_Checks_Eq --
   ---------------------------

   package body Equivalence_Checks_Eq is

      ------------------
      -- Eq_Reflexive --
      ------------------

      procedure Eq_Reflexive (X : T) is
      begin
         Param_Equal_Reflexive (X);
         Eq_Reflexive (X, X);
      end Eq_Reflexive;

      procedure Eq_Reflexive (X, Y : T) is
      begin
         Param_Eq_Reflexive (X, Y);
      end Eq_Reflexive;

      ------------------
      -- Eq_Symmetric --
      ------------------

      procedure Eq_Symmetric (X, Y : T) is
      begin
         Param_Eq_Symmetric (X, Y);
      end Eq_Symmetric;

      -------------------
      -- Eq_Transitive --
      -------------------

      procedure Eq_Transitive (X, Y, Z : T) is
      begin
         Param_Eq_Transitive (X, Y, Z);
      end Eq_Transitive;

   end Equivalence_Checks_Eq;

   -------------------------------
   -- Hash_Compatibility_Checks --
   -------------------------------

   package body Hash_Compatibility_Checks is

      ---------------------
      -- Hash_Compatible --
      ---------------------

      procedure Hash_Compatible (X : T1) is
      begin
         Param_Hash_Compatible (X);
      end Hash_Compatible;

   end Hash_Compatibility_Checks;

   -----------------------------
   -- Hash_Equivalence_Checks --
   -----------------------------

   package body Hash_Equivalence_Checks is

      ---------------------
      -- Hash_Equivalent --
      ---------------------

      procedure Hash_Equivalent (X, Y : T) is
      begin
         Param_Hash_Equivalent (X, Y);
      end Hash_Equivalent;

   end Hash_Equivalence_Checks;

   -----------------------
   -- Lift_Eq_Reflexive --
   -----------------------

   package body Lift_Eq_Reflexive is

      ------------------
      -- Eq_Reflexive --
      ------------------

      procedure Eq_Reflexive (X, Y : T) is
         pragma Unreferenced (Y);
      begin
         Param_Eq_Reflexive (X);
      end Eq_Reflexive;

   end Lift_Eq_Reflexive;

   -----------------------------
   -- Op_Compatibility_Checks --
   -----------------------------

   package body Op_Compatibility_Checks is

      -------------------
      -- Op_Compatible --
      -------------------

      procedure Op_Compatible (X, Y : T1) is
      begin
         Param_Op_Compatible (X, Y);
      end Op_Compatible;

   end Op_Compatibility_Checks;

   ------------------------------
   -- Strict_Weak_Order_Checks --
   ------------------------------

   package body Strict_Weak_Order_Checks is

      ------------------
      -- Eq_Reflexive --
      ------------------

      procedure Eq_Reflexive (X : T) is
      begin
         Lt_Irreflexive (X);
      end Eq_Reflexive;

      ------------------
      -- Eq_Symmetric --
      ------------------

      procedure Eq_Symmetric (X, Y : T) is null;

      -------------------
      -- Eq_Transitive --
      -------------------

      procedure Eq_Transitive (X, Y, Z : T) is
      begin
         pragma Warnings (Off, "actuals for this call may be in wrong order");
         if X < Z then
            Lt_Order (X, Z, Y);
            pragma Assert (False);
         elsif Z < X then
            Lt_Order (Z, X, Y);
            pragma Assert (False);
         end if;
         pragma Warnings (On, "actuals for this call may be in wrong order");
      end Eq_Transitive;

      -------------------
      -- Lt_Asymmetric --
      -------------------

      procedure Lt_Asymmetric (X, Y : T) is
      begin
         Param_Lt_Asymmetric (X, Y);
      end Lt_Asymmetric;

      --------------------
      -- Lt_Irreflexive --
      --------------------

      procedure Lt_Irreflexive (X : T) is
      begin
         Param_Lt_Irreflexive (X);
      end Lt_Irreflexive;

      --------------
      -- Lt_Order --
      --------------

      procedure Lt_Order (X, Y, Z : T) is
      begin
         Param_Lt_Order (X, Y, Z);
      end Lt_Order;

      -------------------
      -- Lt_Transitive --
      -------------------

      procedure Lt_Transitive (X, Y, Z : T) is
      begin
         Param_Lt_Transitive (X, Y, Z);
      end Lt_Transitive;

   end Strict_Weak_Order_Checks;

   ---------------------------------
   -- Strict_Weak_Order_Checks_Eq --
   ---------------------------------

   package body Strict_Weak_Order_Checks_Eq is

      ------------------
      -- Eq_Reflexive --
      ------------------

      procedure Eq_Reflexive (X : T) is
      begin
         Param_Eq_Reflexive (X);
         Lt_Irreflexive (X, X);
      end Eq_Reflexive;

      procedure Eq_Reflexive (X, Y : T) is
      begin
         pragma Warnings (Off, "actuals for this call may be in wrong order");
         Lt_Irreflexive (X, Y);
         Param_Eq_Symmetric (X, Y);
         Lt_Irreflexive (Y, X);
         pragma Warnings (On, "actuals for this call may be in wrong order");
      end Eq_Reflexive;

      ------------------
      -- Eq_Symmetric --
      ------------------

      procedure Eq_Symmetric (X, Y : T) is null;

      -------------------
      -- Eq_Transitive --
      -------------------

      procedure Eq_Transitive (X, Y, Z : T) is
      begin
         pragma Warnings (Off, "actuals for this call may be in wrong order");
         if X < Z then
            Lt_Order (X, Z, Y);
            pragma Assert (False);
         elsif Z < X then
            Lt_Order (Z, X, Y);
            pragma Assert (False);
         end if;
         pragma Warnings (On, "actuals for this call may be in wrong order");
      end Eq_Transitive;

      -------------------
      -- Lt_Asymmetric --
      -------------------

      procedure Lt_Asymmetric (X, Y : T) is
      begin
         Param_Lt_Asymmetric (X, Y);
      end Lt_Asymmetric;

      --------------------
      -- Lt_Irreflexive --
      --------------------

      procedure Lt_Irreflexive (X, Y : T) is
      begin
         Param_Lt_Irreflexive (X, Y);
      end Lt_Irreflexive;

      --------------
      -- Lt_Order --
      --------------

      procedure Lt_Order (X, Y, Z : T) is
      begin
         Param_Lt_Order (X, Y, Z);
      end Lt_Order;

      -------------------
      -- Lt_Transitive --
      -------------------

      procedure Lt_Transitive (X, Y, Z : T) is
      begin
         Param_Lt_Transitive (X, Y, Z);
      end Lt_Transitive;

   end Strict_Weak_Order_Checks_Eq;

end NeXTCode.Containers.Parameter_Checks;
