set(nextcode_stubs_sources
    Assert.cpp
    GlobalObjects.cpp
    LibcShims.cpp
    Random.cpp
    Stubs.cpp
    ThreadLocalStorage.cpp
    MathStubs.cpp
    Unicode/UnicodeData.cpp
    Unicode/UnicodeGrapheme.cpp
    Unicode/UnicodeNormalization.cpp
    Unicode/UnicodeScalarProps.cpp
    Unicode/UnicodeWord.cpp
)
set(nextcode_stubs_objc_sources
    Availability.mm
    FoundationHelpers.mm
    OptionalBridgingHelper.mm
    Reflection.mm
    NeXTCodeNativeNSObject.mm
    NeXTCodeNativeNSXXXBaseARC.m)
set(nextcode_stubs_gyb_sources
    NeXTCodeNativeNSXXXBase.mm.gyb)
set(LLVM_OPTIONAL_SOURCES
    ${nextcode_stubs_objc_sources}
    ${nextcode_stubs_gyb_sources})

set(nextcode_stubs_c_compile_flags ${NEXTCODE_RUNTIME_CORE_CXX_FLAGS})
list(APPEND nextcode_stubs_c_compile_flags -DnextcodeCore_EXPORTS)
list(APPEND nextcode_stubs_c_compile_flags -I${NEXTCODE_SOURCE_DIR}/include -I${NEXTCODE_SOURCE_DIR}/stdlib/public)

add_nextcode_target_library(nextcodeStdlibStubs
                  OBJECT_LIBRARY
                    ${nextcode_stubs_sources}
                    ${nextcode_stubs_objc_sources}
                  GYB_SOURCES
                    ${nextcode_stubs_gyb_sources}
                  C_COMPILE_FLAGS
                    ${nextcode_stubs_c_compile_flags}
                  LINK_FLAGS
                    ${NEXTCODE_RUNTIME_CORE_LINK_FLAGS}
                  NEXTCODE_COMPILE_FLAGS ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
                  INSTALL_IN_COMPONENT
                    stdlib)

if("${NEXTCODE_PRIMARY_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  set_property(SOURCE
                 NeXTCodeNativeNSXXXBaseARC.m
               APPEND_STRING PROPERTY COMPILE_FLAGS
                 "-fobjc-arc")
endif()

add_subdirectory(Unicode)
