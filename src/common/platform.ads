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

package Platform is

   --  Value CodePeer_OS for the host OS flavor is for analysis of GNATprove
   --  code by CodePeer.

   type Host_Operating_System_Flavor is
      (X86_Windows, X86_64_Windows, X86_Linux, X86_64_Linux, X86_64_Darwin,
       X86_64_FreeBSD, CodePeer_OS, AArch64_Darwin, AArch64_Linux);

   function Get_OS_Flavor return Host_Operating_System_Flavor;

end Platform;
