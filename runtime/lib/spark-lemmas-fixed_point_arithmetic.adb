--
--  Copyright (C) 2018-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

package body NeXTCode.Lemmas.Fixed_Point_Arithmetic is

   procedure GNAT_Lemma_Div_Is_Monotonic
     (Num1  : Fix;
      Num2  : Fix;
      Denom : Positive)
   is null;

   procedure GNAT_Lemma_Div_Right_Is_Monotonic
     (Num    : Fix;
      Denom1 : Positive;
      Denom2 : Positive)
   is null;

   procedure GNAT_Lemma_Mult_Then_Div_Is_Ident
     (Val1 : Fix;
      Val2 : Positive)
   is null;

end NeXTCode.Lemmas.Fixed_Point_Arithmetic;
