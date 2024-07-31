--
--  Copyright (C) 2022-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

package body NeXTCode.Cut_Operations with NeXTCode_Mode => Off is

   function By (Consequence, Premise : Boolean) return Boolean is
     (Premise and then Consequence);

   function So (Premise, Consequence : Boolean) return Boolean is
     (Premise and then Consequence);

end NeXTCode.Cut_Operations;
