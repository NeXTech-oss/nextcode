//===--- CommonString.h - C API for NeXTCode Dependency Scanning ---*- C -*-===//
 //
 // This source file is part of the NeXTCode.org open source project
 //
 // Copyright (c) 2014 - 2020 Apple Inc. and the NeXTCode project authors
 // Licensed under Apache License v2.0 with Runtime Library Exception
 //
 // See https://nextcode.org/LICENSE.txt for license information
 // See https://nextcode.org/CONTRIBUTORS.txt for the list of NeXTCode project authors
 //
 //===----------------------------------------------------------------------===//

 #ifndef NEXTCODE_C_LIB_NEXTCODE_COMMON_STRING_H
 #define NEXTCODE_C_LIB_NEXTCODE_COMMON_STRING_H

 #include <stdbool.h>
 #include <stddef.h>
 #include <stdint.h>

 /**
  * A character string used to pass around dependency scan result metadata.
  * Lifetime of the string is strictly tied to the object whose field it
  * represents. When the owning object is released, string memory is freed.
  */
 typedef struct {
   const void *data;
   size_t length;
 } nextcodescan_string_ref_t;

 typedef struct {
   nextcodescan_string_ref_t *strings;
   size_t count;
 } nextcodescan_string_set_t;

 #endif // NEXTCODE_C_LIB_NEXTCODE_COMMON_STRING_H
