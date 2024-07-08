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
