//===--- Symbol.h- Symbol Graph Node --------------------------------------===//
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

#ifndef NEXTCODE_SYMBOLGRAPHGEN_SYMBOL_H
#define NEXTCODE_SYMBOLGRAPHGEN_SYMBOL_H

#include "llvm/Support/JSON.h"
#include "nextcode/AST/Attr.h"
#include "nextcode/AST/Type.h"
#include "nextcode/Basic/LLVM.h"
#include "nextcode/Markup/Markup.h"
#include "nextcode/SymbolGraphGen/PathComponent.h"
#include "nextcode/SymbolGraphGen/FragmentInfo.h"

namespace nextcode {
namespace symbolgraphgen {

struct Availability;
struct SymbolGraphASTWalker;
struct SymbolGraph;

/// A symbol from a module: a node in a graph.
class Symbol {
  /// The symbol graph in which this symbol resides.
  SymbolGraph *Graph;
  /// Either a ValueDecl* or ExtensionDecl*.
  const Decl *D;
  Type BaseType;
  const NominalTypeDecl *SynthesizedBaseTypeDecl;

  Symbol(SymbolGraph *Graph, const ValueDecl *VD, const ExtensionDecl *ED,
         const NominalTypeDecl *SynthesizedBaseTypeDecl,
         Type BaseTypeForSubstitution = Type());

  nextcode::DeclName getName(const Decl *D) const;

  void serializeKind(StringRef Identifier, StringRef DisplayName,
                     llvm::json::OStream &OS) const;

  void serializeKind(llvm::json::OStream &OS) const;

  void serializeIdentifier(llvm::json::OStream &OS) const;

  void serializePathComponents(llvm::json::OStream &OS) const;

  void serializeNames(llvm::json::OStream &OS) const;

  void serializePosition(StringRef Key, SourceLoc Loc,
                         SourceManager &SourceMgr,
                         llvm::json::OStream &OS) const;

  void serializeRange(size_t InitialIndentation,
                      SourceRange Range, SourceManager &SourceMgr,
                      llvm::json::OStream &OS) const;

  void serializeDocComment(llvm::json::OStream &OS) const;

  void serializeFunctionSignature(llvm::json::OStream &OS) const;

  void serializeGenericParam(const nextcode::GenericTypeParamType &Param,
                             llvm::json::OStream &OS) const;

  void serializeNeXTCodeGenericMixin(llvm::json::OStream &OS) const;

  void serializeNeXTCodeExtensionMixin(llvm::json::OStream &OS) const;

  void serializeDeclarationFragmentMixin(llvm::json::OStream &OS) const;

  void serializeAccessLevelMixin(llvm::json::OStream &OS) const;

  void serializeMetadataMixin(llvm::json::OStream &OS) const;

  void serializeLocationMixin(llvm::json::OStream &OS) const;

  void serializeAvailabilityMixin(llvm::json::OStream &OS) const;

  void serializeSPIMixin(llvm::json::OStream &OS) const;

public:
  Symbol(SymbolGraph *Graph, const ExtensionDecl *ED,
         const NominalTypeDecl *SynthesizedBaseTypeDecl,
         Type BaseTypeForSubstitution = Type());

  Symbol(SymbolGraph *Graph, const ValueDecl *VD,
         const NominalTypeDecl *SynthesizedBaseTypeDecl,
         Type BaseTypeForSubstitution = Type());

  void serialize(llvm::json::OStream &OS) const;

  const SymbolGraph *getGraph() const {
    return Graph;
  }

  const ValueDecl *getSymbolDecl() const;

  const Decl *getLocalSymbolDecl() const { return D; }

  Type getBaseType() const {
    return BaseType;
  }

  const NominalTypeDecl *getSynthesizedBaseTypeDecl() const {
    return SynthesizedBaseTypeDecl;
  }

  /// Retrieve the path components associated with this symbol, from outermost
  /// to innermost (this symbol).
  void getPathComponents(SmallVectorImpl<PathComponent> &Components) const;

  /// Retrieve information about all symbols referenced in the declaration
  /// fragment printed for this symbol.
  void getFragmentInfo(SmallVectorImpl<FragmentInfo> &FragmentInfo) const;

