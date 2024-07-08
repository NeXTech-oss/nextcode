add_nextcode_target_executable(nextcode-reflection-test BUILD_WITH_STDLIB
  NONEXTCODERT
  nextcode-reflection-test.c
  overrides.c
  LINK_LIBRARIES
    nextcodeRemoteMirror
    nextcodeRemoteInspection

  TARGET_SDKS NOT LINUX_STATIC)

# NOTE(compnerd) since _WINDLL has no impact on non-Windows targets,
# we just spam it on all the targets for simplicity due to the build
# structure of nextcode.  This will make the global variable imported,
# which we need.
set_source_files_properties(nextcode-reflection-test.c
                            PROPERTIES
                              COMPILE_FLAGS -D_WINDLL)
