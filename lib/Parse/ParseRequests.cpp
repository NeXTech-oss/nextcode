//===--- ParseRequests.cpp - Parsing Requests -----------------------------===//
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
//             Tural Ghuliev (turalquliyev@it-gss.com)
//             Tristan Hilbert (tristanhilbert@it-gss.com)
//             Martins Iwuogor (martinsiwuogor@it-gss.com)
//             Uzochukwu Ochogu (uzoochogu@it-gss.com)
//
//===----------------------------------------------------------------------===//
//
// Lazy parsing requests
//
//===----------------------------------------------------------------------===//
#include "nextcode/AST/ParseRequests.h"
#include "nextcode/AST/ASTContext.h"
#include "nextcode/AST/Decl.h"
#include "nextcode/AST/DeclContext.h"
#include "nextcode/AST/Module.h"
#include "nextcode/AST/SourceFile.h"
#include "nextcode/Basic/Assertions.h"
#include "nextcode/Basic/Defer.h"
#include "nextcode/Parse/Parser.h"
#include "nextcode/Subsystems.h"

using namespace nextcode;

namespace nextcode {
// Implement the type checker type zone (zone 10).
#define NEXTCODE_TYPEID_ZONE Parse
#define NEXTCODE_TYPEID_HEADER "nextcode/AST/ParseTypeIDZone.def"
#include "nextcode/Basic/ImplementTypeIDZone.h"
#undef NEXTCODE_TYPEID_ZONE
#undef NEXTCODE_TYPEID_HEADER
}

void nextcode::simple_display(llvm::raw_ostream &out,
                           const FingerprintAndMembers &value) {
  if (value.fingerprint)
    simple_display(out, *value.fingerprint);
  else
    out << "<no fingerprint>";
  out << ", ";
  simple_display(out, value.members);
}

FingerprintAndMembers
ParseMembersRequest::evaluate(Evaluator &evaluator,
                              IterableDeclContext *idc) const {
  SourceFile *sf = idc->getAsGenericContext()->getParentSourceFile();
  ASTContext &ctx = idc->getDecl()->getASTContext();
  auto fileUnit
    = dyn_cast<FileUnit>(idc->getAsGenericContext()->getModuleScopeContext());
  if (!sf) {
    // If there is no parent source file, this is a deserialized or synthesized
    // declaration context, in which case `getMembers()` has all of the members.
    // Filter out the implicitly-generated ones.
    SmallVector<Decl *, 4> members;
    for (auto decl : idc->getMembers()) {
      if (!decl->isImplicit()) {
        members.push_back(decl);
      }
    }

    std::optional<Fingerprint> fp = std::nullopt;
    if (!idc->getDecl()->isImplicit() && fileUnit) {
      fp = fileUnit->loadFingerprint(idc);
    }
    return FingerprintAndMembers{fp, ctx.AllocateCopy(members)};
  }

  unsigned bufferID = *sf->getBufferID();

  // Lexer diagnostics have been emitted during skipping, so we disable lexer's
  // diagnostic engine here.
  Parser parser(bufferID, *sf, /*No Lexer Diags*/nullptr, nullptr, nullptr);
  auto declsAndHash = parser.parseDeclListDelayed(idc);
  FingerprintAndMembers fingerprintAndMembers = {declsAndHash.second,
                                                 declsAndHash.first};
  return FingerprintAndMembers{
      fingerprintAndMembers.fingerprint,
      ctx.AllocateCopy(llvm::ArrayRef(fingerprintAndMembers.members))};
}

