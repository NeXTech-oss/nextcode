//===--- MockPlugin.h ---------------------------------------------*- C -*-===//
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

#ifndef NEXTCODE_C_MOCK_PLUGIN_H
#define NEXTCODE_C_MOCK_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

int _mock_plugin_main(const char *);

#ifdef __cplusplus
}
#endif

/// Usage: MOCK_PLUGIN(JSON)
/// 'JSON' is a *bare* JSON value.
#define MOCK_PLUGIN(...)                                                       \
  int main() { return _mock_plugin_main(#__VA_ARGS__); }

#endif // NEXTCODE_C_MOCK_PLUGIN_H
