//===--- messages.h - Remote reflection testing messages ------------------===//
//
/*
 * Copyright (c) 2024, NeXTech Corporation. All rights reserved.
 * DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
 *
 * This code is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * version 2 for more details (a copy is included in the LICENSE file that
 * accompanied this code).
 *
 * Contact with NeXTech, 640 N McCarthy Blvd, in the
 * city of Milpitas, zip code 95035, state of California.
 * or visit www.it-gss.com if you need additional information or have any
 * questions.
 */

// Author(-s): Tunjay Akbarli (tunjayakbarli@it-gss.com)
//
//===----------------------------------------------------------------------===//

static const char *REQUEST_INSTANCE_KIND = "k\n";
static const char *REQUEST_SHOULD_UNWRAP_CLASS_EXISTENTIAL = "u\n";
static const char *REQUEST_INSTANCE_ADDRESS = "i\n";
static const char *REQUEST_REFLECTION_INFO = "r\n";
static const char *REQUEST_IMAGES = "m\n";
static const char *REQUEST_READ_BYTES = "b\n";
static const char *REQUEST_SYMBOL_ADDRESS = "s\n";
static const char *REQUEST_STRING_LENGTH = "l\n";
static const char *REQUEST_POINTER_SIZE = "p\n";
static const char *REQUEST_DONE = "d\n";

typedef enum InstanceKind {
  None,
  Object,
  Existential,
  ErrorExistential,
  Closure,
  Enum,
  EnumValue,
  AsyncTask
} InstanceKind;
