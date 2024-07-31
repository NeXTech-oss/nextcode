--
--  Copyright (C) 2022-2024, AdaCore
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

with Ada.Unchecked_Conversion;
with Ada.Unchecked_Deallocation;

package body NeXTCode.Pointers.Pointers_With_Aliasing_Separate_Memory with
  NeXTCode_Mode => Off
is
   procedure Dealloc_Obj is new Ada.Unchecked_Deallocation (Object, Pointer);
   function Pointer_To_Integer is new
     Ada.Unchecked_Conversion (Pointer, Address_Type);

   ---------
   -- "=" --
   ---------

   function "=" (P1, P2 : Pointer) return Boolean is (Eq (P1, P2));

   -------------
   -- Address --
   -------------

   function Address (P : Pointer) return Address_Type is
     (Pointer_To_Integer (P));

   ------------
   -- Assign --
   ------------

   procedure Assign (Memory : in out Memory_Type; P : Pointer; O : Object) is
      pragma Unreferenced (Memory);
   begin
      P.all := O;
   end Assign;

   ------------------------
   -- Constant_Reference --
   ------------------------

   function Constant_Reference (Memory : Memory_Type; P : Pointer)
                                return not null access constant Object
   is
     (P);

   ------------
   -- Create --
   ------------

   procedure Create (Memory : in out Memory_Type; O : Object; P : out Pointer)
   is
      pragma Unreferenced (Memory);
   begin
      P := new Object'(O);
   end Create;

   -------------
   -- Dealloc --
   -------------

   procedure Dealloc (Memory : in out Memory_Type; P : in out Pointer) is
      pragma Unreferenced (Memory);
   begin
      Dealloc_Obj (P);
   end Dealloc;

   -----------
   -- Deref --
   -----------

   function Deref (Memory : Memory_Type; P : Pointer) return Object is (P.all);

   -----------------
   -- Move_Memory --
   -----------------

   procedure Move_Memory (Source, Target : in out Memory_Type; F : Footprint)
   is null;

   ------------------
   -- Null_Pointer --
   ------------------

   function Null_Pointer return Pointer is (null);

   ---------------
   -- Reference --
   ---------------

   function Reference (Memory : not null access Memory_Type; P : Pointer)
                       return   not null access Object
   is
     (P);
end NeXTCode.Pointers.Pointers_With_Aliasing_Separate_Memory;
