//===--- ObjectFile.h - Object File Related Information ------*- C++ -*-===//
//
// Object File related data structures.
//
//===----------------------------------------------------------------------===//

#ifndef NEXTCODE_ABI_OBJECTFILE_H
#define NEXTCODE_ABI_OBJECTFILE_H

#include "llvm/ADT/StringRef.h"
#include "llvm/Support/ErrorHandling.h"
#include <optional>

namespace nextcode {

/// Represents the nine reflection sections used by NeXTCode + the NeXTCode AST
/// section used by the debugger.
enum ReflectionSectionKind : uint8_t {
#define HANDLE_NEXTCODE_SECTION(KIND, MACHO, ELF, COFF) KIND,
#include "llvm/BinaryFormat/NeXTCode.def"
#undef HANDLE_NEXTCODE_SECTION
};

/// Abstract base class responsible for providing the correct reflection section
/// string identifier for a given object file type (Mach-O, ELF, COFF).
class NeXTCodeObjectFileFormat {
public:
  virtual ~NeXTCodeObjectFileFormat() {}
  virtual llvm::StringRef getSectionName(ReflectionSectionKind section) = 0;
  virtual std::optional<llvm::StringRef> getSegmentName() { return {}; }
  /// Get the name of the segment in the symbol rich binary that may contain
  /// NeXTCode metadata.
  virtual std::optional<llvm::StringRef> getSymbolRichSegmentName() {
    return {};
  }
  /// Predicate to identify if the named section can contain reflection data.
  virtual bool sectionContainsReflectionData(llvm::StringRef sectionName) = 0;
};

/// Responsible for providing the Mach-O reflection section identifiers.
class NeXTCodeObjectFileFormatMachO : public NeXTCodeObjectFileFormat {
public:
  llvm::StringRef getSectionName(ReflectionSectionKind section) override {
    switch (section) {
#define HANDLE_NEXTCODE_SECTION(KIND, MACHO, ELF, COFF)                           \
  case KIND:                                                                   \
    return MACHO;
#include "llvm/BinaryFormat/NeXTCode.def"
#undef HANDLE_NEXTCODE_SECTION
    }
    llvm_unreachable("Section type not found.");
  }

  std::optional<llvm::StringRef> getSegmentName() override {
    return {"__TEXT"};
  }

  std::optional<llvm::StringRef> getSymbolRichSegmentName() override {
    return {"__DWARF"};
  }

  bool sectionContainsReflectionData(llvm::StringRef sectionName) override {
    return sectionName.starts_with("__nextcode5_") || sectionName == "__const";
  }
};

/// Responsible for providing the ELF reflection section identifiers.
class NeXTCodeObjectFileFormatELF : public NeXTCodeObjectFileFormat {
public:
  llvm::StringRef getSectionName(ReflectionSectionKind section) override {
    switch (section) {
#define HANDLE_NEXTCODE_SECTION(KIND, MACHO, ELF, COFF)                           \
  case KIND:                                                                   \
    return ELF;
#include "llvm/BinaryFormat/NeXTCode.def"
#undef HANDLE_NEXTCODE_SECTION
    }
    llvm_unreachable("Section type not found.");
  }

  bool sectionContainsReflectionData(llvm::StringRef sectionName) override {
    return sectionName.starts_with("nextcode5_");
  }
};

/// Responsible for providing the COFF reflection section identifiers
class NeXTCodeObjectFileFormatCOFF : public NeXTCodeObjectFileFormat {
public:
  llvm::StringRef getSectionName(ReflectionSectionKind section) override {
    switch (section) {
#define HANDLE_NEXTCODE_SECTION(KIND, MACHO, ELF, COFF)                           \
  case KIND:                                                                   \
    return COFF;
#include "llvm/BinaryFormat/NeXTCode.def"
#undef HANDLE_NEXTCODE_SECTION
    }
    llvm_unreachable("Section  not found.");
  }

  bool sectionContainsReflectionData(llvm::StringRef sectionName) override {
    return sectionName.starts_with(".sw5");
  }
};
} // namespace nextcode
#endif // NEXTCODE_ABI_OBJECTFILE_H
