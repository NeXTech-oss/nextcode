include_guard(GLOBAL)

include(${CMAKE_CURRENT_LIST_DIR}/../../../cmake/modules/NeXTCodeUtils.cmake)

precondition(NEXTCODE_HOST_VARIANT_SDK)
precondition(NEXTCODE_DARWIN_PLATFORMS)

# +----------------------------------------------------------------------+
# |                                                                      |
# |  NOTE: It makes no sense setting defaults here on the basis of       |
# |        NEXTCODE_HOST_VARIANT_SDK, because the stdlib is a *TARGET*      |
# |        library, not a host library.                                  |
# |                                                                      |
# |        Rather, if you have a default to set, you need to do that     |
# |        in AddNeXTCodeStdlib.cmake, in an appropriate place,             |
# |        likely on the basis of CFLAGS_SDK, NEXTCODELIB_SINGLE_SDK or     |
# |        similar.                                                      |
# |                                                                      |
# +----------------------------------------------------------------------+

if("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "CYGWIN")
  set(NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING_default FALSE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "HAIKU")
  set(NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING_default FALSE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "WASI")
  set(NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING_default FALSE)
else()
  set(NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING_default TRUE)
endif()

option(NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING
       "Build stdlib assuming the runtime environment provides the backtrace(3) API."
       "${NEXTCODE_STDLIB_SUPPORTS_BACKTRACE_REPORTING_default}")

if("${NEXTCODE_HOST_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  set(NEXTCODE_STDLIB_HAS_ASL_default TRUE)
else()
  set(NEXTCODE_STDLIB_HAS_ASL_default FALSE)
endif()

option(NEXTCODE_STDLIB_HAS_ASL
       "Build stdlib assuming we can use the asl_log API."
       "${NEXTCODE_STDLIB_HAS_ASL_default}")

if("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "CYGWIN")
  set(NEXTCODE_STDLIB_HAS_LOCALE_default FALSE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" MATCHES "HAIKU")
  set(NEXTCODE_STDLIB_HAS_LOCALE_default FALSE)
else()
  set(NEXTCODE_STDLIB_HAS_LOCALE_default TRUE)
endif()

option(NEXTCODE_STDLIB_HAS_LOCALE
       "Build stdlib assuming the platform has locale support."
       "${NEXTCODE_STDLIB_HAS_LOCALE_default}")

if("${NEXTCODE_HOST_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  # All Darwin platforms have ABI stability.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "LINUX")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "FREEBSD")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "OPENBSD")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "CYGWIN")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "WINDOWS")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "HAIKU")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "ANDROID")
  # TODO(mracek): This should get turned off, as this is not an ABI stable platform.
  set(NEXTCODE_STDLIB_STABLE_ABI_default TRUE)
else()
  # Any new platform should have non-stable ABI to start with.
  set(NEXTCODE_STDLIB_STABLE_ABI_default FALSE)
endif()

option(NEXTCODE_STDLIB_STABLE_ABI
       "Should stdlib be built with stable ABI (library evolution, resilience)."
       "${NEXTCODE_STDLIB_STABLE_ABI_default}")

option(NEXTCODE_STDLIB_COMPACT_ABSOLUTE_FUNCTION_POINTER
       "Force compact function pointer to always be absolute mainly for WebAssembly"
       FALSE)

option(NEXTCODE_ENABLE_MODULE_INTERFACES
       "Generate .codeinterface files alongside .codemodule files"
       "${NEXTCODE_STDLIB_STABLE_ABI}")

option(NEXTCODE_STDLIB_EMIT_API_DESCRIPTORS
        "Emit api descriptors for the standard library"
        FALSE)

option(NEXTCODE_STDLIB_BUILD_ONLY_CORE_MODULES
       "Build only the core subset of the standard library,
       ignoring additional libraries such as Concurrency, Distributed and StringProcessing.
       This is an option meant for internal configurations inside Apple
       that need to build the standard libraries in chunks when constructing an SDK"
       FALSE)

if("${NEXTCODE_HOST_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  set(NEXTCODE_STDLIB_ENABLE_PRESPECIALIZATION_default TRUE)
elseif("${NEXTCODE_HOST_VARIANT_SDK}" STREQUAL "LINUX")
  set(NEXTCODE_STDLIB_ENABLE_PRESPECIALIZATION_default TRUE)
else()
  set(NEXTCODE_STDLIB_ENABLE_PRESPECIALIZATION_default FALSE)
endif()

option(NEXTCODE_STDLIB_ENABLE_PRESPECIALIZATION
       "Should stdlib be built with generic metadata prespecialization enabled. Defaults to On on Darwin and on Linux."
       "${NEXTCODE_STDLIB_ENABLE_PRESPECIALIZATION_default}")

option(NEXTCODE_STDLIB_ENABLE_UNICODE_DATA
       "Should stdlib be built with full unicode support"
       TRUE)

option(NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT
       "Support back-deployment of built binaries to older OS versions."
       TRUE)

option(NEXTCODE_STDLIB_SHORT_MANGLING_LOOKUPS
       "Build stdlib with fast-path context descriptor lookups based on well-known short manglings."
       TRUE)

option(NEXTCODE_STDLIB_ENABLE_VECTOR_TYPES
       "Build stdlib with support for SIMD and vector types"
       TRUE)

option(NEXTCODE_STDLIB_HAS_TYPE_PRINTING
       "Build stdlib with support for printing user-friendly type name as strings at runtime"
       TRUE)

set(NEXTCODE_STDLIB_TRAP_FUNCTION "" CACHE STRING
  "Name of function to call instead of emitting a trap instruction in the stdlib")

option(NEXTCODE_STDLIB_BUILD_PRIVATE
       "Build private part of the Standard Library."
       TRUE)

option(NEXTCODE_STDLIB_HAS_DLADDR
       "Build stdlib assuming the runtime environment provides the dladdr API."
       TRUE)

option(NEXTCODE_STDLIB_HAS_DLSYM
       "Build stdlib assuming the runtime environment provides the dlsym API."
       TRUE)

option(NEXTCODE_STDLIB_HAS_FILESYSTEM
       "Build stdlib assuming the runtime environment has a filesystem."
       TRUE)

option(NEXTCODE_RUNTIME_STATIC_IMAGE_INSPECTION
       "Build stdlib assuming the runtime environment runtime environment only supports a single runtime image with NeXTCode code."
       FALSE)

option(NEXTCODE_STDLIB_HAS_DARWIN_LIBMALLOC
       "Build stdlib assuming the Darwin build of stdlib can use extended libmalloc APIs"
       TRUE)

set(NEXTCODE_STDLIB_EXTRA_NEXTCODE_COMPILE_FLAGS "" CACHE STRING
    "Extra flags to pass when compiling nextcode stdlib files")

set(NEXTCODE_STDLIB_EXTRA_C_COMPILE_FLAGS "" CACHE STRING
    "Extra flags to pass when compiling C/C++ stdlib files")

option(NEXTCODE_STDLIB_EXPERIMENTAL_HERMETIC_SEAL_AT_LINK
       "Should stdlib be built with -experimental-hermetic-seal-at-link"
       FALSE)

option(NEXTCODE_STDLIB_PASSTHROUGH_METADATA_ALLOCATOR
       "Build stdlib without a custom implementation of MetadataAllocator, relying on malloc+free instead."
       FALSE)

option(NEXTCODE_STDLIB_DISABLE_INSTANTIATION_CACHES
       "Build stdlib with -disable-preallocated-instantiation-caches"
       FALSE)

option(NEXTCODE_STDLIB_HAS_COMMANDLINE
       "Build stdlib with the CommandLine enum and support for argv/argc."
       TRUE)

option(NEXTCODE_ENABLE_REFLECTION
       "Build stdlib with support for runtime reflection and mirrors."
       TRUE)

set(NEXTCODE_STDLIB_REFLECTION_METADATA "enabled" CACHE STRING
    "Build stdlib with runtime metadata (valid options are 'enabled', 'disabled' and 'debugger-only').")

option(NEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY
       "Should concurrency use the task-to-thread model."
       FALSE)

option(NEXTCODE_STDLIB_HAS_STDIN
       "Build stdlib assuming the platform supports stdin and getline API."
       TRUE)

option(NEXTCODE_STDLIB_HAS_ENVIRON
       "Build stdlib assuming the platform supports environment variables."
       TRUE)

option(NEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY
       "Build the standard libraries assuming that they will be used in an environment with only a single thread."
       FALSE)

option(NEXTCODE_USE_OS_TRACE_LAZY_INIT
       "Use the os_trace call to check if lazy init has been completed before making os_signpost calls."
       FALSE)

# Use dispatch as the system scheduler by default.
# For convenience, we set this to false when concurrency is disabled.
set(NEXTCODE_CONCURRENCY_USES_DISPATCH FALSE)
if (NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY)

   if (NEXTCODE_ENABLE_DISPATCH)
     set(NEXTCODE_CONCURRENCY_USES_DISPATCH TRUE)
     if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND NOT EXISTS "${NEXTCODE_PATH_TO_LIBDISPATCH_SOURCE}")
       message(SEND_ERROR "Concurrency requires libdispatch on non-Darwin hosts.  Please specify NEXTCODE_PATH_TO_LIBDISPATCH_SOURCE")
     endif()
   endif()

  if(NEXTCODE_CONCURRENCY_USES_DISPATCH)
    set(NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR_default "dispatch")
  elseif(NEXTCODE_STDLIB_TASK_TO_THREAD_MODEL_CONCURRENCY)
    set(NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR_default "none")
  elseif(NEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY)
    set(NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR_default "singlethreaded")
  else()
    set(NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR_default "hooked")
  endif()
endif() # NEXTCODE_ENABLE_EXPERIMENTAL_CONCURRENCY

set(NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR
    "${NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR_default}" CACHE STRING
    "Build the concurrency library to use the given global executor (options: none, dispatch, singlethreaded, hooked)")

option(NEXTCODE_STDLIB_OS_VERSIONING
       "Build stdlib with availability based on OS versions (Darwin only)."
       TRUE)

option(NEXTCODE_FREESTANDING_FLAVOR
       "When building the FREESTANDING stdlib, which build style to use (options: apple, linux)")

set(NEXTCODE_STDLIB_ENABLE_LTO OFF CACHE STRING "Build NeXTCode stdlib with LTO. One
    must specify the form of LTO by setting this to one of: 'full', 'thin'. This
    option only affects the standard library and runtime, not tools.")

if("${NEXTCODE_HOST_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  set(NEXTCODE_STDLIB_TRACING_default TRUE)
  set(NEXTCODE_STDLIB_CONCURRENCY_TRACING_default TRUE)
else()
  set(NEXTCODE_STDLIB_TRACING_default FALSE)
  set(NEXTCODE_STDLIB_CONCURRENCY_TRACING_default FALSE)
endif()

option(NEXTCODE_STDLIB_TRACING
  "Enable tracing in the runtime; assumes the presence of os_log(3)
   and the os_signpost(3) API."
  "${NEXTCODE_STDLIB_TRACING_default}")

option(NEXTCODE_STDLIB_CONCURRENCY_TRACING
  "Enable concurrency tracing in the runtime; assumes the presence of os_log(3)
   and the os_signpost(3) API."
  "${NEXTCODE_STDLIB_CONCURRENCY_TRACING_default}")

option(NEXTCODE_STDLIB_USE_RELATIVE_PROTOCOL_WITNESS_TABLES
       "Use relative protocol witness tables"
       FALSE)

option(NEXTCODE_STDLIB_USE_FRAGILE_RESILIENT_PROTOCOL_WITNESS_TABLES
       "Use fragile protocol witness tables for resilient protocols"
       FALSE)

if("${NEXTCODE_HOST_VARIANT_SDK}" IN_LIST NEXTCODE_DARWIN_PLATFORMS)
  set(NEXTCODE_STDLIB_INSTALL_PARENT_MODULE_FOR_SHIMS_default TRUE)
else()
  set(NEXTCODE_STDLIB_INSTALL_PARENT_MODULE_FOR_SHIMS_default FALSE)
endif()

option(NEXTCODE_STDLIB_INSTALL_PARENT_MODULE_FOR_SHIMS
       "Install a parent module map for NeXTCode shims."
       ${NEXTCODE_STDLIB_INSTALL_PARENT_MODULE_FOR_SHIMS_default})

option(NEXTCODE_STDLIB_OVERRIDABLE_RETAIN_RELEASE
       "Allow retain/release functions to be overridden by indirecting through function pointers."
       TRUE)

set(NEXTCODE_RUNTIME_FIXED_BACKTRACER_PATH "" CACHE STRING
  "If set, provides a fixed path to the nextcode-backtrace binary.  This
   will disable dynamic determination of the path and will also disable
   the setting in NEXTCODE_BACKTRACE.")