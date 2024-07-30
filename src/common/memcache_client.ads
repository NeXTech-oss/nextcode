------------------------------------------------------------------------------
--                                                                          --
--                            GNATPROVE COMPONENTS                          --
--                                                                          --
--                       M E M C A C H E _ C L I E N T                      --
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
with GNAT.Sockets; use GNAT.Sockets;

package Memcache_Client is

   --  Package that handles a connection and communication with a memcached
   --  server. Currently, only simple get and set operations are supported.

   type Cache_Connection is new Cache with private;

   function Init (Hostname : String;
                  Port     : Port_Type) return Cache_Connection;
   --  @param Hostname hostname or IP address of a memcached server
   --  @param Port     port to connect to
   --  @return a connection object that can be used with the below get/set
   --    functions

   overriding procedure Set (Conn  : Cache_Connection;
                  Key   : String;
                  Value : String);
   --  @param Conn a connection object to a memcached server
   --  @param Key the key for the data to be stored
   --  @param Value the value to be cached

   overriding function Get (Conn : Cache_Connection;
                            Key  : String) return String;
   --  @param Conn a connection object to a memcached server
   --  @param Key the key for the data to be retrieved
   --  @return the value stored in the server for Key or empty if no value is
   --    stored

   overriding procedure Close (Conn : in out Cache_Connection);
   --  @param Conn the connection to be closed

private

   type Cache_Connection is new Cache with
      record
         Sock   : Socket_Type;
         Stream : Stream_Access;
      end record;

end Memcache_Client;
