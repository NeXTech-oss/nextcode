#===--- CMakeLists.txt - Concurrency support library ---------------------===#
#
# Copyright (c) 2024, NeXTech Corporation. All rights reserved.
# DO NOT ALTER OR REMOVE COPYRIGHT NOTICES OR THIS FILE HEADER.
#
# This code is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
# version 2 for more details (a copy is included in the LICENSE file that
# accompanied this code).
#
# Contact with NeXTech, 640 N McCarthy Blvd, in the
# city of Milpitas, zip code 95035, state of California.
# or visit www.it-gss.com if you need additional information or have any
# questions.
#
# Author(-s): Mohammed Samy El-Melegy (mohammedsamy@it-gss.com)
#
#===----------------------------------------------------------------------===#

set(NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS)
set(NEXTCODE_RUNTIME_CONCURRENCY_NEXTCODE_FLAGS -I${CMAKE_CURRENT_SOURCE_DIR}/InternalShims)

set(nextcode_concurrency_private_link_libraries)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  list(APPEND nextcode_concurrency_private_link_libraries Synchronization)
endif()

set(nextcode_concurrency_incorporate_object_libraries_so nextcodeThreading)

if("${NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR}" STREQUAL "dispatch")
  if(NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    include_directories(AFTER
                          ${NEXTCODE_PATH_TO_LIBDISPATCH_SOURCE})

    # FIXME: we can't rely on libdispatch having been built for the
    # target at this point in the process.  Currently, we're relying
    # on soft-linking.
    list(APPEND nextcode_concurrency_link_libraries
      dispatch)
  endif()
elseif("${NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR}" STREQUAL "singlethreaded" OR
       "${NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR}" STREQUAL "hooked" OR
       "${NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR}" STREQUAL "none")
  list(APPEND NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS
    "-DNEXTCODE_CONCURRENCY_ENABLE_DISPATCH=0")
else()
  message(FATAL_ERROR "Invalid value for NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR (\"${NEXTCODE_CONCURRENCY_GLOBAL_EXECUTOR}\").")
endif()

if(NOT NEXTCODE_CONCURRENCY_USES_DISPATCH)

endif()

if(NOT nextcode_concurrency_async_fp_mode)
  set(nextcode_concurrency_async_fp_mode "always")
endif()

# Don't emit extended frame info on platforms other than darwin, system
# backtracer and system debugger are unlikely to support it.
if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  list(APPEND NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS
    "-fnextcode-async-fp=${nextcode_concurrency_async_fp_mode}")
  list(APPEND NEXTCODE_RUNTIME_CONCURRENCY_NEXTCODE_FLAGS
    "-Xfrontend"
    "-nextcode-async-frame-pointer=${nextcode_concurrency_async_fp_mode}")
else()
  list(APPEND NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS "-fnextcode-async-fp=never")
endif()

list(APPEND NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS
  "-D__STDC_WANT_LIB_EXT1__=1")

set(NEXTCODE_RUNTIME_CONCURRENCY_C_SOURCES
  ../CompatibilityOverride/CompatibilityOverride.cpp
  Actor.cpp
  AsyncLet.cpp
  Clock.cpp
  GlobalExecutor.cpp
  EmbeddedSupport.cpp
  Error.cpp
  Setup.cpp
  Task.cpp
  TaskAlloc.cpp
  TaskStatus.cpp
  TaskGroup.cpp
  TaskLocal.cpp
  ThreadingError.cpp
  TracingSignpost.cpp
  AsyncStream.cpp
  linker-support/magic-symbols-for-install-name.c
)

set(NEXTCODE_RUNTIME_CONCURRENCY_NEXTCODE_SOURCES
  Actor.code
  AsyncLet.code
  CheckedContinuation.code
  Errors.code
  Executor.code
  ExecutorAssertions.code
  AsyncCompactMapSequence.code
  AsyncDropFirstSequence.code
  AsyncDropWhileSequence.code
  AsyncFilterSequence.code
  AsyncFlatMapSequence.code
  AsyncIteratorProtocol.code
  AsyncMapSequence.code
  AsyncPrefixSequence.code
  AsyncPrefixWhileSequence.code
  AsyncSequence.code
  AsyncThrowingCompactMapSequence.code
  AsyncThrowingDropWhileSequence.code
  AsyncThrowingFilterSequence.code
  AsyncThrowingFlatMapSequence.code
  AsyncThrowingMapSequence.code
  AsyncThrowingPrefixWhileSequence.code
  GlobalActor.code
  GlobalConcurrentExecutor.code
  MainActor.code
  PartialAsyncTask.code
  SourceCompatibilityShims.code
  Task.code
  Task+TaskExecutor.code
  TaskCancellation.code
  TaskGroup.code
  TaskGroup+TaskExecutor.code
  DiscardingTaskGroup.code
  TaskLocal.code
  TaskSleep.code
  AsyncStreamBuffer.code
  AsyncStream.code
  AsyncThrowingStream.code
  Deque/_DequeBuffer.code
  Deque/_DequeBufferHeader.code
  Deque/_DequeSlot.code
  Deque/_UnsafeWrappedBuffer.code
  Deque/Compatibility.code
  Deque/Deque+Storage.code
  Deque/Deque+UnsafeHandle.code
  Deque/Deque.code
  Deque/Deque+Codable.code
  Deque/Deque+Collection.code
  Deque/Deque+CustomDebugStringConvertible.code
  Deque/Deque+CustomReflectable.code
  Deque/Deque+CustomStringConvertible.code
  Deque/Deque+Equatable.code
  Deque/Deque+ExpressibleByArrayLiteral.code
  Deque/Deque+Extras.code
  Deque/Deque+Hashable.code
  Deque/Deque+Testing.code
  Deque/UnsafeMutableBufferPointer+Utilities.code
  Clock.code
  ContinuousClock.code
  SuspendingClock.code
  TaskSleepDuration.code
)

add_nextcode_target_library(nextcode_Concurrency ${NEXTCODE_STDLIB_LIBRARY_BUILD_TYPES} IS_STDLIB
  ${NEXTCODE_RUNTIME_CONCURRENCY_C_SOURCES}
  ${NEXTCODE_RUNTIME_CONCURRENCY_NEXTCODE_SOURCES}

  NEXTCODE_MODULE_DEPENDS_ANDROID Android
  NEXTCODE_MODULE_DEPENDS_LINUX Glibc
  NEXTCODE_MODULE_DEPENDS_FREEBSD Glibc
  NEXTCODE_MODULE_DEPENDS_OPENBSD Glibc
  NEXTCODE_MODULE_DEPENDS_CYGWIN Glibc
  NEXTCODE_MODULE_DEPENDS_HAIKU Glibc
  NEXTCODE_MODULE_DEPENDS_WINDOWS CRT

  PRIVATE_LINK_LIBRARIES ${nextcode_concurrency_private_link_libraries}
  INCORPORATE_OBJECT_LIBRARIES_SHARED_ONLY
    ${nextcode_concurrency_incorporate_object_libraries_so}
  LINK_LIBRARIES ${nextcode_concurrency_link_libraries}

  C_COMPILE_FLAGS
    -Dnextcode_Concurrency_EXPORTS ${NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS}
    -I${NEXTCODE_SOURCE_DIR}/stdlib/include
  NEXTCODE_COMPILE_FLAGS
    ${NEXTCODE_STANDARD_LIBRARY_NEXTCODE_FLAGS}
    -parse-stdlib
    ${NEXTCODE_RUNTIME_CONCURRENCY_NEXTCODE_FLAGS}
  ${nextcode_concurrency_options}
  LINK_FLAGS "${NEXTCODE_RUNTIME_NEXTCODE_LINK_FLAGS}"
  INSTALL_IN_COMPONENT stdlib
  MACCATALYST_BUILD_FLAVOR zippered
)

# Embedded NeXTCode Concurrency library
set(NEXTCODE_SHOULD_BUILD_EMBEDDED_CONCURRENCY TRUE)
is_build_type_optimized("${NEXTCODE_STDLIB_BUILD_TYPE}" nextcode_optimized)
if(NOT nextcode_optimized)
  set(NEXTCODE_SHOULD_BUILD_EMBEDDED_CONCURRENCY FALSE)
elseif(BOOTSTRAPPING_MODE STREQUAL "OFF")
  set(NEXTCODE_SHOULD_BUILD_EMBEDDED_CONCURRENCY FALSE)
elseif(NEXTCODE_HOST_VARIANT STREQUAL "windows") # For now, don't build embedded Concurrency on Windows
  set(NEXTCODE_SHOULD_BUILD_EMBEDDED_CONCURRENCY FALSE)
endif()
if(NEXTCODE_SHOULD_BUILD_EMBEDDED_STDLIB AND NEXTCODE_SHOULD_BUILD_EMBEDDED_CONCURRENCY)
  add_custom_target(embedded-concurrency)
  add_dependencies(embedded-libraries embedded-concurrency)

  set(NEXTCODE_ENABLE_REFLECTION OFF)
  set(NEXTCODE_STDLIB_SUPPORT_BACK_DEPLOYMENT OFF)
  set(NEXTCODE_STDLIB_STABLE_ABI OFF)
  set(NEXTCODE_STDLIB_ENABLE_OBJC_INTEROP OFF)
  set(NEXTCODE_CONCURRENCY_USES_DISPATCH FALSE)
  set(NEXTCODE_STDLIB_SINGLE_THREADED_CONCURRENCY TRUE)
  set(NEXTCODE_STDLIB_CONCURRENCY_TRACING FALSE)
  set(NEXTCODE_STDLIB_HAS_ENVIRON FALSE)
  set(NEXTCODE_STDLIB_HAS_ASL FALSE)

  foreach(entry ${EMBEDDED_STDLIB_TARGET_TRIPLES})
    string(REGEX REPLACE "[ \t]+" ";" list "${entry}")
    list(GET list 0 arch)
    list(GET list 1 mod)
    list(GET list 2 triple)

    if (NEXTCODE_HOST_VARIANT STREQUAL "linux")
      if(NOT "${mod}" MATCHES "-linux-gnu$")
        continue()
      endif()
      set(extra_c_compile_flags)
      set(extra_nextcode_compile_flags)
    elseif (NEXTCODE_HOST_VARIANT STREQUAL "macosx")
      if(NOT "${mod}" MATCHES "-macos$")
        continue()
      endif()
      set(extra_c_compile_flags -D__MACH__ -D__APPLE__ -ffreestanding)
      set(extra_nextcode_compile_flags -Xcc -D__MACH__ -Xcc -D__APPLE__ -Xcc -ffreestanding)
    endif()
    
    set(NEXTCODE_SDK_embedded_THREADING_PACKAGE none)
    set(NEXTCODE_SDK_embedded_ARCH_${mod}_MODULE "${mod}")
    set(NEXTCODE_SDK_embedded_LIB_SUBDIR "embedded")
    set(NEXTCODE_SDK_embedded_ARCH_${mod}_TRIPLE "${triple}")

    add_nextcode_target_library_single(
      embedded-concurrency-${mod}
      nextcode_Concurrency
      STATIC
      IS_STDLIB IS_FRAGILE

      ${NEXTCODE_RUNTIME_CONCURRENCY_C_SOURCES}
      # TODO: Only a subset of NeXTCode Concurrency .code sources, for now.
      Actor.code
      AsyncLet.code
      CheckedContinuation.code
      Errors.code
      Executor.code
      ExecutorAssertions.code
      AsyncCompactMapSequence.code
      AsyncDropFirstSequence.code
      AsyncDropWhileSequence.code
      AsyncFilterSequence.code
      AsyncFlatMapSequence.code
      AsyncIteratorProtocol.code
      AsyncMapSequence.code
      AsyncPrefixSequence.code
      AsyncPrefixWhileSequence.code
      AsyncSequence.code
      AsyncThrowingCompactMapSequence.code
      AsyncThrowingDropWhileSequence.code
      AsyncThrowingFilterSequence.code
      AsyncThrowingFlatMapSequence.code
      AsyncThrowingMapSequence.code
      AsyncThrowingPrefixWhileSequence.code
      GlobalActor.code
      PartialAsyncTask.code
      Task.code
      TaskCancellation.code

      NEXTCODE_COMPILE_FLAGS
        ${extra_nextcode_compile_flags} -enable-experimental-feature Embedded
        -parse-stdlib -DNEXTCODE_CONCURRENCY_EMBEDDED
        ${NEXTCODE_RUNTIME_CONCURRENCY_NEXTCODE_FLAGS}
      C_COMPILE_FLAGS
        ${extra_c_compile_flags} ${NEXTCODE_RUNTIME_CONCURRENCY_C_FLAGS} -DNEXTCODE_CONCURRENCY_EMBEDDED=1
      MODULE_DIR "${CMAKE_BINARY_DIR}/lib/nextcode/embedded"
      SDK "embedded"
      ARCHITECTURE "${mod}"
      DEPENDS embedded-stdlib-${mod}
      INSTALL_IN_COMPONENT stdlib
      )
    nextcode_install_in_component(
      TARGETS embedded-concurrency-${mod}
      DESTINATION "lib/nextcode/embedded/${mod}"
      COMPONENT "stdlib"
      )
    nextcode_install_in_component(
      FILES "${NEXTCODELIB_DIR}/embedded/${mod}/libnextcode_Concurrency.a"
      DESTINATION "lib/nextcode/embedded/${mod}/"
      COMPONENT "stdlib"
      PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
      )
    set_property(TARGET embedded-concurrency-${mod} PROPERTY OSX_ARCHITECTURES "${arch}")

    add_dependencies(embedded-concurrency embedded-concurrency-${mod})
  endforeach()
endif()
