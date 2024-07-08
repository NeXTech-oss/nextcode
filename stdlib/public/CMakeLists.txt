add_subdirectory(LLVMSupport)

set(NEXTCODE_RUNTIME_CXX_FLAGS)
set(NEXTCODE_RUNTIME_LINK_FLAGS)
set(NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS)
set(NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS)

if(NEXTCODE_RUNTIME_USE_SANITIZERS)
  # TODO: Refactor this
  if("Thread" IN_LIST NEXTCODE_RUNTIME_USE_SANITIZERS)
    list(APPEND NEXTCODE_RUNTIME_CXX_FLAGS "-fsanitize=thread")
    list(APPEND NEXTCODE_RUNTIME_LINK_FLAGS "-fsanitize=thread")
    list(APPEND NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS "-sanitize=thread")
    list(APPEND NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS "-fsanitize=thread")
  endif()
endif()

if(NEXTCODE_STDLIB_SIL_DEBUGGING)
  list(APPEND NEXTCODE_RUNTIME_NEXTCODE_COMPILE_FLAGS "-Xfrontend" "-sil-based-debuginfo")
endif()

option(NEXTCODE_RUNTIME_DYLD_SHARED_CACHE_COMPATIBLE
       "On Darwin platforms, link the standard library in a way that
       allows inclusion in the dyld shared cache"
       FALSE)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND
   LLVM_LINKER_IS_APPLE AND
   NOT NEXTCODE_RUNTIME_DYLD_SHARED_CACHE_COMPATIBLE)
  set(shared_cache_link_flags "LINKER:-not_for_dyld_shared_cache")
  list(APPEND NEXTCODE_RUNTIME_LINK_FLAGS ${shared_cache_link_flags})
  list(APPEND NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS ${shared_cache_link_flags})
endif()

# Build the runtime with -Wall to catch, e.g., uninitialized variables
# warnings.
if(NEXTCODE_COMPILER_IS_MSVC_LIKE)
  list(APPEND NEXTCODE_RUNTIME_CXX_FLAGS "/W3")
else()
  list(APPEND NEXTCODE_RUNTIME_CXX_FLAGS "-Wall")
endif()

set(NEXTCODE_RUNTIME_CORE_CXX_FLAGS "${NEXTCODE_RUNTIME_CXX_FLAGS}")
set(NEXTCODE_RUNTIME_CORE_LINK_FLAGS "${NEXTCODE_RUNTIME_LINK_FLAGS}")

if(CMAKE_SYSTEM_NAME STREQUAL "CYGWIN")
  list(APPEND NEXTCODE_RUNTIME_CORE_CXX_FLAGS "-mcmodel=large")
elseif(CMAKE_SYSTEM_NAME STREQUAL "OpenBSD")
  list(APPEND NEXTCODE_RUNTIME_CORE_CXX_FLAGS "-xc++")
endif()

# We should avoid non-literals in format strings, or appropriately mark
# functions.
check_cxx_compiler_flag("-Wformat-nonliteral -Werror=format-nonliteral" CXX_SUPPORTS_FORMAT_NONLITERAL_WARNING)
if (CXX_SUPPORTS_FORMAT_NONLITERAL_WARNING)
  list(APPEND NEXTCODE_RUNTIME_CORE_CXX_FLAGS "-Wformat-nonliteral"
    "-Werror=format-nonliteral")
endif()

# C++ code in the runtime and standard library should generally avoid
# introducing static constructors or destructors.
check_cxx_compiler_flag("-Wglobal-constructors -Werror=global-constructors" CXX_SUPPORTS_GLOBAL_CONSTRUCTORS_WARNING)
if(CXX_SUPPORTS_GLOBAL_CONSTRUCTORS_WARNING)
  list(APPEND NEXTCODE_RUNTIME_CORE_CXX_FLAGS "-Wglobal-constructors"
    "-Werror=global-constructors")
endif()

# C++ code in the runtime and standard library should generally avoid
# introducing static constructors or destructors.
check_cxx_compiler_flag("-Wexit-time-destructors -Werror=exit-time-destructors" CXX_SUPPORTS_EXIT_TIME_DESTRUCTORS_WARNING)
if(CXX_SUPPORTS_EXIT_TIME_DESTRUCTORS_WARNING)
  list(APPEND NEXTCODE_RUNTIME_CORE_CXX_FLAGS "-Wexit-time-destructors"
    "-Werror=exit-time-destructors")
endif()

add_subdirectory(NeXTCodeShims/nextcode/shims)
add_subdirectory(CommandLineSupport)
if(NEXTCODE_ENABLE_EXPERIMENTAL_CXX_INTEROP)
  add_subdirectory(Cxx)
else()
  # NeXTCodeCompilerSources rely on C++ interop, specifically on being able to
  # import the C++ standard library into NeXTCode. On Linux, this is only possible
  # when using the modulemap that NeXTCode provides for libstdc++. Make sure we
  # include the libstdc++ modulemap into the build even when not building
  # C++ interop modules.
  add_subdirectory(Cxx/libstdcxx)
