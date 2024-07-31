--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

with NeXTCode.Lemmas.Unconstrained_Array;

pragma Style_Checks (Off);
package body NeXTCode.Tests.Array_Lemmas
  with NeXTCode_Mode =>
#if NeXTCode_BODY_MODE="On"
  On
#else
  Off
#end if;
is
   pragma Style_Checks (On);
   pragma Warnings
     (Off, "postcondition does not check the outcome of calling");

   package Test_Uint is new NeXTCode.Lemmas.Unconstrained_Array
     (Index_Type => Integer,
      Element_T  => Integer,
      A          => Arr_Int_Unconstrained,
      Less       => "<");

   package Test_Ufloat is new NeXTCode.Lemmas.Unconstrained_Array
     (Index_Type => Integer,
      Element_T  => Float,
      A          => Arr_Float_Unconstrained,
      Less       => "<");

   --  For now, constrained array need a type conversion. In the future,
   --  there will be a constrained array library.

   procedure Test_Transitive_Order_Float (Arr : Arr_Float_Constrained) is
   begin
      Test_Ufloat.Lemma_Transitive_Order
        (Arr_Float_Unconstrained (Arr));
   end Test_Transitive_Order_Float;

   procedure Test_Transitive_Order_Float
     (Arr : Arr_Float_Unconstrained) is
   begin
      Test_Ufloat.Lemma_Transitive_Order (Arr);
   end Test_Transitive_Order_Float;

   procedure Test_Transitive_Order_Int (Arr : Arr_Int_Constrained) is
   begin
      Test_Uint.Lemma_Transitive_Order
        (Arr_Int_Unconstrained (Arr));
   end Test_Transitive_Order_Int;

   procedure Test_Transitive_Order_Int (Arr : Arr_Int_Unconstrained) is
   begin
      Test_Uint.Lemma_Transitive_Order (Arr);
   end Test_Transitive_Order_Int;

end NeXTCode.Tests.Array_Lemmas;
