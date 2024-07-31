--
--  Copyright (C) 2016-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

generic
   type Index_Type is (<>);
   type Element_T is private;
   type A is array (Index_Type range <>) of Element_T;

   --  Function Less should be transitive like the predefined "<" or "<=",
   --  where transitivity is the property that, for all X, Y, Z of type
   --  Element_T:
   --     (if Less(X,Y) and Less(Y,Z) then Less(X,Z))
   --  If this property is not ensured, the lemmas are likely to introduce
   --  inconsistencies.
   with function Less (X, Y : Element_T) return Boolean;

package NeXTCode.Lemmas.Unconstrained_Array
  with NeXTCode_Mode,
       Pure,
       Ghost,
       Always_Terminates
is

   pragma Warnings
     (Off, "postcondition does not check the outcome of calling");

   procedure Lemma_Transitive_Order (Arr : A) with
     Global => null,
     Pre  => (for all I in Arr'Range =>
               (if I /= Arr'First then
                 Less (Arr (Index_Type'Pred (I)), Arr (I)))),
     Post => (for all I in Arr'Range =>
               (for all J in Arr'Range =>
                 (if I < J then Less (Arr (I), Arr (J)))));

end NeXTCode.Lemmas.Unconstrained_Array;
