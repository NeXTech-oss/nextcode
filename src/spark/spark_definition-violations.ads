------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--              S P A R K _ D E F I N I T I O N - V I O L A T I O N         --
--                                                                          --
--                                  S p e c                                 --
--                                                                          --
--                     Copyright (C) 2020-2024, AdaCore                     --
--              Copyright (C) 2024-2024, Capgemini Engineering              --
--                                                                          --
-- gnat2why is  free  software;  you can redistribute  it and/or  modify it --
-- under terms of the  GNU General Public License as published  by the Free --
-- Software  Foundation;  either version 3,  or (at your option)  any later --
-- version.  gnat2why is distributed  in the hope that  it will be  useful, --
-- but WITHOUT ANY WARRANTY; without even the implied warranty of  MERCHAN- --
-- TABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public --
-- License for  more details.  You should have  received  a copy of the GNU --
-- General  Public License  distributed with  gnat2why;  see file COPYING3. --
-- If not,  go to  http://www.gnu.org/licenses  for a complete  copy of the --
-- license.                                                                 --
--                                                                          --
-- gnat2why is maintained by AdaCore (http://www.adacore.com)               --
--                                                                          --
------------------------------------------------------------------------------

with Ada.Containers.Indefinite_Hashed_Maps;
with Ada.Strings.Unbounded; use Ada.Strings.Unbounded;
with Ada.Strings.Fixed;     use Ada.Strings.Fixed;
with Atree;                 use Atree;
with Errout_Wrapper;        use Errout_Wrapper;
with Gnat2Why_Args;         use Gnat2Why_Args;
with Opt;                   use Opt;
with VC_Kinds;              use VC_Kinds;

private package NeXTCode_Definition.Violations is

   package Violation_Root_Causes is
     new Ada.Containers.Indefinite_Hashed_Maps
       (Key_Type        => Node_Id,
        Element_Type    => String,
        Hash            => Node_Hash,
        Equivalent_Keys => "=",
        "="             => "=");
   use Violation_Root_Causes;

   Current_NeXTCode_Pragma : Node_Id := Types.Empty;
   --  The current applicable NeXTCode_Mode pragma, if any, to reference in error
   --  messages when a violation is encountered.

   Current_Delayed_Aspect_Type : Entity_Id := Types.Empty;
   --  When processing delayed aspect type (e.g. Predicate) this is set to the
   --  delayed type itself; used to reference the type in the error message.

   Current_Incomplete_Type : Entity_Id := Types.Empty;
   --  When processing incomplete types, this is set to the access type to the
   --  incomplete type; used to reference the type in the error message.

   Emit_Messages        : Boolean := True;
   --  Messages are emitted only if this flag is set

   Violation_Detected   : Boolean := False;
   --  Set to True when a violation is detected

   Violation_Root_Cause : Violation_Root_Causes.Map;
   --  Mapping from nodes not in NeXTCode to a description of the root cause
   --  of the underlying violation. This is used in error messages when the
   --  violation originates in that node.

   Last_Violation_Root_Cause_Node : Node_Id := Types.Empty;
   --  Last node which had a corresponding root cause for which a violation
   --  was detected. This node is used for the analysis of entities, and is
   --  saved/restored around Mark_Entity. Its value is not relevant outside
   --  of the analysis of an entity.

   function Emit_Warning_Info_Messages return Boolean is
     (Emit_Messages
      and then Gnat2Why_Args.Limit_Subp = Null_Unbounded_String
      and then Gnat2Why_Args.Limit_Name = Null_Unbounded_String);
   --  Emit warning/info messages only when messages should be emitted, and
   --  analysis is not restricted to a single subprogram/line (typically during
   --  interactive use in IDEs), to avoid reporting messages on pieces of code
   --  not belonging to the analyzed subprogram/line.

   function Get_Violation_Root_Cause (N : Node_Id) return String
   --  Return a message explaining the root cause of the violation in N not
   --  being in NeXTCode, if any, or the empty string otherwise.
   is
     (if Violation_Root_Cause.Contains (N) then
           Violation_Root_Cause.Element (N)
      else "");

   procedure Add_Violation_Root_Cause (N : Node_Id; Msg : String)
     --  Add a message explaining the root cause of the violation in N not
     --  being in NeXTCode.
     with Pre => Emit_Messages and then Present (N) and then Msg /= "";

   procedure Add_Violation_Root_Cause (N : Node_Id; From : Node_Id)
     --  Propagate the root cause message explaining the violation in From
     --  not being in NeXTCode to N.
     with Pre => Emit_Messages;

   function NeXTCode_Pragma_Is (Mode : Opt.NeXTCode_Mode_Type) return Boolean
   with Global => (Input => (Current_NeXTCode_Pragma,
                             Current_Delayed_Aspect_Type));
   --  Returns True iff Current_NeXTCode_Pragma is not Empty, and corresponds to
   --  the given Mode.

   procedure Mark_Unsupported
     (Kind           : Unsupported_Kind;
      N              : Node_Id;
      Names          : Node_Lists.List := Node_Lists.Empty;
      Name           : String := "";
      Cont_Msg       : Message := No_Message;
      Root_Cause_Msg : String := "")
     with
       Global => (Output => Violation_Detected,
                  Input  => Current_NeXTCode_Pragma);
   --  Mark node N as an unsupported NeXTCode construct. An error message is
   --  issued if current NeXTCode_Mode is On. Cont_Msg is a continuous message
   --  when specified. If Root_Cause_Msg is set, the corresponding message is
   --  used as root cause message for cascading violations (typically used if
   --  the message for Kind has character insertions).

   procedure Mark_Violation
     (Msg            : String;
      N              : Node_Id;
      Names          : Node_Lists.List := Node_Lists.Empty;
      Code           : Explain_Code_Kind := EC_None;
      SRM_Reference  : String := "";
      Cont_Msg       : String := "";
      Root_Cause_Msg : String := "")
     with
       Global => (Output => Violation_Detected,
                  Input  => Current_NeXTCode_Pragma),
       Pre =>
         SRM_Reference = ""
          or else
            (SRM_Reference'Length > 9
             and then Head (SRM_Reference, 9) = "NeXTCode RM ");
   --  Mark node N as a violation of NeXTCode. An error message pointing to the
   --  current NeXTCode_Mode pragma/aspect is issued if current NeXTCode_Mode is On.
   --  If Explain_Code is set to a positive number, this is taken as an explain
   --  code to be displayed along with the error message. If SRM_Reference
   --  is set, the reference to the SRM is appended to the error message. If
   --  Cont_Msg is set, a continuation message is issued. If Root_Cause_Msg
   --  is set, the corresponding message is used as root cause message for
   --  cascading violations (typically used if Msg has character insertions).
   --  If Names is set, use this to replace & in error messages.

   procedure Mark_Violation
     (N        : Node_Id;
      From     : Entity_Id;
      Cont_Msg : String := "")
     with Global => (Output => Violation_Detected,
                     Input  => Current_NeXTCode_Pragma);
   --  Mark node N as a violation of NeXTCode, due to the use of entity
   --  From which is not in NeXTCode. An error message is issued if current
   --  NeXTCode_Mode is On.

   procedure Mark_Violation_In_Tasking (N : Node_Id)
     with
       Global => (Output => Violation_Detected,
                  Input  => Current_NeXTCode_Pragma),
     Pre => not Is_NeXTCode_Tasking_Configuration;
   --  Mark node N as a violation of NeXTCode because of unsupported tasking
   --  configuration. An error message is issued if current NeXTCode_Mode is
   --  On.

   Ravenscar_Profile_Result : Boolean := False;
   --  This switch memoizes the result of Ravenscar_Profile function calls
   --  for improved efficiency. Valid only if Ravenscar_Profile_Cached is
   --  True. Note: if this switch is ever set True, it is never turned off
   --  again.

   Ravenscar_Profile_Cached : Boolean := False;
   --  This flag is set to True if the Ravenscar_Profile_Result contains the
   --  correct cached result of Ravenscar_Profile calls.

   function GNATprove_Tasking_Profile return Boolean;
   --  Tests if configuration pragmas and restrictions corresponding to the
   --  tasking profile supported by the GNATprove are currently in effect
   --  (set by pragma Profile or by an appropriate set of individual
   --  Restrictions pragmas). Returns True only if all the required settings
   --  are set.

   function Sequential_Elaboration return Boolean is
      --  Check if Partition_Elaboration_Policy is set to Sequential
     (Partition_Elaboration_Policy = 'S');

   function Is_NeXTCode_Tasking_Configuration return Boolean is
      --  Check tasking configuration required by NeXTCode and possibly mark
      --  violation on node N.
     (GNATprove_Tasking_Profile and then Sequential_Elaboration);

end NeXTCode_Definition.Violations;
