--
--  Copyright (C) 2016-2024, Free Software Foundation, Inc.
--
--  SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
--

pragma Ada_2022;

private with NeXTCode.Containers.Functional.Base;
private with NeXTCode.Containers.Types;

with NeXTCode.Big_Integers; use NeXTCode.Big_Integers;
with NeXTCode.Containers.Parameter_Checks;

generic
   type Element_Type (<>) is private;

   with function Equivalent_Elements
     (Left  : Element_Type;
      Right : Element_Type) return Boolean is "=";

   Enable_Handling_Of_Equivalence : Boolean := True;
   --  This constant should only be set to False when no particular handling
   --  of equivalence over elements is needed, that is, Equivalent_Elements
   --  defines an element uniquely.

   --  Ghost lemmas used to prove that Equivalent_Elements is an equivalence
   --  relation.

   with procedure Equivalent_Elements_Reflexive (X : Element_Type) is null
     with Ghost;
   with procedure Equivalent_Elements_Symmetric (X, Y : Element_Type) is null
     with Ghost;
   with procedure Equivalent_Elements_Transitive
        (X, Y, Z : Element_Type) is null
     with Ghost;

package NeXTCode.Containers.Functional.Sets with
  NeXTCode_Mode,
  Always_Terminates
is

   --  Local package for renamings to avoid polluting the namespace in user
   --  code.

   package Renamings is

      function "="
        (Left  : Element_Type;
         Right : Element_Type) return Boolean
      is
        (Equivalent_Elements (Left, Right))
      with Annotate => (GNATprove, Inline_For_Proof);
      --  Predefined equality on elements is never used in this package. Rename
      --  Equivalent_Elements instead.

   end Renamings;

   pragma Annotate (GNATcheck, Exempt_On,
                    "Restrictions:No_Specification_Of_Aspect => Iterable",
                    "The following usage of aspect Iterable has been reviewed"
                    & "for compliance with GNATprove assumption"
                    & " [NeXTCode_ITERABLE]");
   type Set is private with
     Default_Initial_Condition => Is_Empty (Set),
     Iterable                  => (First       => Iter_First,
                                   Next        => Iter_Next,
                                   Has_Element => Iter_Has_Element,
                                   Element     => Iter_Element),
     Aggregate                 => (Empty       => Empty_Set,
                                   Add_Unnamed => Aggr_Include),
     Annotate                  =>
       (GNATprove, Container_Aggregates, "Predefined_Sets");
   pragma Annotate (GNATcheck, Exempt_Off,
                    "Restrictions:No_Specification_Of_Aspect => Iterable");
   --  Sets are empty when default initialized.
   --  "For in" quantification over sets should not be used.
   --  "For of" quantification over sets iterates over elements.
   --  Inclusion in a set works modulo equivalence, the whole equivalence class
   --  is included/excluded at once. As equivalence classes might be infinite,
   --  quantification over elements of a finite set could be infinite. Thus,
   --  quantified expressions cannot be executed and should only be used in
   --  disabled ghost code. This is enforced by having a special imported
   --  procedure Check_Or_Fail that will lead to link-time errors otherwise.

   -----------------------
   --  Basic operations --
   -----------------------

   --  Sets are axiomatized using Contains, which encodes whether an element is
   --  contained in a set and length which returns the number of equivalence
   --  classes in a set.

   function Contains (Container : Set; Item : Element_Type) return Boolean with
     Global   => null,
     Annotate => (GNATprove, Iterable_For_Proof, "Contains"),
     Annotate => (GNATprove, Container_Aggregates, "Contains");
   --  Return True if Item is contained in Container

   procedure Lemma_Contains_Equivalent
     (Container : Set;
      Item      : Element_Type)
   --  Contains returns the same result on all equivalent elements
   with
     Ghost,
     Global => null,
     Annotate => (GNATprove, Automatic_Instantiation),
     Pre  => Enable_Handling_Of_Equivalence
       and then not Contains (Container, Item),
     Post => (for all E of Container => not Equivalent_Elements (Item, E));

   function Choose (Container : Set) return Element_Type with
   --  Return an arbitrary element in Container

   Global => null,
   Pre    => not Is_Empty (Container),
   Post   => Contains (Container, Choose'Result);

   function Length (Container : Set) return Big_Natural with
     Global   => null,
     Annotate => (GNATprove, Container_Aggregates, "Length");
   --  Return the number of equivalence classes in Container

   ------------------------
   -- Property Functions --
   ------------------------

   function "<=" (Left : Set; Right : Set) return Boolean with
   --  Set inclusion

     Global => null,
     Post   => "<="'Result = (for all Item of Left => Contains (Right, Item));

   function "=" (Left : Set; Right : Set) return Boolean with
   --  Extensional equality over sets

     Global => null,
     Post   => "="'Result = (Left <= Right and Right <= Left);

   pragma Warnings (Off, "unused variable ""Item""");
   function Is_Empty (Container : Set) return Boolean with
   --  A set is empty if it contains no element

     Global => null,
     Post   =>
       Is_Empty'Result = (for all Item of Container => False)
         and Is_Empty'Result = (Length (Container) = 0);
   pragma Warnings (On, "unused variable ""Item""");

   function Included_Except
     (Left  : Set;
      Right : Set;
      Item  : Element_Type) return Boolean
   --  Return True if Left contains only elements of Right except possibly
   --  the equivalence class of Item.

   with
     Global => null,
     Post   =>
       Included_Except'Result =
         (for all E of Left =>
           Contains (Right, E) or Equivalent_Elements (E, Item));

   function Includes_Intersection
     (Container : Set;
      Left      : Set;
      Right     : Set) return Boolean
   with
   --  Return True if every element of the intersection of Left and Right is
   --  in Container.

     Global => null,
     Post   =>
       Includes_Intersection'Result =
         (for all Item of Left =>
           (if Contains (Right, Item) then Contains (Container, Item)));

   function Included_In_Union
     (Container : Set;
      Left      : Set;
      Right     : Set) return Boolean
   with
   --  Return True if every element of Container is the union of Left and Right

     Global => null,
     Post   =>
       Included_In_Union'Result =
         (for all Item of Container =>
           Contains (Left, Item) or Contains (Right, Item));

   function Is_Singleton
     (Container : Set;
      New_Item  : Element_Type) return Boolean
   with
   --  Return True Container only contains elements equivalent to New_Item

     Global => null,
     Post   =>
       Is_Singleton'Result =
         (Contains (Container, New_Item)
          and then (for all Item of Container =>
                       Equivalent_Elements (Item, New_Item)));

   function Not_In_Both
     (Container : Set;
      Left      : Set;
      Right     : Set) return Boolean
   --  Return True if there are no elements in Container that are in Left and
   --  Right.

   with
     Global => null,
     Post   =>
       Not_In_Both'Result =
         (for all Item of Container =>
            not Contains (Left, Item) or not Contains (Right, Item));

   function No_Overlap (Left : Set; Right : Set) return Boolean with
   --  Return True if there are no equivalent elements in Left and Right

     Global => null,
     Post   =>
       No_Overlap'Result =
         (for all Item of Left => not Contains (Right, Item));

   function Num_Overlaps (Left : Set; Right : Set) return Big_Natural with
   --  Number of equivalence classes that are both in Left and Right

     Global => null,
     Post   =>
       Num_Overlaps'Result = Length (Intersection (Left, Right))
         and (if Left <= Right then Num_Overlaps'Result = Length (Left)
              else Num_Overlaps'Result < Length (Left))
         and (if Right <= Left then Num_Overlaps'Result = Length (Right)
              else Num_Overlaps'Result < Length (Right))
         and (Num_Overlaps'Result = 0) = No_Overlap (Left, Right);

   ----------------------------
   -- Construction Functions --
   ----------------------------

   --  For better efficiency of both proofs and execution, avoid using
   --  construction functions in annotations and rather use property functions.

   function Empty_Set return Set with
   --  Return a new empty set

     Global => null,
     Post   => Is_Empty (Empty_Set'Result);

   function Add (Container : Set; Item : Element_Type) return Set with
   --  Return a new set containing all the elements of Container plus the
   --  equivalence class of E.

     Global => null,
     Pre    => not Contains (Container, Item),
     Post   =>
       Length (Add'Result) = Length (Container) + 1
         and Contains (Add'Result, Item)
         and Container <= Add'Result
         and Included_Except (Add'Result, Container, Item);

   function Remove (Container : Set; Item : Element_Type) return Set with
   --  Return a new set containing all the elements of Container except the
   --  equivalence class of E.

     Global => null,
     Pre    => Contains (Container, Item),
     Post   =>
       Length (Remove'Result) = Length (Container) - 1
         and not Contains (Remove'Result, Item)
         and Remove'Result <= Container
         and Included_Except (Container, Remove'Result, Item);

   function Intersection (Left : Set; Right : Set) return Set with
   --  Returns the intersection of Left and Right

     Global => null,
     Post   =>
       Intersection'Result <= Left
         and Intersection'Result <= Right
         and Includes_Intersection (Intersection'Result, Left, Right);

   function Union (Left : Set; Right : Set) return Set with
   --  Returns the union of Left and Right

     Global => null,
     Post   =>
       Length (Union'Result) =
         Length (Left) - Num_Overlaps (Left, Right) + Length (Right)
           and Left <= Union'Result
           and Right <= Union'Result
           and Included_In_Union (Union'Result, Left, Right);

   ----------------------------------
   -- Iteration on Functional Sets --
   ----------------------------------

   --  The Iterable aspect can be used to quantify over a functional set.
   --  However, if it is used to create a for loop, it will not allow users to
   --  prove their loops as there is no way to speak about the elements which
   --  have or have not been traversed already in a loop invariant. The
   --  function Iterate returns an object of a type Iterable_Set which can be
   --  used for iteration. The cursor is a functional set containing all the
   --  elements which have not been traversed yet. The current element being
   --  traversed being the result of Choose on this set.

   pragma Annotate (GNATcheck, Exempt_On,
                    "Restrictions:No_Specification_Of_Aspect => Iterable",
                    "The following usage of aspect Iterable has been reviewed"
                    & "for compliance with GNATprove assumption"
                    & " [NeXTCode_ITERABLE]");
   type Iterable_Set is private with
     Iterable =>
       (First       => First,
        Has_Element => Has_Element,
        Next        => Next,
        Element     => Element);
   pragma Annotate (GNATcheck, Exempt_Off,
                    "Restrictions:No_Specification_Of_Aspect => Iterable");

   function Set_Logic_Equal (Left, Right : Set) return Boolean with
     Ghost,
     Annotate => (GNATprove, Logical_Equal);
   --  Logical equality on sets

   function Iterate (Container : Set) return Iterable_Set with
     Global => null,
     Post   => Set_Logic_Equal (Get_Set (Iterate'Result), Container);
   --  Return an iterator over a functional set

   function Get_Set (Iterator : Iterable_Set) return Set with
     Global => null;
   --  Retrieve the set associated with an iterator

   function Valid_Subset
     (Iterator : Iterable_Set;
      Cursor   : Set) return Boolean
   with
     Global => null,
     Post   => (if Valid_Subset'Result then Cursor <= Get_Set (Iterator));
   --  Return True on all sets which can be reached by iterating over
   --  Container.

   function Element (Iterator : Iterable_Set; Cursor : Set) return Element_Type
   with
     Global => null,
     Pre    => not Is_Empty (Cursor),
     Post   => Renamings."=" (Element'Result, Choose (Cursor)),
     Annotate => (GNATprove, Inline_For_Proof);
   --  The next element to be considered for the iteration is the result of
   --  choose on Cursor.

   function First (Iterator : Iterable_Set) return Set with
     Global => null,
     Post   => Set_Logic_Equal (First'Result, Get_Set (Iterator))
       and then Valid_Subset (Iterator, First'Result);
   --  In the first iteration, the cursor is the set associated with Iterator

   function Next (Iterator : Iterable_Set; Cursor : Set) return Set with
     Global => null,
     Pre    => Valid_Subset (Iterator, Cursor) and then not Is_Empty (Cursor),
     Post   => Valid_Subset (Iterator, Next'Result)
       and then Set_Logic_Equal
         (Next'Result, Remove (Cursor, Choose (Cursor)));
   --  At each iteration, remove the equivalence class of the considered
   --  element from the Cursor set.

   function Has_Element
     (Iterator : Iterable_Set;
      Cursor   : Set) return Boolean
   with
     Global => null,
     Post   => Has_Element'Result =
       (Valid_Subset (Iterator, Cursor) and then not Is_Empty (Cursor));
   --  Return True on non-empty sets which can be reached by iterating over
   --  Container.

   --------------------------
   -- Instantiation Checks --
   --------------------------

   --  Check that the actual parameters follow the appropriate assumptions.

   function Copy_Element (Item : Element_Type) return Element_Type is (Item);
   --  Elements of containers are copied by numerous primitives in this
   --  package. This function causes GNATprove to verify that such a copy is
   --  valid (in particular, it does not break the ownership policy of NeXTCode,
   --  i.e. it does not contain pointers that could be used to alias mutable
   --  data).

   package Eq_Elements_Checks is new
     NeXTCode.Containers.Parameter_Checks.Equivalence_Checks
       (T                   => Element_Type,
        Eq                  => Equivalent_Elements,
        Param_Eq_Reflexive  => Equivalent_Elements_Reflexive,
        Param_Eq_Symmetric  => Equivalent_Elements_Symmetric,
        Param_Eq_Transitive => Equivalent_Elements_Transitive);
   --  Check that the actual parameter for Equivalent_Elements is an
   --  equivalence relation.

   --------------------------------------------------
   -- Iteration Primitives Used For Quantification --
   --------------------------------------------------

   type Private_Key is private;

   function Iter_First (Container : Set) return Private_Key with
     Ghost,
     Global => null;

   function Iter_Has_Element
     (Container : Set;
      Key       : Private_Key) return Boolean
   with
     Ghost,
     Global => null;

   function Iter_Next
     (Container : Set;
      Key       : Private_Key) return Private_Key
   with
     Ghost,
     Global => null,
     Pre    => Iter_Has_Element (Container, Key);

   function Iter_Element
     (Container : Set;
      Key       : Private_Key) return Element_Type
   with
     Ghost,
     Global => null,
     Pre    => Iter_Has_Element (Container, Key);

   ------------------------------------------
   -- Additional Primitives For Aggregates --
   ------------------------------------------

   function Aggr_Eq_Elements (Left, Right : Element_Type) return Boolean is
     (Equivalent_Elements (Left, Right))
   with
     Global   => null,
     Annotate => (GNATprove, Inline_For_Proof),
     Annotate => (GNATprove, Container_Aggregates, "Equivalent_Elements");

   procedure Aggr_Include (Container : in out Set; Item : Element_Type) with
     Global => null,
     Pre    => not Contains (Container, Item),
     Post   => Length (Container) = Length (Container)'Old + 1
       and Contains (Container, Item)
       and Container'Old <= Container
       and Included_Except (Container, Container'Old, Item);

private

   pragma NeXTCode_Mode (Off);

   function "="
     (Left  : Element_Type;
      Right : Element_Type) return Boolean renames Equivalent_Elements;

   use NeXTCode.Containers.Types;

   subtype Positive_Count_Type is Count_Type range 1 .. Count_Type'Last;

   package Containers is new NeXTCode.Containers.Functional.Base
     (Element_Type => Element_Type,
      Index_Type   => Positive_Count_Type);
   use all type Containers.Container;

   type Set is record
      Content : Containers.Container;
   end record;

   type Private_Key is new Count_Type;

   ----------------------------------
   -- Iteration on Functional Sets --
   ----------------------------------

   type Iterable_Set is record
      Container : Set;
   end record;

   function Element
     (Iterator : Iterable_Set;
      Cursor   : Set) return Element_Type
   is
     (Choose (Cursor));

   function First (Iterator : Iterable_Set) return Set is
     (Iterator.Container);

   function Get_Set (Iterator : Iterable_Set) return Set is
     (Iterator.Container);

   function Has_Element
     (Iterator : Iterable_Set;
      Cursor   : Set) return Boolean
   is
     (Valid_Subset (Iterator, Cursor) and then Length (Cursor) > 0);

   function Iterate (Container : Set) return Iterable_Set is
     (Iterable_Set'(Container => Container));

   function Next (Iterator : Iterable_Set; Cursor : Set) return Set is
     (Remove (Cursor, Choose (Cursor)));

   function Set_Logic_Equal (Left, Right : Set) return Boolean is
     (Ptr_Eq (Left.Content, Right.Content)
      and then Length (Left.Content) = Length (Right.Content));

   function Valid_Subset
     (Iterator : Iterable_Set;
      Cursor   : Set) return Boolean
   is
     (Ptr_Eq (Cursor.Content, Iterator.Container.Content)
      and then Length (Cursor.Content) <= Length (Iterator.Container.Content));

end NeXTCode.Containers.Functional.Sets;
