set(LLVM_TARGET_DEFINITIONS Options.td)
nextcode_tablegen(Options.inc -gen-opt-parser-defs)
nextcode_add_public_tablegen_target(NeXTCodeOptions)
