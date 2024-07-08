//===--- DeclarationFragmentPrinter.h - Declaration Fragment Printer ------===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_DECLARATIONFRAGMENTPRINTER_H
#define NEXTCODE_SYMBOLGRAPHGEN_DECLARATIONFRAGMENTPRINTER_H

#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/Support/JSON.h"
#include "nextcode/AST/ASTPrinter.h"
#include "nextcode/Basic/LLVM.h"

namespace nextcode {

class Decl;
class Type;
class TypeDecl;
class GenericTypeDecl;

namespace symbolgraphgen {

struct SymbolGraph;

/// Prints AST nodes as a stream of tagged fragments for syntax highlighting.
///
/// These fragments are meant to display a somewhat abbreviated part of the
/// declaration for display in documentation, ignoring things like member and
/// function bodies.
///
/// For example, a function:
///
/// ```nextcode
/// func foo() {
///   print("Hello, world!")
/// }
/// ```
///
/// Will have fragments representing the `func foo()` part.
class DeclarationFragmentPrinter : public ASTPrinter {
public:
  enum class FragmentKind {
    None,
    Keyword,
    Attribute,
    NumberLiteral,
    StringLiteral,
    Identifier,
    TypeIdentifier,
    GenericParameter,
    ExternalParam,
    InternalParam,
    Text,
  };
private:
  /// The symbol graph for which a declaration is being printed.
  const SymbolGraph *SG;

  /// The output stream to print fragment objects to.
  llvm::json::OStream &OS;

  /// The current fragment being considered.
  FragmentKind Kind;

  /// The actual source text of the fragment.
  SmallString<256> Spelling;

  SmallString<256> USR;

  /// Stores the set of decls referenced in the fragment if non-null.
  SmallPtrSet<const Decl*, 8> *ReferencedDecls;

  StringRef getKindSpelling(FragmentKind Kind) const;

  /// Open a new kind of fragment without committing its spelling.
  void openFragment(FragmentKind Kind);

  /// Close the current fragment if there is one, and commit it for display.
  void closeFragment();

  unsigned NumFragments;

public:
  DeclarationFragmentPrinter(
      const SymbolGraph *SG, llvm::json::OStream &OS,
      std::optional<StringRef> Key = std::nullopt,
      SmallPtrSet<const Decl *, 8> *ReferencedDecls = nullptr)
      : SG(SG), OS(OS), Kind(FragmentKind::None),
        ReferencedDecls(ReferencedDecls), NumFragments(0) {
    if (Key) {
      OS.attributeBegin(*Key);
      OS.arrayBegin();
    } else {
      OS.arrayBegin();
    }
  }

  /// Print an abridged form of a nominal type declaration, as:
  /// keyword text(" ") typeIdentifier.
  ///
  /// Subheadings for types don't include the complete declaration line
  /// including generics and inheritance.
  ///
  /// \param TD The type declaration to print.
  /// \param PrintKeyword Print the corresponding keyword introducer if `true`.
  void printAbridgedType(const GenericTypeDecl *TD, bool PrintKeyword);

  void printDeclLoc(const Decl *D) override;

  void printDeclNameEndLoc(const Decl *D) override {
    closeFragment();
  }

  void printNamePre(PrintNameContext Context) override;

  void printStructurePre(PrintStructureKind Kind, const Decl *D) override;

  void printNamePost(PrintNameContext Context) override {
    closeFragment();
  }

  void printTypeRef(Type T, const TypeDecl *RefTo, Identifier Name,
                    PrintNameContext NameContext) override;

  /// Print plain text to the current fragment, opening a new text fragment
  /// if there isn't an open fragment.
  void printText(StringRef Text) override;

  ~DeclarationFragmentPrinter() {
    closeFragment();
    OS.arrayEnd();
    OS.attributeEnd();
    assert(NumFragments);
  }
};

} // end namespace symbolgraphgen
} // end namespace nextcode

#endif // NEXTCODE_SYMBOLGRAPHGEN_DECLARATIONFRAGMENTPRINTER_H
