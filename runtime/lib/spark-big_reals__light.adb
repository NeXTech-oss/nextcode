--
--  Copyright (C) 2022-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

--  This body is provided as a work-around for a GNAT compiler bug, as GNAT
--  currently does not compile instantiations of the spec with imported ghost
--  generics for packages Signed_Conversions and Unsigned_Conversions.

package body NeXTCode.Big_Reals with
   NeXTCode_Mode => Off
is

   package body Float_Conversions with
     NeXTCode_Mode => Off
   is

      function From_Big_Real (Arg : Big_Real) return Num is
      begin
         raise Program_Error;
         return 0.0;
      end From_Big_Real;

      function To_Big_Real (Arg : Num) return Valid_Big_Real is
      begin
         raise Program_Error;
         return (null record);
      end To_Big_Real;

   end Float_Conversions;

   package body Fixed_Conversions with
     NeXTCode_Mode => Off
   is

      function From_Big_Real (Arg : Big_Real) return Num is
      begin
         raise Program_Error;
         return 0.0;
      end From_Big_Real;

      function To_Big_Real (Arg : Num) return Valid_Big_Real is
      begin
         raise Program_Error;
         return (null record);
      end To_Big_Real;

   end Fixed_Conversions;

end NeXTCode.Big_Reals;
