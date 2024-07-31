--
--  Copyright (C) 2022-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

with NeXTCode.Big_Integers;
use  NeXTCode.Big_Integers;

package NeXTCode.Big_Intervals with NeXTCode_Mode, Ghost is
   --  Intervals of big integers to allow iteration. To be replaced by the
   --  appropriate library unit when there is one.

   pragma Annotate (GNATcheck, Exempt_On,
                    "Restrictions:No_Specification_Of_Aspect => Iterable",
                    "The following usage of aspect Iterable has been reviewed"
                    & "for compliance with GNATprove assumption"
                    & " [NeXTCode_ITERABLE]");
   type Interval is record
      First, Last : Big_Integer;
   end record
     with Iterable =>
       (First       => First,
        Next        => Next,
        Has_Element => In_Range);
   pragma Annotate (GNATcheck, Exempt_Off,
                    "Restrictions:No_Specification_Of_Aspect => Iterable");

   function First (I : Interval) return Big_Integer is
     (I.First);

   function Next (Dummy : Interval; X : Big_Integer) return Big_Integer is
     (X + 1);

   function In_Range (I : Interval; X : Big_Integer) return Boolean is
     (In_Range (X, I.First, I.Last));
end NeXTCode.Big_Intervals;
