//===--- ConstTypeInfo.h - Const Nominal Type Info Structure ----*- C++ -*-===//
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

#ifndef NEXTCODE_AST_CONST_TYPE_INFO_H
#define NEXTCODE_AST_CONST_TYPE_INFO_H

#include "nextcode/AST/Attr.h"
#include "nextcode/AST/Type.h"
#include <memory>
#include <string>
#include <vector>

namespace nextcode {
class NominalTypeDecl;
class VarDecl;
class Type;

/// Representation of a compile-time-known value, for example
/// in a type property initializer expression
class CompileTimeValue {
public:
  enum ValueKind {
    RawLiteral,
    InitCall,
    Builder,
    Dictionary,
    Array,
    Tuple,
    Enum,
    Type,
    KeyPath,
    FunctionCall,
    MemberReference,
    InterpolatedString,
    Runtime
  };

  ValueKind getKind() const { return Kind; }

protected:
  CompileTimeValue(ValueKind ValueKind) : Kind(ValueKind) {}

private:
  ValueKind Kind;
};

/// A string representation of a raw literal value,
/// for example an integer or string or float literal.
class RawLiteralValue : public CompileTimeValue {
public:
  RawLiteralValue(std::string Value)
  : CompileTimeValue(ValueKind::RawLiteral), Value(Value) {}

  std::string getValue() const { return Value; }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::RawLiteral;
  }

private:
  std::string Value;
};

struct FunctionParameter {
  std::string Label;
  nextcode::Type Type;
  std::shared_ptr<CompileTimeValue> Value;
};

/// A representation of a call to a type's initializer
/// with a collection of (potentially compile-time-known) parameters
class InitCallValue : public CompileTimeValue {
public:
  InitCallValue(nextcode::Type Type, std::vector<FunctionParameter> Parameters)
      : CompileTimeValue(ValueKind::InitCall), Type(Type),
        Parameters(Parameters) {}

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::InitCall;
  }

  nextcode::Type getType() const { return Type; }
  std::vector<FunctionParameter> getParameters() const { return Parameters; }

private:
  nextcode::Type Type;
  std::vector<FunctionParameter> Parameters;
};

/// A representation of a Builder pattern initialization expression
class BuilderValue : public CompileTimeValue {
public:
  BuilderValue() : CompileTimeValue(ValueKind::Builder) {}

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Builder;
  }

private:
  std::vector<CompileTimeValue> Members;
};

struct TupleElement {
  std::optional<std::string> Label;
  nextcode::Type Type;
  std::shared_ptr<CompileTimeValue> Value;
};

/// A representation of a tuple and each tuple-element
class TupleValue : public CompileTimeValue {
public:
  TupleValue(std::vector<TupleElement> Elements)
      : CompileTimeValue(ValueKind::Tuple), Elements(Elements) {}

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Tuple;
  }

  std::vector<TupleElement> getElements() const { return Elements; }

private:
  std::vector<TupleElement> Elements;
};

/// An array literal value representation
class ArrayValue : public CompileTimeValue {
public:
  ArrayValue(std::vector<std::shared_ptr<CompileTimeValue>> Elements)
      : CompileTimeValue(ValueKind::Array), Elements(Elements) {}

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Array;
  }
  std::vector<std::shared_ptr<CompileTimeValue>> getElements() const {
    return Elements;
  }

private:
  std::vector<std::shared_ptr<CompileTimeValue>> Elements;
};

/// A dictionary literal value representation
class DictionaryValue : public CompileTimeValue {
public:
  DictionaryValue(std::vector<std::shared_ptr<TupleValue>> elements)
      : CompileTimeValue(ValueKind::Dictionary), Elements(elements) {}

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Dictionary;
  }

  std::vector<std::shared_ptr<TupleValue>> getElements() const {
    return Elements;
  }

private:
  std::vector<std::shared_ptr<TupleValue>> Elements;
};

/// An enum value representation
class EnumValue : public CompileTimeValue {
public:
  EnumValue(std::string Identifier,
            std::optional<std::vector<FunctionParameter>> Parameters)
      : CompileTimeValue(ValueKind::Enum), Identifier(Identifier),
        Parameters(Parameters) {}

  std::string getIdentifier() const { return Identifier; }
  std::optional<std::vector<FunctionParameter>> getParameters() const {
    return Parameters;
  }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Enum;
  }

private:
  std::string Identifier;
  std::optional<std::vector<FunctionParameter>> Parameters;
};

/// A type value representation
class TypeValue : public CompileTimeValue {
public:
  TypeValue(nextcode::Type Type) : CompileTimeValue(ValueKind::Type), Type(Type) {}

