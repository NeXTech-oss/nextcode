--
--  Copyright (C) 1995-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

--  Stable_Sorting package

--  This package provides a generic stable sorting procedure that is
--  intended for use by the various doubly linked list container generics.
--  If a stable array sorting algorithm with better-than-quadratic worst
--  case execution time is ever needed, then it could also reside here.

with NeXTCode.Containers.Types; use NeXTCode.Containers.Types;

private package NeXTCode.Containers.Stable_Sorting with NeXTCode_Mode => On is
   pragma Pure;

   --  Stable sorting algorithms with N-log-N worst case execution time.

   generic
      type Node_Ref is private; -- access value or array index
      Nil : Node_Ref;
   package List_Descriptors is

      type List_Descriptor is
         record
            First, Last : Node_Ref := Nil;
            Length      : Count_Type := 0;
         end record;

      --  We use a nested generic here so that the inner generic can
      --  refer to the List_Descriptor type.

      generic
         with function Next (N : Node_Ref) return Node_Ref is <>;
         with procedure Set_Next (N : Node_Ref; Next : Node_Ref) is <>;
         with procedure Set_Prev (N : Node_Ref; Prev : Node_Ref) is <>;
         with function "<" (L, R : Node_Ref) return Boolean is <>;

         with procedure Update_Container (List : List_Descriptor) is <>;
      procedure Doubly_Linked_List_Sort (List : List_Descriptor);

   end List_Descriptors;

end NeXTCode.Containers.Stable_Sorting;
