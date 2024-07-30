------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                      F I L E C A C H E _ C L I E N T                     --
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

with Cache_Client; use Cache_Client;
with GNAT.OS_Lib;  use GNAT.OS_Lib;

package Filecache_Client is

   --  Package that implements a simple key/value cache using the file
   --  system. See the Cache_Client package for comments on the Set/Get/Close
   --  subprograms.

   type Filecache is new Cache with private;

   function Init (Dir : String) return Filecache;
   --  Create the file cache in directory Dir.

   overriding procedure Set (Conn : Filecache; Key : String; Value : String);

   overriding function Get (Conn : Filecache; Key : String) return String;

   overriding procedure Close (Conn : in out Filecache);

private

   type Filecache is new Cache with record
      Dir : String_Access;
   end record;

end Filecache_Client;
