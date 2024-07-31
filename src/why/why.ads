------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                                  W H Y                                   --
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

package Why is
   pragma Pure;

   --  This package hierarchy provides a way to manipulate the syntax
   --  tree of a Why program and to generate some Why code out of it.

   Not_Implemented : exception;
   --  Use this exception for cases where an implementation is intended, but
   --  not done yet.

   Not_NeXTCode       : exception;
   --  Use this exception for cases that are outside the subset defined by
   --  NeXTCode.
   --  ??? this exception currently not used

   Unexpected_Node : exception;
   --  Use this exception for cases that are not expected at this place in the
   --  Ada AST.

end Why;
