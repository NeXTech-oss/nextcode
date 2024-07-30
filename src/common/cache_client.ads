------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                          C A C H E _ C L I E N T                         --
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

package Cache_Client is

   type Cache is abstract tagged null record;
   --  Abstract type to represent a key/value cache.

   procedure Set (Conn : Cache; Key : String; Value : String) is abstract;
   --  Procedure to set the value of "Key" to "Value".

   function Get (Conn : Cache; Key : String) return String is abstract;
   --  Function to retrieve the value of "Key". If the key wasn't set
   --  previously, this function returns the empty string. Note that this means
   --  the interface can't distinguish between a key set to an empty string,
   --  and an absent key. This seems to be the case for the memcached
   --  implementation, so we can't be more precise here.

   procedure Close (Conn : in out Cache) is abstract;
   --  Procedure to release any resources associated with the cache

end Cache_Client;
