--
--  Copyright (C) 2024-2024, Free Software Foundation, Inc.
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--
--  This unit is a wrapper over Interfaces.C.Strings ensuring absence of
--  aliasing and memory safety through ownership. This handling allows for safe
--  allocation and reclamation of memory as well as precise contracts on the
--  content of strings.
--
--  Contracts in this unit are meant for analysis only, not for run-time
--  checking, so that the expected exceptions are raised. This is enforced by
--  setting the assertion policy to Ignore. These preconditions protect from
--  Constraint_Error, Dereference_Error and Update_Error, but not from
--  Storage_Error.

pragma Assertion_Policy (Ignore);

with Interfaces.C;         use Interfaces.C;
with Interfaces.C.Strings;
with System.Parameters;

package NeXTCode.C.Strings with
  NeXTCode_Mode,
  Always_Terminates
is
   pragma Unevaluated_Use_Of_Old (Allow);

   type chars_ptr is private with
     Annotate => (GNATprove, Ownership, "Needs_Reclamation"),
     Annotate => (GNATprove, Predefined_Equality, "Only_Null"),
     Default_Initial_Condition => Is_Null (Chars_Ptr);
   pragma Preelaborable_Initialization (chars_ptr);

   Null_Ptr : constant chars_ptr with
     Annotate => (GNATprove, Ownership, "Reclaimed_Value"),
     Annotate => (GNATprove, Predefined_Equality, "Null_Value");

   function Is_Null (Item : chars_ptr) return Boolean with
     Ghost,
     Post => Is_Null'Result = (Item = Null_Ptr);

   function C_Length_Ghost (Item : char_array) return size_t with
     Ghost,
     Pre  => Is_Nul_Terminated (Item),
     Post => C_Length_Ghost'Result = Interfaces.C.C_Length_Ghost (Item),
     Annotate => (GNATprove, Inline_For_Proof);

   function Is_Nul_Terminated_Ghost (Item : String) return Boolean with
     Ghost,
     Post => Is_Nul_Terminated_Ghost'Result =
         (for some C of Item => C = To_Ada (nul));
   --  Ghost function returns True if Item contains nul, and False otherwise

   function C_Length_Ghost (Item : String) return Natural with
     Ghost,
     Pre  => Is_Nul_Terminated_Ghost (Item),
     Post => C_Length_Ghost'Result <= Item'Last - Item'First
       and then Item (Item'First + C_Length_Ghost'Result) = To_Ada (nul)
       and then (for all J in Item'First .. Item'First + C_Length_Ghost'Result
                   when J /= Item'First + C_Length_Ghost'Result =>
                     Item (J) /= To_Ada (nul));
   --  Ghost function to compute the length of a string up to the first nul
   --  character.

   --  Non NeXTCode primitives, should be used with care

   function To_Chars_Ptr
     (Item      : Interfaces.C.Strings.char_array_access;
      Nul_Check : Boolean := False) return chars_ptr
   with
     NeXTCode_Mode => Off;  --  To_Chars_Ptr'Result is aliased with Item

   function To_Chars_Ptr
     (Item : Interfaces.C.Strings.chars_ptr) return chars_ptr
   with
     NeXTCode_Mode => Off;
   --  To_Chars_Ptr'Result is aliased with Interfaces.C.Strings.C_Memory

   function From_Chars_Ptr
     (Item : chars_ptr) return Interfaces.C.Strings.chars_ptr
   with
     NeXTCode_Mode => Off;  --  Item is aliased with Interfaces.C.Strings.C_Memory

   --  NeXTCode primitives

   function New_Char_Array (Chars : char_array) return chars_ptr with
     Pre    => Chars'Last < size_t'Last or else Chars'First > 0
       or else Is_Nul_Terminated (Chars),
     --  This precondition ensures that Strlen does not wrap-around on the
     --  new array.

     Post   => New_Char_Array'Result /= Null_Ptr
       and then Strlen (New_Char_Array'Result) =
         (if Is_Nul_Terminated (Chars) then C_Length_Ghost (Chars)
          else Chars'Length)
     --  Strlen returns the number of elements before the first occurrence of
     --  nul in Chars & nul.

       and then
         Value (New_Char_Array'Result) (Strlen (New_Char_Array'Result)) = nul
       and then
         (if Strlen (New_Char_Array'Result) > 0
          then
            (for all I in 0 .. Strlen (New_Char_Array'Result) - 1 =>
               Value (New_Char_Array'Result) (I) = Chars (Chars'First + I))),
     --  Value returns the prefix of Chars & nul up to and including the first
     --  occurrence of nul.

     Global => null;

   function New_String (Str : String) return chars_ptr with
     Post   => New_String'Result /= Null_Ptr
       and then Strlen (New_String'Result) =
         (if Is_Nul_Terminated_Ghost (Str) then size_t (C_Length_Ghost (Str))
          else Str'Length)
     --  Strlen returns the number of elements before the first occurrence of
     --  nul in Str & nul.

       and then
         (for all I in 1 .. Natural (Strlen (New_String'Result)) =>
            Value (New_String'Result) (I) = Str (Str'First + (I - 1))),
     --  Value returns the prefix of Str & nul up to but excluding the first
     --  occurrence of nul.

     Global => null;

   procedure Free (Item : in out chars_ptr) with
     Post    => Item = Null_Ptr,
     Global  => null,
     Depends => (Item => null, null => Item);
   --  When deallocation is prohibited (eg: cert runtimes) this routine
   --  will raise Program_Error.

   function Value (Item : chars_ptr) return char_array with
     Pre    => Item /= Null_Ptr,
     Post   => Value'Result'First = 0
       and then Value'Result'Last = Strlen (Item)
       and then Value'Result (Strlen (Item)) = nul
       and then (for all I in 0 .. Strlen (Item) =>
                   (if I < Strlen (Item) then Value'Result (I) /= nul)),
     Global => null;
   --  Value returns the prefix of the value pointed by Item up to and
   --  including the first occurrence of nul.

   function Value
     (Item   : chars_ptr;
      Length : size_t) return char_array
   with
     Pre    => Item /= Null_Ptr and then Length /= 0,
     Post   => Value'Result'First = 0
       and then Value'Result'Last = size_t'Min (Length - 1, Strlen (Item))
       and then (for all I in 0 .. size_t'(Value'Result'Length - 1) =>
                   Value'Result (I) = char_array'(Value (Item)) (I)),
     Global => null;
   --  Value returns the longest prefix of Value (Item) containing at most
   --  Length elements.

   function Value (Item : chars_ptr) return String with
     Pre    => Item /= Null_Ptr
       and then Strlen (Item) <= size_t (Natural'Last),
     Post   => Value'Result'First = 1
       and then Value'Result'Length = Strlen (Item)
       and then (for all I in Value'Result'Range =>
                   Value'Result (I) /= To_Ada (nul))
       and then (for all I in Value'Result'Range =>
                   Value'Result (I) = To_Ada (Value (Item) (size_t (I - 1)))),
     Global => null;
   --  Value returns the prefix of the value pointed by Item up to but
   --  excluding the first occurrence of nul.

   function Value
     (Item   : chars_ptr;
      Length : size_t) return String
   with
     Pre    => Item /= Null_Ptr and then Length /= 0
       and then (Strlen (Item) <= size_t (Natural'Last)
                 or else Length <= size_t (Natural'Last)),
     Post   => Value'Result'First = 1
       and then Value'Result'Length = size_t'Min (Length, Strlen (Item))
       and then (for all I in Value'Result'Range =>
                   Value'Result (I) = To_Ada (Value (Item) (size_t (I - 1))))
       and then
         (if Strlen (Item) <= size_t (Natural'Last)
          then (for all I in Value'Result'Range =>
                      Value'Result (I) = Value (Item) (I))),
     Global => null;
   --  Value returns the longest prefix of Value (Item) containing at most
   --  Length elements.

   function Strlen (Item : chars_ptr) return size_t with
     Pre    => Item /= Null_Ptr,
     Global => null;
   --  Strlen returns the number of elements before the first occurrence of nul
   --  in the value pointed by Item.

   procedure Update
     (Item   : in out chars_ptr;
      Offset : size_t;
      Chars  : char_array;
      Check  : Boolean := True)
   with
     Pre    => Item /= Null_Ptr
       and then (Chars'First /= 0 or else Chars'Last /= size_t'Last)
       and then Chars'Length <= size_t'Last - Offset
       and then Chars'Length + Offset <= Strlen (Item),
     Post   => Item /= Null_Ptr
       and then Strlen (Item) =
         (if Is_Nul_Terminated (Chars)
          then Offset + C_Length_Ghost (Chars)
          else Strlen (Item)'Old)
       --  Strlen returns the number of elements before the first occurrence of
       --  nul in the value pointed by Item, updated starting at position
       --  Offset, using Chars as the data to be copied into the array.

       and then
         (for all I in 0 .. Strlen (Item) =>
            (if Chars'Length > 0
               and then I in Offset .. Offset + Chars'Length - 1
             then char_array'(Value (Item)) (I) =
                 Chars (I - Offset + Chars'First)
             else char_array'(Value (Item)) (I) =
                 char_array'(Value (Item))'Old (I))),
       --  Value returns a prefix of the value pointed by Item, updated
       --  starting at position Offset, using Chars as the data to be copied
       --  into the array.

     Global => null;

   procedure Update
     (Item   : in out chars_ptr;
      Offset : size_t;
      Str    : String;
      Check  : Boolean := True)
   with
     Pre    => Item /= Null_Ptr
       and then Str'Length <= size_t'Last - Offset
       and then Str'Length + Offset <= Strlen (Item),
     Post   => Item /= Null_Ptr
       and then Strlen (Item) =
         (if Is_Nul_Terminated_Ghost (Str)
          then Offset + size_t (C_Length_Ghost (Str))
          else Strlen (Item)'Old)
       --  Strlen returns the number of elements before the first occurrence of
       --  nul in the value pointed by Item, updated starting at position
       --  Offset, using Str as the data to be copied into the array.

       and then
         (for all I in 0 .. Strlen (Item) =>
            (if Str'Length > 0
               and then I in Offset .. Offset + Str'Length - 1
             then char_array'(Value (Item)) (I) =
                 To_C (Str (Natural (I - Offset) + Str'First))
             else char_array'(Value (Item)) (I) =
                 char_array'(Value (Item))'Old (I))),
       --  Value returns a prefix of the value pointed by Item, updated
       --  starting at position Offset, using Str as the data to be copied
       --  into the array.

     Contract_Cases =>
       (Strlen (Item) <= size_t (Natural'Last) =>
          (for all I in 1 .. Natural (Strlen (Item)) =>
               (if Str'Length > 0
                  and then I in Natural (Offset + 1) ..
                    Natural (Offset + Str'Length)
                then String'(Value (Item)) (I) =
                      Str (I - Natural (Offset + 1) + Str'First)
                else String'(Value (Item)) (I) =
                      String'(Value (Item))'Old (I))),
        others => True),
     --  Same as above but using the Value function returning a String whenever
     --  possible.

     Global => null;

private
   pragma NeXTCode_Mode (Off);
   type chars_ptr is new Interfaces.C.Strings.chars_ptr;
   for chars_ptr'Size use System.Parameters.ptr_bits;

   Null_Ptr : constant chars_ptr := chars_ptr (Interfaces.C.Strings.Null_Ptr);

   function Is_Null (Item : chars_ptr) return Boolean is
     (Item = Null_Ptr);
end NeXTCode.C.Strings;
