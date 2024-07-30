------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                     A S S U M P T I O N S . S E A R C H                  --
--                                                                          --
--                                 B o d y                                  --
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

with Ada.Containers.Ordered_Maps;

package body Assumptions.Search is

   package Goal_Maps is new Ada.Containers.Ordered_Maps
     (Key_Type        => Token,
      Element_Type    => Token_Sets.Set,
      "<"             => "<",
      "="             => Token_Sets."=");

   Goals : Goal_Maps.Map := Goal_Maps.Empty_Map;

   ------------
   -- Import --
   ------------

   procedure Import (L : Rule_Lists.List) is
   begin
      for Elt of L loop
         Goals.Insert (Key      => Elt.Claim,
                       New_Item => Elt.Assumptions);
         --  ??? Elt.Assumptions can be Moved to avoid copying
      end loop;
   end Import;

   ----------------------
   -- Claim_Depends_On --
   ----------------------

   function Claim_Depends_On (C : Token) return Token_Sets.Set is

      use Goal_Maps;

      Needed_Claims     : Token_Sets.Set := Token_Sets.To_Set (C);
      Unverified_Claims : Token_Sets.Set := Token_Sets.Empty_Set;
      Seen              : Token_Sets.Set := Token_Sets.Empty_Set;

   begin
      while not Needed_Claims.Is_Empty loop
         declare
            Cur_Token : constant Token :=
              Needed_Claims (Needed_Claims.First);

            Map_Cur : constant Goal_Maps.Cursor := Goals.Find (Cur_Token);

            Unused   : Token_Sets.Cursor;
            Inserted : Boolean;

         begin

            if Goal_Maps.Has_Element (Map_Cur) then
               Seen.Insert (New_Item => Cur_Token,
                            Position => Unused,
                            Inserted => Inserted);

               if Inserted then
                  Needed_Claims.Union (Goals (Map_Cur));
               end if;
            else
               Unverified_Claims.Include (Cur_Token);
            end if;

            Needed_Claims.Delete (Cur_Token);
         end;
      end loop;

      return Unverified_Claims;
   end Claim_Depends_On;

   --------------------
   -- Get_All_Claims --
   --------------------

   function Get_All_Claims return Token_Sets.Set is
      S : Token_Sets.Set := Token_Sets.Empty_Set;
   begin
      for Cursor in Goals.Iterate loop
         S.Insert (Goal_Maps.Key (Cursor));
      end loop;

      return S;
   end Get_All_Claims;

end Assumptions.Search;
