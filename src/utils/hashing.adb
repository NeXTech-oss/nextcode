------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                              H A S H I N G                               --
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

with Ada.Unchecked_Conversion;

package body Hashing is

   --------------------------
   -- Generic_Integer_Hash --
   --------------------------

   function Generic_Integer_Hash (N : Integer)
                                  return Ada.Containers.Hash_Type
   is
      function To_HT is
         new Ada.Unchecked_Conversion (Source => Integer,
                                       Target => Ada.Containers.Hash_Type);
   begin
      --  We can do something more clever here in the future.
      return To_HT (N);
   end Generic_Integer_Hash;

end Hashing;