endif()
add_subdirectory(Threading)

# This static library is shared across nextcodeCore and nextcodeRemoteInspection
if(NEXTCODE_BUILD_STDLIB OR NEXTCODE_BUILD_REMOTE_MIRROR)
  # TODO: due to the use of `add_nextcode_target_library` rather than `add_library`
  # we cannot use `target_sources` and thus must resort to list manipulations to
  # adjust the source list.
  set(nextcodeDemanglingSources
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/Context.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/Demangler.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/ManglingUtils.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/NodePrinter.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/Punycode.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/Remangler.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/NodeDumper.cpp"
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/Errors.cpp")
  set(nextcodeDemanglingCRSources
    "${NEXTCODE_SOURCE_DIR}/lib/Demangling/CrashReporter.cpp")

  set(nextcode_demangling_cflags)

  if(NEXTCODE_RUNTIME_CRASH_REPORTER_CLIENT)
  list(APPEND nextcode_demangling_cflags
      "-DNEXTCODE_HAVE_CRASHREPORTERCLIENT=1")
  endif()

  # The old mangling support is only needed on platforms with ObjC.
  if(NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP)
    list(APPEND nextcodeDemanglingSources
      "${NEXTCODE_SOURCE_DIR}/lib/Demangling/OldDemangler.cpp"
      "${NEXTCODE_SOURCE_DIR}/lib/Demangling/OldRemangler.cpp"
      )
    list(APPEND nextcode_demangling_cflags -DNEXTCODE_SUPPORT_OLD_MANGLING=1)
  else()
    list(APPEND nextcode_demangling_cflags -DNEXTCODE_SUPPORT_OLD_MANGLING=0)
  endif()

  if(NEXTCODE_STDLIB_HAS_TYPE_PRINTING)
    list(APPEND nextcode_demangling_cflags -DNEXTCODE_STDLIB_HAS_TYPE_PRINTING)
  endif()

  # Gold LTO is unsupported. To prevent tests from failing when building
  # with LTO, force nextcodeDemangling library to compile without LTO for Linux.
  add_nextcode_target_library(nextcodeDemangling OBJECT_LIBRARY
    ${nextcodeDemanglingSources}
    C_COMPILE_FLAGS
      -DnextcodeCore_EXPORTS
      ${nextcode_demangling_cflags}
    C_COMPILE_FLAGS_LINUX -fno-lto
    INSTALL_IN_COMPONENT never_install)

  add_nextcode_target_library(nextcodeDemanglingCR OBJECT_LIBRARY
    ${nextcodeDemanglingCRSources}
    C_COMPILE_FLAGS
      -DnextcodeCore_EXPORTS
      ${nextcode_demangling_cflags}
    INSTALL_IN_COMPONENT never_install)
endif()

option(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB
      "Enable build of the embedded NeXTCode standard library and runtime"
      TRUE)

option(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB_CROSS_COMPILING
      "Enable build of the embedded NeXTCode standard library and runtime for cross-compiling targets"
      FALSE)

if((NOT NEXTCODE_HOST_VARIANT STREQUAL "macosx") AND (NOT NEXTCODE_HOST_VARIANT STREQUAL "linux") AND (NOT NEXTCODE_HOST_VARIANT STREQUAL "windows"))
  set(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB FALSE)
elseif(BOOTSTRAPPING_MODE STREQUAL "OFF")
  set(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB FALSE)
endif()

if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  add_custom_target(embedded-libraries ALL)
endif()

set(EMBEDDED_STDLIB_TARGET_TRIPLES)

if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB_CROSS_COMPILING)
  if("ARM" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      # arch    module_name               target triple
      "armv6    armv6-apple-none-macho    armv6-apple-none-macho"
      "armv6m   armv6m-apple-none-macho   armv6m-apple-none-macho"
      "armv7    armv7-apple-none-macho    armv7-apple-none-macho"
      "armv7em  armv7em-apple-none-macho  armv7em-apple-none-macho"
    )
  endif()
  if("AArch64" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "arm64    arm64-apple-none-macho    arm64-apple-none-macho"
    )
  endif()

  # the following are all ELF targets
  if("ARM" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "armv4t   armv4t-none-none-eabi     armv4t-none-none-eabi"
      "armv6    armv6-none-none-eabi      armv6-none-none-eabi"
      "armv6m   armv6m-none-none-eabi     armv6m-none-none-eabi"
      "armv7    armv7-none-none-eabi      armv7-none-none-eabi"
      "armv7em  armv7em-none-none-eabi    armv7em-none-none-eabi"
    )
  endif()
  if("AArch64" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "aarch64  aarch64-none-none-elf     aarch64-none-none-elf"
    )
  endif()
  if("RISCV" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "riscv32  riscv32-none-none-eabi    riscv32-none-none-eabi"
      "riscv64  riscv64-none-none-eabi    riscv64-none-none-eabi"
    )
  endif()
  if("X86" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "i686    i686-unknown-none-elf       i686-unknown-none-elf"
      "x86_64  x86_64-unknown-none-elf     x86_64-unknown-none-elf"
    )
  endif()

  if("WebAssembly" IN_LIST LLVM_TARGETS_TO_BUILD)
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "wasm32    wasm32-unknown-none-wasm    wasm32-unknown-none-wasm"
      "wasm64    wasm64-unknown-none-wasm    wasm64-unknown-none-wasm"
    )
  endif()
