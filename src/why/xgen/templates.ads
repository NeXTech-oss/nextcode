------------------------------------------------------------------------------
--                                                                          --
--                            GNAT2WHY COMPONENTS                           --
--                                                                          --
--                            T E M P L A T E S                             --
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

with Outputs; use Outputs;

package Templates is
   --  This package provides some support for substitutions of patterns
   --  in template files.

   procedure Add
     (Pattern   : String;
      Generator : not null access procedure (O : in out Output_Record));
   --  Associate Pattern to Generator

   procedure Process (Filename : String);
   --  Open an output record for Filename and fill Filename using
   --  a template (whose name is Filename suffixed by --  "-tmpl");
   --  when reading the template, each occurence of Patterns in
   --  are replaced by the result of the execution of its associated
   --  generator.
   --
   --  Patterns are recognized by the following syntax:
   --
   --     _@My_Pattern@_
   --
   --  Each time such a pattern is found, the corresponding generator
   --  (registered by Add) is looked up; if not found, no substitution
   --  is done and a warning is issued. Otherwise, the indentation level
   --  of the output is set the indentation level of the pattern and
   --  the generator is called to operate the substitution.

end Templates;
