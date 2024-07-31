--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma Style_Checks (Off);
package body NeXTCode.Lemmas.Unconstrained_Array
  with NeXTCode_Mode =>
#if NeXTCode_BODY_MODE="On"
  On
#else
  Off
#end if;
is
   pragma Style_Checks (On);

   procedure Lemma_Transitive_Order (Arr : A) is
   begin
      null;
   end Lemma_Transitive_Order;

end NeXTCode.Lemmas.Unconstrained_Array;
