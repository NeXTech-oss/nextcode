//===--- ColorUtils.h - -----------------------------------------*- C++ -*-===//
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
//
// This file defines several utilities for helping print colorful outputs
// to the terminal.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_BASIC_COLORUTILS_H
#define NEXTCODE_BASIC_COLORUTILS_H

#include "llvm/Support/raw_ostream.h"

namespace nextcode {

/// RAII class for setting a color for a raw_ostream and resetting when it goes
/// out-of-scope.
class OSColor {
  llvm::raw_ostream &OS;
  bool HasColors;
public:
  OSColor(llvm::raw_ostream &OS, llvm::raw_ostream::Colors Color) : OS(OS) {
    HasColors = OS.has_colors();
    if (HasColors)
      OS.changeColor(Color);
  }
  ~OSColor() {
    if (HasColors)
      OS.resetColor();
  }

  OSColor &operator<<(char C) { OS << C; return *this; }
  OSColor &operator<<(llvm::StringRef Str) { OS << Str; return *this; }
};

/// A stream which forces color output.
class ColoredStream : public raw_ostream {
  raw_ostream &Underlying;

public:
  explicit ColoredStream(raw_ostream &underlying) : Underlying(underlying) {}
  ~ColoredStream() override { flush(); }

  raw_ostream &changeColor(Colors color, bool bold = false,
                           bool bg = false) override {
    Underlying.changeColor(color, bold, bg);
    return *this;
  }
  raw_ostream &resetColor() override {
    Underlying.resetColor();
    return *this;
  }
  raw_ostream &reverseColor() override {
    Underlying.reverseColor();
    return *this;
  }
  bool has_colors() const override { return true; }

  void write_impl(const char *ptr, size_t size) override {
    Underlying.write(ptr, size);
  }
  uint64_t current_pos() const override {
    return Underlying.tell() - GetNumBytesInBuffer();
  }

  size_t preferred_buffer_size() const override { return 0; }
};

/// A stream which drops all color settings.
class NoColorStream : public raw_ostream {
  raw_ostream &Underlying;

public:
  explicit NoColorStream(raw_ostream &underlying) : Underlying(underlying) {}
  ~NoColorStream() override { flush(); }

  bool has_colors() const override { return false; }

  void write_impl(const char *ptr, size_t size) override {
    Underlying.write(ptr, size);
  }
  uint64_t current_pos() const override {
    return Underlying.tell() - GetNumBytesInBuffer();
  }

  size_t preferred_buffer_size() const override { return 0; }
};

} // namespace nextcode

#endif // NEXTCODE_BASIC_COLORUTILS_H
