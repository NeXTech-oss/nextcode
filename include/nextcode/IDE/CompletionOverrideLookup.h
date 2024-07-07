//===--- CompletionOverrideLookup.h ---------------------------------------===//
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

#ifndef NEXTCODE_IDE_COMPLETION_OVERRIDE_LOOKUP_H
#define NEXTCODE_IDE_COMPLETION_OVERRIDE_LOOKUP_H

#include "nextcode/AST/NameLookup.h"
#include "nextcode/IDE/CodeCompletionResultSink.h"
#include "nextcode/Sema/IDETypeChecking.h"
#include "nextcode/Parse/Token.h"

namespace nextcode {
namespace ide {

class CompletionOverrideLookup : public nextcode::VisibleDeclConsumer {
  CodeCompletionResultSink &Sink;
  ASTContext &Ctx;
  const DeclContext *CurrDeclContext;
  SmallVectorImpl<StringRef> &ParsedKeywords;
  SourceLoc introducerLoc;

  bool hasFuncIntroducer = false;
  bool hasVarIntroducer = false;
  bool hasTypealiasIntroducer = false;
  bool hasInitializerModifier = false;
  bool hasAccessModifier = false;
  bool hasOverride = false;
  bool hasOverridabilityModifier = false;
  bool hasStaticOrClass = false;

public:
  CompletionOverrideLookup(CodeCompletionResultSink &Sink, ASTContext &Ctx,
                           const DeclContext *CurrDeclContext,
                           SmallVectorImpl<StringRef> &ParsedKeywords,
                           SourceLoc introducerLoc)
      : Sink(Sink), Ctx(Ctx), CurrDeclContext(CurrDeclContext),
        ParsedKeywords(ParsedKeywords), introducerLoc(introducerLoc) {
    hasFuncIntroducer = isKeywordSpecified("function");
    hasVarIntroducer = isKeywordSpecified("var") ||
                       isKeywordSpecified("let");
    hasTypealiasIntroducer = isKeywordSpecified("typealias");
    hasInitializerModifier = isKeywordSpecified("required") ||
                             isKeywordSpecified("convenience");
    hasAccessModifier = isKeywordSpecified("private") ||
                        isKeywordSpecified("fileprivate") ||
                        isKeywordSpecified("internal") ||
                        isKeywordSpecified("package") ||
                        isKeywordSpecified("public") ||
                        isKeywordSpecified("open");
    hasOverride = isKeywordSpecified("override");
    hasOverridabilityModifier =
        isKeywordSpecified("final") || isKeywordSpecified("open");
    hasStaticOrClass = isKeywordSpecified(getTokenText(tok::kw_class)) ||
                       isKeywordSpecified(getTokenText(tok::kw_static));
  }

  bool isKeywordSpecified(StringRef Word) {
    return std::find(ParsedKeywords.begin(), ParsedKeywords.end(), Word) !=
           ParsedKeywords.end();
  }

  bool missingOverride(DeclVisibilityKind Reason) {
    return !hasOverride && Reason == DeclVisibilityKind::MemberOfSuper &&
           !CurrDeclContext->getSelfProtocolDecl();
  }

  /// Add an access modifier (i.e. `public`) to \p Builder is necessary.
  /// Returns \c true if the modifier is actually added, \c false otherwise.
  bool addAccessControl(const ValueDecl *VD,
                        CodeCompletionResultBuilder &Builder);

  /// Return type if the result type if \p VD should be represented as opaque
  /// result type.
  Type getOpaqueResultType(const ValueDecl *VD, DeclVisibilityKind Reason,
                           DynamicLookupInfo dynamicLookupInfo);

  void addValueOverride(const ValueDecl *VD, DeclVisibilityKind Reason,
                        DynamicLookupInfo dynamicLookupInfo,
                        CodeCompletionResultBuilder &Builder,
                        bool hasDeclIntroducer);

  void addMethodOverride(const FuncDecl *FD, DeclVisibilityKind Reason,
                         DynamicLookupInfo dynamicLookupInfo);

  void addVarOverride(const VarDecl *VD, DeclVisibilityKind Reason,
                      DynamicLookupInfo dynamicLookupInfo);

  void addSubscriptOverride(const SubscriptDecl *SD, DeclVisibilityKind Reason,
                            DynamicLookupInfo dynamicLookupInfo);

  void addTypeAlias(const AssociatedTypeDecl *ATD, DeclVisibilityKind Reason,
                    DynamicLookupInfo dynamicLookupInfo);

  void addConstructor(const ConstructorDecl *CD, DeclVisibilityKind Reason,
                      DynamicLookupInfo dynamicLookupInfo);

  // Implement nextcode::VisibleDeclConsumer.
  void foundDecl(ValueDecl *D, DeclVisibilityKind Reason,
                 DynamicLookupInfo dynamicLookupInfo) override;

  void addDesignatedInitializers(NominalTypeDecl *NTD);

  void addAssociatedTypes(NominalTypeDecl *NTD);

  static StringRef
  getResultBuilderDocComment(ResultBuilderBuildFunction function);

  void addResultBuilderBuildCompletion(NominalTypeDecl *builder,
                                       Type componentType,
                                       ResultBuilderBuildFunction function);

  /// Add completions for the various "build" functions in a result builder.
  void addResultBuilderBuildCompletions(NominalTypeDecl *builder);

  void getOverrideCompletions(SourceLoc Loc);
};

} // end namespace ide
} // end namespace nextcode

#endif // NEXTCODE_IDE_COMPLETION_OVERRIDE_LOOKUP_H