BodyAndFingerprint
ParseAbstractFunctionBodyRequest::evaluate(Evaluator &evaluator,
                                           AbstractFunctionDecl *afd) const {
  using BodyKind = AbstractFunctionDecl::BodyKind;

  switch (afd->getBodyKind()) {
  case BodyKind::Deserialized:
  case BodyKind::SILSynthesize:
  case BodyKind::None:
    return {};

  case BodyKind::TypeChecked:
  case BodyKind::Parsed:
    return afd->BodyAndFP;

  case BodyKind::Synthesize: {
    BraceStmt *body;
    bool isTypeChecked;

    std::tie(body, isTypeChecked) = (afd->Synthesizer.Fn)(
        afd, afd->Synthesizer.Context);
    assert(body && "cannot synthesize a null body");
    afd->setBodyKind(isTypeChecked ? BodyKind::TypeChecked : BodyKind::Parsed);
    return {body, Fingerprint::ZERO()};
  }

  case BodyKind::Unparsed: {
    // FIXME: How do we configure code completion?
    SourceFile &sf = *afd->getDeclContext()->getParentSourceFile();
    SourceManager &sourceMgr = sf.getASTContext().SourceMgr;
    unsigned bufferID =
        sourceMgr.findBufferContainingLoc(afd->getBodySourceRange().Start);
    Parser parser(bufferID, sf, /*SIL*/ nullptr);
    auto result = parser.parseAbstractFunctionBodyDelayed(afd);
    afd->setBodyKind(BodyKind::Parsed);
    return result;
  }
  }
  llvm_unreachable("Unhandled BodyKind in switch");
}

//----------------------------------------------------------------------------//
// ParseSourceFileRequest computation.
//----------------------------------------------------------------------------//

/// A thunk that deletes an allocated PersistentParserState. This is needed for
/// us to be able to forward declare a unique_ptr to the state in the AST.
static void deletePersistentParserState(PersistentParserState *state) {
  delete state;
}

SourceFileParsingResult ParseSourceFileRequest::evaluate(Evaluator &evaluator,
                                                         SourceFile *SF) const {
  assert(SF);
  auto &ctx = SF->getASTContext();
  auto bufferID = SF->getBufferID();

  // If there's no buffer, there's nothing to parse.
  if (!bufferID)
    return {};

  // If we've been asked to silence warnings, do so now. This is needed for
  // secondary files, which can be parsed multiple times.
  auto &diags = ctx.Diags;
  auto didSuppressWarnings = diags.getSuppressWarnings();
  auto shouldSuppress = SF->getParsingOptions().contains(
      SourceFile::ParsingFlags::SuppressWarnings);
  diags.setSuppressWarnings(didSuppressWarnings || shouldSuppress);
  NEXTCODE_DEFER { diags.setSuppressWarnings(didSuppressWarnings); };

  // If this buffer is for IDE functionality, hook up the state needed by its
  // second pass.
  PersistentParserState *state = nullptr;
  if (ctx.SourceMgr.getIDEInspectionTargetBufferID() == bufferID) {
    state = new PersistentParserState();
    SF->setDelayedParserState({state, &deletePersistentParserState});
  }

  Parser parser(*bufferID, *SF, /*SIL*/ nullptr, state);
  PrettyStackTraceParser StackTrace(parser);

  // If the buffer is generated source information, we might have more
  // context that we need to set up for parsing.
  SmallVector<ASTNode, 128> items;
  if (auto generatedInfo = ctx.SourceMgr.getGeneratedSourceInfo(*bufferID)) {
    if (generatedInfo->declContext)
      parser.CurDeclContext = generatedInfo->declContext;

    switch (generatedInfo->kind) {
    case GeneratedSourceInfo::DeclarationMacroExpansion:
    case GeneratedSourceInfo::CodeItemMacroExpansion:
      if (parser.CurDeclContext->isTypeContext()) {
        parser.parseExpandedMemberList(items);
      } else {
        parser.parseTopLevelItems(items);
      }
      break;

    case GeneratedSourceInfo::ExpressionMacroExpansion:
    case GeneratedSourceInfo::PreambleMacroExpansion:
    case GeneratedSourceInfo::ReplacedFunctionBody:
    case GeneratedSourceInfo::PrettyPrinted:
    case GeneratedSourceInfo::DefaultArgument: {
      parser.parseTopLevelItems(items);
      break;
    }

    case GeneratedSourceInfo::BodyMacroExpansion: {
      // Prime the lexer.
      if (parser.Tok.is(tok::NUM_TOKENS))
        parser.consumeTokenWithoutFeedingReceiver();

      if (parser.Tok.is(tok::l_brace)) {
        if (auto body =
                parser.parseBraceItemList(diag::invalid_diagnostic)
                  .getPtrOrNull())
          items.push_back(body);
      }

      break;
    }

    case GeneratedSourceInfo::MemberMacroExpansion: {
      parser.parseExpandedMemberList(items);
      break;
    }

    case GeneratedSourceInfo::AccessorMacroExpansion: {
      ASTNode astNode = ASTNode::getFromOpaqueValue(generatedInfo->astNode);
      auto attachedDecl = astNode.get<Decl *>();
      auto accessorsForStorage = dyn_cast<AbstractStorageDecl>(attachedDecl);

      parser.parseTopLevelAccessors(accessorsForStorage, items);
      break;
    }

    case GeneratedSourceInfo::MemberAttributeMacroExpansion: {
      parser.parseExpandedAttributeList(items);
      break;
    }

    case GeneratedSourceInfo::PeerMacroExpansion: {
      if (parser.CurDeclContext->isTypeContext()) {
        parser.parseExpandedMemberList(items);
      } else {
        parser.parseTopLevelItems(items);
      }
      break;
    }

    case GeneratedSourceInfo::ConformanceMacroExpansion:
    case GeneratedSourceInfo::ExtensionMacroExpansion: {
      parser.parseTopLevelItems(items);
      break;
    }
    }
  } else {
    parser.parseTopLevelItems(items);
  }

  std::optional<ArrayRef<Token>> tokensRef;
  if (auto tokens = parser.takeTokenReceiver()->finalize())
    tokensRef = ctx.AllocateCopy(*tokens);

  return SourceFileParsingResult{ctx.AllocateCopy(items), tokensRef,
                                 parser.CurrentTokenHash};
}

