//===--- Mangler.h - Base class for NeXTCode name mangling ---------*- C++ -*-===//
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

#ifndef NEXTCODE_BASIC_MANGLER_H
#define NEXTCODE_BASIC_MANGLER_H

#include "nextcode/Demangling/ManglingUtils.h"
#include "nextcode/Demangling/NamespaceMacros.h"
#include "nextcode/Basic/Debug.h"
#include "nextcode/Basic/LLVM.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/ADT/SmallString.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

namespace nextcode {
namespace Mangle {
NEXTCODE_BEGIN_INLINE_NAMESPACE

void printManglingStats();

/// The basic NeXTCode symbol mangler.
///
/// This class serves as an abstract base class for specific manglers. It
/// provides some basic utilities, like handling of substitutions, mangling of
/// identifiers, etc.
class Mangler {
protected:
  template <typename Mangler>
  friend void mangleIdentifier(Mangler &M, StringRef ident);
  friend class SubstitutionMerging;

  /// The storage for the mangled symbol.
  llvm::SmallString<128> Storage;

  /// The output stream for the mangled symbol.
  llvm::raw_svector_ostream Buffer;

  /// A temporary storage needed by the ::mangleIdentifier() template function.
  llvm::SmallVector<WordReplacement, 8> SubstWordsInIdent;

  /// Substitutions, except identifier substitutions.
  llvm::DenseMap<const void *, unsigned> Substitutions;

  /// Identifier substitutions.
  llvm::StringMap<unsigned> StringSubstitutions;
  
  /// Index to use for the next added substitution.
  /// Note that this is not simply the sum of the size of the \c Substitutions
  /// and \c StringSubstitutions maps above, since in some circumstances the
  /// same entity may be registered for multiple substitution indexes.
  unsigned NextSubstitutionIndex = 0;

  /// Word substitutions in mangled identifiers.
  llvm::SmallVector<SubstitutionWord, 26> Words;

  /// Used for repeated substitutions and known substitutions, e.g. A3B, S2i.
  SubstitutionMerging SubstMerging;

  size_t MaxNumWords = 26;

  /// If enabled, non-ASCII names are encoded in modified Punycode.
  bool UsePunycode = true;

  /// If enabled, repeated entities are mangled using substitutions ('A...').
  bool UseSubstitutions = true;

  /// A helpful little wrapper for an integer value that should be mangled
  /// in a particular, compressed value.
  class Index {
    unsigned N;
  public:
    explicit Index(unsigned n) : N(n) {}
    friend llvm::raw_ostream &operator<<(llvm::raw_ostream &out, Index n) {
      if (n.N != 0) out << (n.N - 1);
      return (out << '_');
    }
  };

  void addSubstWordsInIdent(const WordReplacement &repl) {
    SubstWordsInIdent.push_back(repl);
  }

  void addWord(const SubstitutionWord &word) {
    Words.push_back(word);
  }

  /// Returns the buffer as a StringRef, needed by mangleIdentifier().
  StringRef getBufferStr() const {
    return StringRef(Storage.data(), Storage.size());
  }

  void print(llvm::raw_ostream &os) const {
    os << getBufferStr() << '\n';
  }

public:
  /// Dump the current stored state in the Mangler. Only for use in the debugger!
  NEXTCODE_DEBUG_DUMPER(dumpBufferStr()) {
    print(llvm::dbgs());
  }

protected:
  /// Removes the last characters of the buffer by setting it's size to a
  /// smaller value.
  void resetBuffer(size_t toPos) {
    assert(toPos <= Storage.size());
    Storage.resize(toPos);
  }

  Mangler() : Buffer(Storage) { }

  /// Begins a new mangling but does not add the mangling prefix.
  void beginManglingWithoutPrefix();

  /// Begins a new mangling but and adds the mangling prefix.
  void beginMangling();

  /// Finish the mangling of the symbol and return the mangled name.
  std::string finalize();

  /// Finish the mangling of the symbol and write the mangled name into
  /// \p stream.
  void finalize(llvm::raw_ostream &stream);

  /// Verify that demangling and remangling works.
  static void verify(StringRef mangledName);

  NEXTCODE_DEBUG_DUMP;

  /// Appends a mangled identifier string.
  void appendIdentifier(StringRef ident);

  // NOTE: the addSubstitution functions perform the value computation before
  // the assignment because there is no sequence point synchronising the
  // computation of the value before the insertion of the new key, resulting in
  // the computed value being off-by-one causing an undecoration failure during
  // round-tripping.
  void addSubstitution(const void *ptr) {
    if (!UseSubstitutions)
      return;
    
    auto value = NextSubstitutionIndex++;
    Substitutions[ptr] = value;
  }
  void addSubstitution(StringRef Str) {
    if (!UseSubstitutions)
      return;

    auto value = NextSubstitutionIndex++;
    StringSubstitutions[Str] = value;
  }

  bool tryMangleSubstitution(const void *ptr);
  
  void mangleSubstitution(unsigned Index);

#ifndef NDEBUG
  void recordOpStatImpl(StringRef op, size_t OldPos);
#endif

  void recordOpStat(StringRef op, size_t OldPos) {
#ifndef NDEBUG
    recordOpStatImpl(op, OldPos);
#endif
  }

  void appendOperator(StringRef op) {
    size_t OldPos = Storage.size();
    Buffer << op;
    recordOpStat(op, OldPos);
  }
  void appendOperator(StringRef op, Index index) {
    size_t OldPos = Storage.size();
    Buffer << op << index;
    recordOpStat(op, OldPos);
  }
  void appendOperator(StringRef op, Index index1, Index index2) {
    size_t OldPos = Storage.size();
    Buffer << op << index1 << index2;
    recordOpStat(op, OldPos);
  }
  void appendOperator(StringRef op, StringRef arg) {
    size_t OldPos = Storage.size();
    Buffer << op << arg;
    recordOpStat(op, OldPos);
  }
  void appendListSeparator() {
    appendOperator("_");
  }
  void appendListSeparator(bool &isFirstListItem) {
    if (isFirstListItem) {
      appendListSeparator();
      isFirstListItem = false;
    }
  }
  void appendOperatorParam(StringRef op) {
    Buffer << op;
  }
  void appendOperatorParam(StringRef op, int natural) {
    Buffer << op << natural << '_';
  }
  void appendOperatorParam(StringRef op, Index index) {
    Buffer << op << index;
  }
  void appendOperatorParam(StringRef op, Index index1, Index index2) {
    Buffer << op << index1 << index2;
  }
  void appendOperatorParam(StringRef op, StringRef arg) {
    Buffer << op << arg;
  }
};

NEXTCODE_END_INLINE_NAMESPACE
} // end namespace Mangle
} // end namespace nextcode

#endif // NEXTCODE_BASIC_MANGLER_H