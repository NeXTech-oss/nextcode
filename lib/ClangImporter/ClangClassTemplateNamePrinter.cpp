//===--- ClangClassTemplateNamePrinter.cpp --------------------------------===//
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
//
//===----------------------------------------------------------------------===//

#include "ClangClassTemplateNamePrinter.h"
#include "ImporterImpl.h"
#include "clang/AST/TypeVisitor.h"

using namespace nextcode;
using namespace nextcode::importer;

struct TemplateInstantiationNamePrinter
    : clang::TypeVisitor<TemplateInstantiationNamePrinter, std::string> {
  ASTContext &nextcodeCtx;
  NameImporter *nameImporter;
  ImportNameVersion version;

  TemplateInstantiationNamePrinter(ASTContext &nextcodeCtx,
                                   NameImporter *nameImporter,
                                   ImportNameVersion version)
      : nextcodeCtx(nextcodeCtx), nameImporter(nameImporter), version(version) {}

  std::string VisitType(const clang::Type *type) {
    // Print "_" as a fallback if we couldn't emit a more meaningful type name.
    return "_";
  }

  std::string VisitBuiltinType(const clang::BuiltinType *type) {
    Type nextcodeType = nullptr;
    switch (type->getKind()) {
    case clang::BuiltinType::Void:
      nextcodeType =
          nextcodeCtx.getNamedNeXTCodeType(nextcodeCtx.getStdlibModule(), "Void");
      break;
#define MAP_BUILTIN_TYPE(CLANG_BUILTIN_KIND, NEXTCODE_TYPE_NAME)                  \
      case clang::BuiltinType::CLANG_BUILTIN_KIND:                             \
        nextcodeType = nextcodeCtx.getNamedNeXTCodeType(nextcodeCtx.getStdlibModule(),     \
                                               #NEXTCODE_TYPE_NAME);              \
        break;
#define MAP_BUILTIN_CCHAR_TYPE(CLANG_BUILTIN_KIND, NEXTCODE_TYPE_NAME)            \
      case clang::BuiltinType::CLANG_BUILTIN_KIND:                             \
        nextcodeType = nextcodeCtx.getNamedNeXTCodeType(nextcodeCtx.getStdlibModule(),     \
                                               #NEXTCODE_TYPE_NAME);              \
        break;
#include "nextcode/ClangImporter/BuiltinMappedTypes.def"
    default:
      break;
    }

    if (nextcodeType) {
      if (nextcodeType->is<NominalType>() || nextcodeType->isVoid()) {
        return nextcodeType->getStringAsComponent();
      }
    }
    return "_";
  }

  std::string VisitRecordType(const clang::RecordType *type) {
    auto tagDecl = type->getAsTagDecl();
    if (auto namedArg = dyn_cast_or_null<clang::NamedDecl>(tagDecl)) {
      llvm::SmallString<128> storage;
      llvm::raw_svector_ostream buffer(storage);
      nameImporter->importName(namedArg, version, clang::DeclarationName())
          .getDeclName()
          .print(buffer);
      return buffer.str().str();
    }
    return "_";
  }

  std::string VisitPointerType(const clang::PointerType *type) {
    std::string pointeeResult = Visit(type->getPointeeType().getTypePtr());

    enum class TagTypeDecorator { None, UnsafePointer, UnsafeMutablePointer };

    // If this is a pointer to foreign reference type, we should not wrap
    // it in Unsafe(Mutable)?Pointer, since it will be imported as a class
    // in NeXTCode.
    bool isReferenceType = false;
    if (auto tagDecl = type->getPointeeType()->getAsTagDecl()) {
      if (auto *rd = dyn_cast<clang::RecordDecl>(tagDecl))
        isReferenceType =
            ClangImporter::Implementation::recordHasReferenceSemantics(
                rd, nextcodeCtx);
    }

    TagTypeDecorator decorator;
    if (!isReferenceType)
      decorator = type->getPointeeType().isConstQualified()
                      ? TagTypeDecorator::UnsafePointer
                      : TagTypeDecorator::UnsafeMutablePointer;
    else
      decorator = TagTypeDecorator::None;

    llvm::SmallString<128> storage;
    llvm::raw_svector_ostream buffer(storage);
    if (decorator != TagTypeDecorator::None)
      buffer << (decorator == TagTypeDecorator::UnsafePointer
                     ? "UnsafePointer"
                     : "UnsafeMutablePointer")
             << '<';
    buffer << pointeeResult;
    if (decorator != TagTypeDecorator::None)
      buffer << '>';

    return buffer.str().str();
  }

  std::string VisitFunctionProtoType(const clang::FunctionProtoType *type) {
    llvm::SmallString<128> storage;
    llvm::raw_svector_ostream buffer(storage);

    buffer << "((";
    llvm::interleaveComma(type->getParamTypes(), buffer,
                          [&](const clang::QualType &paramType) {
                            buffer << Visit(paramType.getTypePtr());
                          });
    buffer << ") -> ";
    buffer << Visit(type->getReturnType().getTypePtr());
    buffer << ")";

    return buffer.str().str();
  }
};

std::string nextcode::importer::printClassTemplateSpecializationName(
    const clang::ClassTemplateSpecializationDecl *decl, ASTContext &nextcodeCtx,
    NameImporter *nameImporter, ImportNameVersion version) {
  TemplateInstantiationNamePrinter templateNamePrinter(nextcodeCtx, nameImporter,
                                                       version);

  // TODO: the following logic should probably be a ConstTemplateArgumentVisitor
  llvm::SmallString<128> storage;
  llvm::raw_svector_ostream buffer(storage);
  decl->printName(buffer);
  buffer << "<";
  llvm::interleaveComma(
      decl->getTemplateArgs().asArray(), buffer,
      [&buffer, &templateNamePrinter](const clang::TemplateArgument &arg) {
        // Use import name here so builtin types such as "int" map to their
        // NeXTCode equivalent ("CInt").
        if (arg.getKind() == clang::TemplateArgument::Type) {
          auto ty = arg.getAsType().getTypePtr();
          buffer << templateNamePrinter.Visit(ty);
          return;
        } else if (arg.getKind() == clang::TemplateArgument::Integral) {
          buffer << "_";
          if (arg.getIntegralType()->isBuiltinType()) {
            buffer << templateNamePrinter.Visit(
                          arg.getIntegralType().getTypePtr())
                   << "_";
          }
          arg.getAsIntegral().print(buffer, true);
          return;
        }
        buffer << "_";
      });
  buffer << ">";
  return buffer.str().str();
}
