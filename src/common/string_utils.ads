------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                         S T R I N G - U T I L S                          --
--                                                                          --
--                                 S p e c                                  --
--                                                                          --
-------------------------------------------------------------------------------
--
-- Copyright (c) 2024, NeXTech Corporation. All rights reserved.
-- DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
--
-- This code is distributed in the hope that it will be useful, but WITHOUT
-- ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
-- FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
-- version 2 for more details (a copy is included in the LICENSE file that
-- accompanied this code).
--
-- Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
--             Tural Ghuliev (turalquliyev@it-gss.com)
--
-------------------------------------------------------------------------------

with Ada.Strings.Fixed;
with Ada.Containers;
with Ada.Containers.Indefinite_Doubly_Linked_Lists;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with GNAT.Strings;          use GNAT.Strings;

package String_Utils is

   package String_Lists is new
     Ada.Containers.Indefinite_Doubly_Linked_Lists (String);

   use type String_Lists.Cursor;

   function Capitalize_First (S : String) return String;
   --  Return a string with first character capitalized

   procedure Capitalize_First (S : in out String);
   --  Modify S in place to capitalize the first character

   procedure Lower_Case_First (S : in out String);
   --  Modify S in place to capitalize the first character

   function Lower_Case_First (S : String) return String;

   function Standard_Ada_Case (S : String) return String;
   --  Return a string with standard Ada case, where each word separated by an
   --  underscore is capitalized.

   function Hash_Image (N : Ada.Containers.Hash_Type) return String;
   --  Generate a string from a hash, without the leading space

   function Null_Or_Empty_String (S : GNAT.Strings.String_Access)
                                  return Boolean
   is
     (S = null or else S.all = "");
   --  Return True iff S is null or the empty string

   function To_Unbounded_String (X : Boolean) return Unbounded_String is
     (To_Unbounded_String (if X then "True" else "False"));
   --  Function to print booleans

   function Trimi (S : String; C : Character) return String;
   --  Return a copy of S with all occurences of C removed

   function Case_Insensitive_Find (SL : String_Lists.List; Item : String)
                                   return String_Lists.Cursor;
   --  @param SL a list of strings
   --  @param Item a string to be found in the list
   --  @return True if a String S is in the list SL which is equal modulo
   --    casing to Item

   function Case_Insensitive_Contains (SL : String_Lists.List; Item : String)
                                       return Boolean is
     (Case_Insensitive_Find (SL, Item) /= String_Lists.No_Element);

   function Contains (S, Sub : String) return Boolean is
     (Ada.Strings.Fixed.Index (Source => S, Pattern => Sub) /= 0)
   with Pre => Sub /= "";
   --  Returns True iff string S contains substring Sub

end String_Utils;