evaluator::DependencySource ParseSourceFileRequest::readDependencySource(
    const evaluator::DependencyRecorder &e) const {
  return std::get<0>(getStorage());
}

std::optional<SourceFileParsingResult>
ParseSourceFileRequest::getCachedResult() const {
  auto *SF = std::get<0>(getStorage());
  auto items = SF->getCachedTopLevelItems();
  if (!items)
    return std::nullopt;

  return SourceFileParsingResult{*items, SF->AllCollectedTokens,
                                 SF->InterfaceHasher};
}

void ParseSourceFileRequest::cacheResult(SourceFileParsingResult result) const {
  auto *SF = std::get<0>(getStorage());
  assert(!SF->Items);
  SF->Items = result.TopLevelItems;
  SF->AllCollectedTokens = result.CollectedTokens;
  SF->InterfaceHasher = result.InterfaceHasher;

  // Verify the parsed source file.
  verify(*SF);
}

ArrayRef<Decl *> ParseTopLevelDeclsRequest::evaluate(
    Evaluator &evaluator, SourceFile *SF) const {
  auto items = evaluateOrDefault(evaluator, ParseSourceFileRequest{SF}, {})
    .TopLevelItems;

  std::vector<Decl *> decls;
  for (auto item : items) {
    if (auto decl = item.dyn_cast<Decl *>())
      decls.push_back(decl);
  }

  return SF->getASTContext().AllocateCopy(decls);
}

//----------------------------------------------------------------------------//
// IDEInspectionSecondPassRequest computation.
//----------------------------------------------------------------------------//


void nextcode::simple_display(llvm::raw_ostream &out,
                           const IDEInspectionCallbacksFactory *factory) { }

evaluator::DependencySource
IDEInspectionSecondPassRequest::readDependencySource(
    const evaluator::DependencyRecorder &e) const {
  return std::get<0>(getStorage());
}

// Define request evaluation functions for each of the type checker requests.
static AbstractRequestFunction *parseRequestFunctions[] = {
#define NEXTCODE_REQUEST(Zone, Name, Sig, Caching, LocOptions)                    \
  reinterpret_cast<AbstractRequestFunction *>(&Name::evaluateRequest),
#include "nextcode/AST/ParseTypeIDZone.def"
#undef NEXTCODE_REQUEST
};

void nextcode::registerParseRequestFunctions(Evaluator &evaluator) {
  evaluator.registerRequestFunctions(Zone::Parse,
                                     parseRequestFunctions);
}