  /// Print the symbol path to an output stream.
  void printPath(llvm::raw_ostream &OS) const;

  void getUSR(SmallVectorImpl<char> &USR) const;
  
  /// If this symbol is inheriting docs from a parent class, protocol, or default
  /// implementation, returns that decl. Returns null if there are no docs or if
  /// the symbol has its own doc comments to render.
  const ValueDecl *getDeclInheritingDocs() const;

  /// If this symbol is an implementation of a protocol requirement for a
  /// protocol declared outside its module, returns the upstream decl for that
  /// requirement.
  const ValueDecl *getForeignProtocolRequirement() const;

  /// If this symbol is an implementation of a protocol requirement, returns the
  /// upstream decl for that requirement.
  const ValueDecl *getProtocolRequirement() const;

  /// If this symbol is a synthesized symbol or an implementation of a protocol
  /// requirement, returns the upstream decl.
  const ValueDecl *getInheritedDecl() const;

  static bool supportsKind(DeclKind Kind);

  /// Determines the effective access level of the given extension.
  ///
  /// The effective access level is defined as the minimum of:
  ///  - the maximum access level of a property or conformance
  ///  - the access level of the extended nominal
  ///
  /// The effective access level is defined this way so that the extension
  /// symbol's access level equals the highest access level of any of the
  /// symbols the extension symbol has a relationship to.
  ///
  /// This function is not logically equivalent to
  /// `ExtensionDecl.getMaxAccessLevel()`, which computes the maximum access
  /// level any of the `ExtensionDecl`'s members
  /// **can** have based on the extended type and types used in constraints.
  static AccessLevel getEffectiveAccessLevel(const ExtensionDecl *ED);

  /// Determines the kind of Symbol the given declaration produces and
  /// returns the respective symbol kind identifier and kind name.
  static std::pair<StringRef, StringRef> getKind(const Decl *D);
};

} // end namespace symbolgraphgen
} // end namespace nextcode

namespace llvm {
using Symbol = nextcode::symbolgraphgen::Symbol;
using SymbolGraph = nextcode::symbolgraphgen::SymbolGraph;

template <> struct DenseMapInfo<Symbol> {
  static inline Symbol getEmptyKey() {
    return Symbol {
      DenseMapInfo<SymbolGraph *>::getEmptyKey(),
      DenseMapInfo<const nextcode::ValueDecl *>::getEmptyKey(),
      DenseMapInfo<const nextcode::NominalTypeDecl *>::getTombstoneKey(),
      DenseMapInfo<nextcode::Type>::getEmptyKey(),
    };
  }
  static inline Symbol getTombstoneKey() {
    return Symbol {
      DenseMapInfo<SymbolGraph *>::getTombstoneKey(),
      DenseMapInfo<const nextcode::ValueDecl *>::getTombstoneKey(),
      DenseMapInfo<const nextcode::NominalTypeDecl *>::getTombstoneKey(),
      DenseMapInfo<nextcode::Type>::getTombstoneKey(),
    };
  }
  static unsigned getHashValue(const Symbol S) {
    unsigned H = 0;
    H ^= DenseMapInfo<SymbolGraph *>::getHashValue(S.getGraph());
    H ^=
        DenseMapInfo<const nextcode::Decl *>::getHashValue(S.getLocalSymbolDecl());
    H ^= DenseMapInfo<const nextcode::NominalTypeDecl *>::getHashValue(S.getSynthesizedBaseTypeDecl());
    H ^= DenseMapInfo<nextcode::Type>::getHashValue(S.getBaseType());
    return H;
  }
  static bool isEqual(const Symbol LHS, const Symbol RHS) {
    return LHS.getGraph() == RHS.getGraph() &&
           LHS.getLocalSymbolDecl() == RHS.getLocalSymbolDecl() &&
           LHS.getSynthesizedBaseTypeDecl() ==
               RHS.getSynthesizedBaseTypeDecl() &&
           DenseMapInfo<nextcode::Type>::isEqual(LHS.getBaseType(),
                                              RHS.getBaseType());
  }
};
} // end namespace llvm


#endif // NEXTCODE_SYMBOLGRAPHGEN_SYMBOL_H
