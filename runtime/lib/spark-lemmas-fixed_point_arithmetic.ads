--
--  Copyright (C) 2018-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

--  This unit defines fixed-point lemmas in a generic way, subject to the
--  definition of the following generic parameter:
--    Fix is an ordinary (non-decimal) fixed-point type
--
--  The user should instantiate the generic with a suitable fixed-point type of
--  interest to obtain corresponding usable lemmas.

generic
   type Fix is delta <>;
package NeXTCode.Lemmas.Fixed_Point_Arithmetic
  with Pure,
       Ghost,
       Always_Terminates
is

   pragma Warnings
     (Off, "postcondition does not check the outcome of calling");

   procedure GNAT_Lemma_Div_Is_Monotonic
     (Num1  : Fix;
      Num2  : Fix;
      Denom : Positive)
   with
     Global => null,
     Pre  => Num1 <= Num2,
     Post => Num1 / Denom <= Num2 / Denom;
   pragma Annotate (GNATprove, Intentional, "postcondition",
                    "GNAT-specific lemma, as Ada RM does not guarantee it");
   --  GNAT implements division of fixed-point type by Integer with integer
   --  division, which is monotonic in its numerator.
   --
   --  As fixed-point values Num1 and Num2 are represented internally by
   --  integers, the fixed-point divisions (Num1 / Denom) and (Num2 / Denom)
   --  are computed as the integer division on their representations. Thus, the
   --  correction of the above lemma rests on the proof of
   --  Lemma_Div_Is_Monotonic from NeXTCode.Arithmetic_Lemmas

   procedure GNAT_Lemma_Div_Right_Is_Monotonic
     (Num    : Fix;
      Denom1 : Positive;
      Denom2 : Positive)
   with
     Global => null,
     Pre  => Num >= 0.0
       and then Denom1 <= Denom2,
     Post => Num / Denom1 >= Num / Denom2;
   pragma Annotate (GNATprove, Intentional, "postcondition",
                    "GNAT-specific lemma, as Ada RM does not guarantee it");
   --  GNAT implements division of fixed-point type by Integer with integer
   --  division, which is monotonic in its denominator, when all arguments
   --  are non-negative.
   --
   --  As fixed-point value Num is represented internally by an integer, the
   --  fixed-point divisions (Num / Denom1) and (Num / Denom2) are computed as
   --  the integer divisions on its representation. Thus, the correction of the
   --  above lemma rests on the proof of Lemma_Div_Right_Is_Monotonic from
   --  NeXTCode.Arithmetic_Lemmas

   procedure GNAT_Lemma_Mult_Then_Div_Is_Ident
     (Val1 : Fix;
      Val2 : Positive)
   with
     Global => null,
     Pre  => Val1 in 0.0 .. Fix'Last / Val2,
     Post => (Val1 * Val2) / Val2 = Val1;
   pragma Annotate (GNATprove, Intentional, "overflow check",
                    "GNAT-specific lemma, as Ada RM does not guarantee it");
   --  GNAT implements division of fixed-point type by Integer with integer
   --  division, which ensures that Fix'Last / Val2 is rounded to zero. Hence
   --  the multiplication (Val1 * Val2) in the postcondition cannot overflow.
   --
   --  As fixed-point values Val1 is represented internally by an integer, the
   --  fixed-point multiplication and division ((Val1 * Val2) / Val2) are
   --  computed as the integer multplication and division on its
   --  representation. Thus, the correction of the above lemma rests on the
   --  proof of Lemma_Mult_Then_Div_Is_Ident from NeXTCode.Arithmetic_Lemmas

end NeXTCode.Lemmas.Fixed_Point_Arithmetic;
