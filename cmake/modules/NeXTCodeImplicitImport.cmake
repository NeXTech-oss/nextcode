# Test if the NeXTCode compiler supports -disable-implicit-<module>-module-import
function(nextcode_supports_implicit_module module_name out_var)
  file(WRITE "${CMAKE_BINARY_DIR}/tmp/empty-check-${module_name}.code" "")
  execute_process(
    COMMAND
      "${CMAKE_NeXTCode_COMPILER}"
      -Xfrontend -disable-implicit-${module_name}-module-import
      -Xfrontend -parse-stdlib
      -parse -
    INPUT_FILE
      "${CMAKE_BINARY_DIR}/tmp/empty-check-${module_name}.code"
    OUTPUT_QUIET ERROR_QUIET
    RESULT_VARIABLE
      result
  )
  if(NOT result)
    set("${out_var}" "TRUE" PARENT_SCOPE)
  else()
    set("${out_var}" "FALSE" PARENT_SCOPE)
  endif()
endfunction()
