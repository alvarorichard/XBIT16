{
    files = {
        "Assembler.cpp"
    },
    values = {
        "/home/linuxbrew/.linuxbrew/bin/clang",
        {
            "-Qunused-arguments",
            "-m64",
            "-fvisibility=hidden",
            "-fvisibility-inlines-hidden",
            "-O3",
            "-DNDEBUG"
        }
    },
    depfiles_gcc = "build/.objs/xbit6/linux/x86_64/release/__cpp_Assembler.cpp.cpp:   Assembler.cpp Parser.hpp SymbolTable.hpp\
"
}