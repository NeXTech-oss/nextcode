# The nextcode-frontend tool


# Add additional libraries to which we need to link when the NeXTCode NeXTCode
# parser is built in.
function(add_nextcode_parser_link_libraries target)
  if(NEXTCODE_BUILD_NEXTCODE_SYNTAX)
    target_link_libraries(${target}
                          PRIVATE nextcodeCore)

    get_property(NEXTCODE_MACRO_PLUGINS GLOBAL PROPERTY NEXTCODE_MACRO_PLUGINS)
    foreach(macrolib ${NEXTCODE_MACRO_PLUGINS})
      add_dependencies(${target} ${macrolib})
    endforeach()
    add_dependencies(${target} NeXTCodeInProcPluginServer)
  endif()
endfunction()

if(BOOTSTRAPPING_MODE MATCHES "BOOTSTRAPPING.*")

  # Bootstrapping - level 0
  # For more information on how bootstrapping works, see docs/NeXTCodeInTheCompiler.md

  add_nextcode_host_tool(nextcode-frontend-bootstrapping0
    driver.cpp
    NEXTCODE_COMPONENT no_component
    HAS_NEXTCODE_MODULES
    BOOTSTRAPPING 0
    THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY
  )
  target_link_libraries(nextcode-frontend-bootstrapping0
                        PRIVATE
                          nextcodeDriverTool
                          nextcodeCompilerStub)

  add_nextcode_parser_link_libraries(nextcode-frontend-bootstrapping0)

  nextcode_create_post_build_symlink(nextcode-frontend-bootstrapping0
    SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "codec${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bootstrapping0/${CMAKE_CFG_INTDIR}/bin")

  # Bootstrapping - level 1

  add_nextcode_host_tool(nextcode-frontend-bootstrapping1
    driver.cpp
    NEXTCODE_COMPONENT no_component
    HAS_NEXTCODE_MODULES
    BOOTSTRAPPING 1
    THINLTO_LD64_ADD_FLTO_CODEGEN_ONLY
  )
  target_link_libraries(nextcode-frontend-bootstrapping1
                        PRIVATE
                          nextcodeDriverTool
                          nextcodeCompilerModules-bootstrapping1)

  add_nextcode_parser_link_libraries(nextcode-frontend-bootstrapping1)

  nextcode_create_post_build_symlink(nextcode-frontend-bootstrapping1
    SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "codec${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/bootstrapping1/${CMAKE_CFG_INTDIR}/bin")
endif()

add_nextcode_host_tool(nextcode-frontend
  driver.cpp
  NEXTCODE_COMPONENT compiler
  HAS_NEXTCODE_MODULES
)
target_link_libraries(nextcode-frontend
                      PUBLIC
                        nextcodeDriverTool
                        nextcodeCompilerModules)

add_nextcode_parser_link_libraries(nextcode-frontend)

# Create a `nextcode-driver` executable adjacent to the `nextcode-frontend` executable
# to ensure that `codec` forwards to the standalone driver when invoked.
nextcode_create_early_driver_copies(nextcode-frontend)

# If a `nextcode-driver` executable adjacent to the `nextcode-frontend` executable exists
# then the `nextcode` and `codec` symlinks should point to it by-default
if(EXISTS "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-driver${CMAKE_EXECUTABLE_SUFFIX}" AND EXISTS "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-help${CMAKE_EXECUTABLE_SUFFIX}")
  message(STATUS "Pointing 'nextcode' and 'codec' symlinks at 'nextcode-driver'.")
  nextcode_create_post_build_symlink(nextcode-frontend
    SOURCE "nextcode-driver${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "nextcode${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")
  nextcode_create_post_build_symlink(nextcode-frontend
    SOURCE "nextcode-driver${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "codec${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

  message(STATUS "Pointing 'nextcode-legacy-driver' and 'codec-legacy-driver' symlinks at 'nextcode-frontend'.")
  nextcode_create_post_build_symlink(nextcode-frontend
    SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "nextcode-legacy-driver${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")
  nextcode_create_post_build_symlink(nextcode-frontend
    SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "codec-legacy-driver${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")
else()
  message(STATUS "Pointing 'nextcode' and 'codec' symlinks at 'nextcode-frontend' - no early NeXTCodeDriver build found.")
  nextcode_create_post_build_symlink(nextcode-frontend
    SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "nextcode${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

  nextcode_create_post_build_symlink(nextcode-frontend
    SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
    DESTINATION "codec${CMAKE_EXECUTABLE_SUFFIX}"
    WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")
endif()

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "sil-opt${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "sil-func-extractor${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "sil-nm${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "sil-llvm-gen${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "sil-passpipeline-dumper${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-dependency-tool${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-llvm-opt${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-indent${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-symbolgraph-extract${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-autolink-extract${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-api-digester${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-cache-tool${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

nextcode_create_post_build_symlink(nextcode-frontend
  SOURCE "nextcode-frontend${CMAKE_EXECUTABLE_SUFFIX}"
  DESTINATION "nextcode-parse-test${CMAKE_EXECUTABLE_SUFFIX}"
  WORKING_DIRECTORY "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}")

add_dependencies(compiler nextcode-frontend)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT compiler)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/codec${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT compiler)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-symbolgraph-extract${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT compiler)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-api-digester${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT compiler)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-cache-tool${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT compiler)
add_dependencies(autolink-driver nextcode-frontend)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-autolink-extract${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT autolink-driver)
add_dependencies(editor-integration nextcode-frontend)
nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-indent${CMAKE_EXECUTABLE_SUFFIX}"
                           DESTINATION "bin"
                           COMPONENT editor-integration)
if(EXISTS "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-driver${CMAKE_EXECUTABLE_SUFFIX}" AND EXISTS "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-help${CMAKE_EXECUTABLE_SUFFIX}")
  nextcode_install_in_component(PROGRAMS "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-driver${CMAKE_EXECUTABLE_SUFFIX}"
                             DESTINATION "bin"
                             COMPONENT compiler)
  nextcode_install_in_component(PROGRAMS "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-help${CMAKE_EXECUTABLE_SUFFIX}"
                             DESTINATION "bin"
                             COMPONENT compiler)
  nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/codec-legacy-driver${CMAKE_EXECUTABLE_SUFFIX}"
                             DESTINATION "bin"
                             COMPONENT compiler)
  nextcode_install_in_component(FILES "${NEXTCODE_RUNTIME_OUTPUT_INTDIR}/nextcode-legacy-driver${CMAKE_EXECUTABLE_SUFFIX}"
                             DESTINATION "bin"
                             COMPONENT compiler)
endif()
