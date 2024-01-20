add_rules("mode.debug", "mode.release")

target("xbit6")
    set_kind("binary")
    set_optimize("fastest")
    add_files("SymbolTable.cpp", "Parser.cpp", "Assembler.cpp")
    set_toolchains("clang")
