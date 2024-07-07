//===--- ASTGen.h -----------------------------------------------*- C++ -*-===//
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

#include "nextcode/AST/ASTBridging.h"
#include "nextcode/Parse/ParseBridging.h"

#ifdef __cplusplus
extern "C" {
#endif

void *_Nonnull nextcode_ASTGen_createQueuedDiagnostics();
void nextcode_ASTGen_destroyQueuedDiagnostics(void *_Nonnull queued);
void nextcode_ASTGen_addQueuedSourceFile(
    void *_Nonnull queuedDiagnostics, ssize_t bufferID,
    void *_Nonnull sourceFile, const uint8_t *_Nonnull displayNamePtr,
    intptr_t displayNameLength, ssize_t parentID, ssize_t positionInParent);
void nextcode_ASTGen_addQueuedDiagnostic(
    void *_Nonnull queued, const char *_Nonnull text, ptrdiff_t textLength,
    BridgedDiagnosticSeverity severity, const void *_Nullable sourceLoc,
    const void *_Nullable *_Nullable highlightRanges,
    ptrdiff_t numHighlightRanges);
void nextcode_ASTGen_renderQueuedDiagnostics(
    void *_Nonnull queued, ssize_t contextSize, ssize_t colorize,
    BridgedStringRef *_Nonnull renderedString);

// FIXME: Hack because we cannot easily get to the already-parsed source
// file from here. Fix this egregious oversight!
void *_Nullable nextcode_ASTGen_parseSourceFile(const char *_Nonnull buffer,
                                             size_t bufferLength,
                                             const char *_Nonnull moduleName,
                                             const char *_Nonnull filename,
                                             void *_Nullable ctx);
void nextcode_ASTGen_destroySourceFile(void *_Nonnull sourceFile);

/// Check whether the given source file round-trips correctly. Returns 0 if
/// round-trip succeeded, non-zero otherwise.
int nextcode_ASTGen_roundTripCheck(void *_Nonnull sourceFile);

/// Emit parser diagnostics for given source file.. Returns non-zero if any
/// diagnostics were emitted.
int nextcode_ASTGen_emitParserDiagnostics(
    void *_Nonnull diagEngine, void *_Nonnull sourceFile, int emitOnlyErrors,
    int downgradePlaceholderErrorsToWarnings);

// Build AST nodes for the top-level entities in the syntax.
void nextcode_ASTGen_buildTopLevelASTNodes(
    BridgedDiagnosticEngine diagEngine, void *_Nonnull sourceFile,
    BridgedDeclContext declContext, BridgedASTContext astContext,
    BridgedLegacyParser legacyParser, void *_Nonnull outputContext,
    void (*_Nonnull)(void *_Nonnull, void *_Nonnull));

void nextcode_ASTGen_freeBridgedString(BridgedStringRef);

void *_Nonnull nextcode_ASTGen_resolveExternalMacro(
    const char *_Nonnull moduleName, const char *_Nonnull typeName,
    void *_Nonnull opaquePluginHandle);
void nextcode_ASTGen_destroyExternalMacro(void *_Nonnull macro);

bool nextcode_ASTGen_checkDefaultArgumentMacroExpression(
    void *_Nonnull diagEngine, void *_Nonnull sourceFile,
    const void *_Nonnull macroSourceLocation);

ptrdiff_t nextcode_ASTGen_checkMacroDefinition(
    void *_Nonnull diagEngine, BridgedStringRef sourceFileBuffer,
    BridgedStringRef macroDeclText,
    BridgedStringRef *_Nonnull expansionSourceOutPtr,
    ptrdiff_t *_Nullable *_Nonnull replacementsPtr,
    ptrdiff_t *_Nonnull numReplacements,
    ptrdiff_t *_Nullable *_Nonnull genericReplacementsPtr,
    ptrdiff_t *_Nonnull numGenericReplacements);
void nextcode_ASTGen_freeExpansionReplacements(
    ptrdiff_t *_Nullable replacementsPtr, ptrdiff_t numReplacements);

ptrdiff_t nextcode_ASTGen_expandFreestandingMacro(
    void *_Nonnull diagEngine, const void *_Nonnull macro,
    const char *_Nonnull discriminator, uint8_t rawMacroRole,
    void *_Nonnull sourceFile, const void *_Nullable sourceLocation,
    BridgedStringRef *_Nonnull evaluatedSourceOut);

ptrdiff_t nextcode_ASTGen_expandAttachedMacro(
    void *_Nonnull diagEngine, const void *_Nonnull macro,
    const char *_Nonnull discriminator, const char *_Nonnull qualifiedType,
    const char *_Nonnull conformances, uint8_t rawMacroRole,
    void *_Nonnull customAttrSourceFile,
    const void *_Nullable customAttrSourceLocation,
    void *_Nonnull declarationSourceFile,
    const void *_Nullable declarationSourceLocation,
    void *_Nullable parentDeclSourceFile,
    const void *_Nullable parentDeclSourceLocation,
    BridgedStringRef *_Nonnull evaluatedSourceOut);

bool nextcode_ASTGen_initializePlugin(void *_Nonnull handle,
                                   void *_Nullable diagEngine);
void nextcode_ASTGen_deinitializePlugin(void *_Nonnull handle);
bool nextcode_ASTGen_pluginServerLoadLibraryPlugin(
    void *_Nonnull handle, const char *_Nonnull libraryPath,
    const char *_Nonnull moduleName, BridgedStringRef *_Nullable errorOut);

/// Build a TypeRepr for AST node for the type at the given source location in
/// the specified file.
nextcode::TypeRepr *_Nullable nextcode_ASTGen_buildTypeRepr(
    BridgedDiagnosticEngine diagEngine, void *_Nonnull sourceFile,
    BridgedSourceLoc sourceLoc, BridgedDeclContext declContext,
    BridgedASTContext astContext, BridgedLegacyParser legacyParser,
    BridgedSourceLoc *_Nonnull endSourceLoc);

/// Build a Decl for AST node for the type at the given source location in the
/// specified file.
nextcode::Decl *_Nullable nextcode_ASTGen_buildDecl(
    BridgedDiagnosticEngine diagEngine, void *_Nonnull sourceFile,
    BridgedSourceLoc sourceLoc, BridgedDeclContext declContext,
    BridgedASTContext astContext, BridgedLegacyParser legacyParser,
    BridgedSourceLoc *_Nonnull endSourceLoc);

/// Build a Expr for AST node for the type at the given source location in the
/// specified file.
nextcode::Expr *_Nullable nextcode_ASTGen_buildExpr(
    BridgedDiagnosticEngine diagEngine, void *_Nonnull sourceFile,
    BridgedSourceLoc sourceLoc, BridgedDeclContext declContext,
    BridgedASTContext astContext, BridgedLegacyParser legacyParser,
    BridgedSourceLoc *_Nonnull endSourceLoc);

/// Build a Stmt for AST node for the type at the given source location in the
/// specified file.
nextcode::Stmt *_Nullable nextcode_ASTGen_buildStmt(
    BridgedDiagnosticEngine diagEngine, void *_Nonnull sourceFile,
    BridgedSourceLoc sourceLoc, BridgedDeclContext declContext,
    BridgedASTContext astContext, BridgedLegacyParser legacyParser,
    BridgedSourceLoc *_Nonnull endSourceLoc);

// MARK: - Regex parsing

bool nextcode_ASTGen_lexRegexLiteral(const char *_Nonnull *_Nonnull curPtrPtr,
                                  const char *_Nonnull bufferEndPtr,
                                  bool mustBeRegex,
                                  BridgedNullableDiagnosticEngine diagEngine);

bool nextcode_ASTGen_parseRegexLiteral(BridgedStringRef inputPtr,
                                    size_t *_Nonnull versionOut,
                                    void *_Nonnull UnsafeMutableRawPointer,
                                    size_t captureStructureSize,
                                    BridgedSourceLoc diagLoc,
                                    BridgedDiagnosticEngine diagEngine);

#ifdef __cplusplus
}
#endif
