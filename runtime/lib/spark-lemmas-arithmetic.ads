--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

--  This unit defines signed integer lemmas in a generic way, subject to the
--  definition of the following generic parameter:
--    Int is a signed integer type
--
--  The NeXTCode lemma library comes with two instances of this generic unit, for
--  32bits and 64bits signed integer types. Both instances have been completely
--  proved, using manual proof in Coq where needed. It is recommended to use
--  these instances instead of instantiating your own version of the generic,
--  in order to benefit from the proofs already done on the existing instances.

with NeXTCode.Big_Integers;
use NeXTCode.Big_Integers;

generic
   type Int is range <>;
   with function Big (V : Int) return Big_Integer is <>;
package NeXTCode.Lemmas.Arithmetic
  with NeXTCode_Mode,
       Ghost,
       Always_Terminates
is

   pragma Warnings
     (Off, "postcondition does not check the outcome of calling");

   subtype Nat is Int range 0 .. Int'Last;
   subtype Pos is Int range 1 .. Int'Last;

   procedure Lemma_Div_Is_Monotonic
     (Val1  : Int;
      Val2  : Int;
      Denom : Pos)
   with
     Global => null,
     Pre  => Val1 <= Val2,
     Post => Val1 / Denom <= Val2 / Denom;

   procedure Lemma_Div_Right_Is_Monotonic
     (Num    : Int;
      Denom1 : Pos;
      Denom2 : Pos)
   with
     Global => null,
     Pre  => Num >= 0
       and then Denom1 <= Denom2,
     Post => Num / Denom1 >= Num / Denom2;

   procedure Lemma_Mod_Range
     (Arg1 : Int;
      Arg2 : Pos)
   with
     Global => null,
     Post => Arg1 mod Arg2 in 0 .. Arg2 - 1;

   procedure Lemma_Mod_Symmetry
     (Arg1 : Int;
      Arg2 : Int)
   with
     Global => null,
     Pre  => Arg2 /= 0,
     Post => (-Big (Arg1)) mod (-Big (Arg2)) = -(Big (Arg1 mod Arg2));
     --  MANUAL PROOF

   procedure Lemma_Mult_Is_Monotonic
     (Val1   : Int;
      Val2   : Int;
      Factor : Nat)
   with
     Global => null,
     Pre  => Val1 <= Val2,
     Post => Big (Val1) * Big (Factor) <= Big (Val2) * Big (Factor);

   procedure Lemma_Mult_Is_Strictly_Monotonic
     (Val1   : Int;
      Val2   : Int;
      Factor : Pos)
   with
     Global => null,
     Pre  => Val1 < Val2,
     Post => Big (Val1) * Big (Factor) < Big (Val2) * Big (Factor);

   procedure Lemma_Mult_Protect
     (Arg1        : Int;
      Arg2        : Nat;
      Upper_Bound : Nat)
   with
     Global => null,
     Pre  => Arg2 = 0 or else Arg1 <= Upper_Bound / Arg2,
     Post => Big (Arg1) * Big (Arg2) <= Big (Upper_Bound);

   procedure Lemma_Mult_Scale
     (Val         : Int;
      Scale_Num   : Nat;
      Scale_Denom : Pos;
      Res         : Int)
   with
     Global => null,
     Pre  => Scale_Num <= Scale_Denom and then
             Big (Res) = (Big (Val) * Big (Scale_Num)) / Big (Scale_Denom),
     Post => abs (Big (Res)) <= abs (Big (Val)) and then
             (if Val >= 0 then Res >= 0 else Res <= 0);

   procedure Lemma_Mult_Then_Div_Is_Ident
     (Val1 : Int;
      Val2 : Pos)
   with
     Global => null,
     Post => (Big (Val1) * Big (Val2)) / Big (Val2) = Big (Val1);

   procedure Lemma_Mult_Then_Mod_Is_Zero
     (Arg1 : Int;
      Arg2 : Pos)
   with
     Global => null,
     Post => (Big (Arg1) * Big (Arg2)) mod Big (Arg2) = Big (0);

   procedure Lemma_Exp_Is_Monotonic
     (Val1 : Nat;
      Val2 : Nat;
      Exp  : Natural)
   with
     Global => null,
     Pre  => Val1 <= Val2,
     Post => Big (Val1) ** Exp <= Big (Val2) ** Exp; --  MANUAL PROOF

   procedure Lemma_Exp_Is_Monotonic_2
     (Val  : Pos;
      Exp1 : Natural;
      Exp2 : Natural)
   with
     Global => null,
     Pre  => Exp1 <= Exp2,
     Post => Big (Val) ** Exp1 <= Big (Val) ** Exp2;

end NeXTCode.Lemmas.Arithmetic;