  nextcode::Type getType() const { return Type; }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Type;
  }

private:
  nextcode::Type Type;
};

/// A representation of a Keypath
class KeyPathValue : public CompileTimeValue {
public:
  struct Component {
    std::string Label;
    nextcode::Type Type;
  };
  KeyPathValue(std::string Path,
               nextcode::Type RootType,
               std::vector<Component> Components)
  : CompileTimeValue(ValueKind::KeyPath), Path(Path), RootType(RootType), Components(Components) {}

  std::string getPath() const { return Path; }
  nextcode::Type getRootType() const { return RootType; }
  std::vector<Component> getComponents() const {
    return Components;
  }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::KeyPath;
  }

private:
  std::string Path;
  nextcode::Type RootType;
  std::vector<Component> Components;
};

/// A function call representation. This is for a function declaration such as
/// let foo = bar(baz: "abc")
class FunctionCallValue : public CompileTimeValue {
public:
  FunctionCallValue(std::string Identifier,
                    std::optional<std::vector<FunctionParameter>> Parameters)
      : CompileTimeValue(ValueKind::FunctionCall), Identifier(Identifier),
        Parameters(Parameters) {}

  std::string getIdentifier() const { return Identifier; }
  std::optional<std::vector<FunctionParameter>> getParameters() const {
    return Parameters;
  }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::FunctionCall;
  }

private:
  std::string Identifier;
  std::optional<std::vector<FunctionParameter>> Parameters;
};

/// A member reference representation such as
/// let foo = MyStruct.bar
class MemberReferenceValue : public CompileTimeValue {
public:
  MemberReferenceValue(nextcode::Type BaseType, std::string MemberLabel)
      : CompileTimeValue(ValueKind::MemberReference), BaseType(BaseType),
        MemberLabel(MemberLabel) {}

  std::string getMemberLabel() const { return MemberLabel; }
  nextcode::Type getBaseType() const { return BaseType; }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::MemberReference;
  }

private:
  nextcode::Type BaseType;
  std::string MemberLabel;
};

/// A representation of an Interpolated String Literal
class InterpolatedStringLiteralValue : public CompileTimeValue {
public:
  InterpolatedStringLiteralValue(
      std::vector<std::shared_ptr<CompileTimeValue>> Segments)
      : CompileTimeValue(ValueKind::InterpolatedString), Segments(Segments) {}

  std::vector<std::shared_ptr<CompileTimeValue>> getSegments() const {
    return Segments;
  }

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::InterpolatedString;
  }

private:
  std::vector<std::shared_ptr<CompileTimeValue>> Segments;
};

/// A representation of an arbitrary value that does not fall under
/// any of the above categories.
class RuntimeValue : public CompileTimeValue {
public:
  RuntimeValue() : CompileTimeValue(ValueKind::Runtime) {}

  static bool classof(const CompileTimeValue *T) {
    return T->getKind() == ValueKind::Runtime;
  }
};

struct CustomAttrValue {
  const nextcode::CustomAttr *Attr;
  std::vector<FunctionParameter> Parameters;
};

/// A representation of a single associated value for an enumeration case.
struct EnumElementParameterValue {
  std::optional<std::string> Label;
  nextcode::Type Type;
};

/// A representation of a single enumeration case.
struct EnumElementDeclValue {
  std::string Name;
  std::optional<std::string> RawValue;
  std::optional<std::vector<EnumElementParameterValue>> Parameters;
};

using AttrValueVector = llvm::SmallVector<CustomAttrValue, 2>;
struct ConstValueTypePropertyInfo {
  nextcode::VarDecl *VarDecl;
  std::shared_ptr<CompileTimeValue> Value;
  std::optional<AttrValueVector> PropertyWrappers;

  ConstValueTypePropertyInfo(nextcode::VarDecl *VarDecl,
                             std::shared_ptr<CompileTimeValue> Value,
                             std::optional<AttrValueVector> PropertyWrappers)
      : VarDecl(VarDecl), Value(Value), PropertyWrappers(PropertyWrappers) {}

  ConstValueTypePropertyInfo(nextcode::VarDecl *VarDecl,
                             std::shared_ptr<CompileTimeValue> Value)
      : VarDecl(VarDecl), Value(Value),
        PropertyWrappers(std::optional<AttrValueVector>()) {}
};

struct ConstValueTypeInfo {
  nextcode::NominalTypeDecl *TypeDecl;
  std::vector<ConstValueTypePropertyInfo> Properties;
  std::optional<std::vector<EnumElementDeclValue>> EnumElements;
};
} // namespace nextcode
#endif