endif()

if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  if (NEXTCODE_HOST_VARIANT STREQUAL "linux")
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "${NEXTCODE_HOST_VARIANT_ARCH} ${NEXTCODE_HOST_VARIANT_ARCH}-unknown-linux-gnu ${NEXTCODE_HOST_VARIANT_ARCH}-unknown-linux-gnu"
      # In practice this expands to either:
      # "x86_64   x86_64-unknown-linux-gnu  x86_64-unknown-linux-gnu"
      # "aarch64  aarch64-unknown-linux-gnu aarch64-unknown-linux-gnu"
    )
  elseif (NEXTCODE_HOST_VARIANT STREQUAL "macosx")
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      "x86_64   x86_64-apple-macos        x86_64-apple-macos14"
      "arm64    arm64-apple-macos         arm64-apple-macos14"
      "arm64e   arm64e-apple-macos        arm64e-apple-macos14"
    )
  elseif (NEXTCODE_HOST_VARIANT STREQUAL "windows")
    list(APPEND EMBEDDED_STDLIB_TARGET_TRIPLES
      # Without specifying the `windows` component LLVM refuses to
      # generate valid COFF object files.
      "i686    i686-unknown-windows-msvc   i686-unknown-windows-msvc"
      "x86_64  x86_64-unknown-windows-msvc x86_64-unknown-windows-msvc"
    )
  endif()
endif()

if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB)
  set(triples)
  foreach(entry ${EMBEDDED_STDLIB_TARGET_TRIPLES})
    string(REGEX REPLACE "[ \t]+" ";" list "${entry}")
    list(GET list 2 triple)
    list(APPEND triples ${triple})
  endforeach()

  message(STATUS "Building Embedded NeXTCode standard libraries for targets: ${triples}")
  message(STATUS "")
endif()

if(NEXTCODE_BUILD_STDLIB)
  # These must be kept in dependency order so that any referenced targets
  # exist at the time we look for them in add_nextcode_*.
  add_subdirectory(runtime)
  add_subdirectory(stubs)
  add_subdirectory(core)
  add_subdirectory(NeXTCodeOnoneSupport)

  if(NEXTCODE_BUILD_CLANG_OVERLAYS)
    add_subdirectory(ClangOverlays)
  endif()
endif()

if(NEXTCODE_BUILD_STDLIB AND NOT NEXTCODE_STDLIB_BUILD_ONLY_CORE_MODULES)
  # In some internal Apple configurations we have the need
  # to build Core and Onone separately from the rest
  # of the stdlib
  if(NEXTCODE_ENABLE_EXPERIMENTAL_DIFFERENTIABLE_PROGRAMMING)
    add_subdirectory(Differentiation)
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY)
    add_subdirectory(Concurrency)
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_DISTRIBUTED)
    add_subdirectory(Distributed)
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_STRING_PROCESSING)
    add_subdirectory(RegexParser)
    add_subdirectory(StringProcessing)
    add_subdirectory(RegexBuilder)
  endif()

  if(NEXTCODE_ENABLE_EXPERIMENTAL_OBSERVATION)
    add_subdirectory(Observation)
  endif()

  if(NEXTCODE_ENABLE_BACKTRACING)
    add_subdirectory(Backtracing)
  endif()

  if(NEXTCODE_ENABLE_SYNCHRONIZATION)
    add_subdirectory(Synchronization)
  endif()

  if(NEXTCODE_ENABLE_VOLATILE)
    add_subdirectory(Volatile)
  endif()
endif()

if(NEXTCODE_BUILD_REMOTE_MIRROR)
  add_subdirectory(RemoteInspection)
  add_subdirectory(NeXTCodeRemoteMirror)
endif()

if(NEXTCODE_BUILD_SDK_OVERLAY OR NEXTCODE_BUILD_TEST_SUPPORT_MODULES)
  add_subdirectory(Platform)
endif()

if(NEXTCODE_BUILD_SDK_OVERLAY)
  # On Apple platforms, we aren't building any overlays (other than Darwin in
  # Platform above). Instead, we're picking them up from the SDK.

  if(WINDOWS IN_LIST NEXTCODE_SDKS)
    add_subdirectory(Windows)
  endif()
endif()

if(NEXTCODE_BUILD_LIBEXEC)
add_subdirectory(libexec)
endif()

