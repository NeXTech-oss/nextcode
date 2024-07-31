--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

with NeXTCode.Lemmas.Unconstrained_Array;

pragma Style_Checks (Off);
package body NeXTCode.Lemmas.Constrained_Array
  with NeXTCode_Mode =>
#if NeXTCode_BODY_MODE="On"
  On
#else
  Off
#end if;
is
   pragma Style_Checks (On);

   type A_Unconstrained is array (Index_Type range <>) of Element_T;

   package Test is new NeXTCode.Lemmas.Unconstrained_Array
     (Index_Type => Index_Type,
      Element_T  => Element_T,
      A          => A_Unconstrained,
      Less       => Less);

   procedure Lemma_Transitive_Order (Arr : A) is
      Arr_T : constant A_Unconstrained := A_Unconstrained (Arr);
   begin
      Test.Lemma_Transitive_Order (Arr_T);
   end Lemma_Transitive_Order;

end NeXTCode.Lemmas.Constrained_Array;
