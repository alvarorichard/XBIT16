{
    files = {
        "Parser.cpp"
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
    depfiles_gcc = "build/.objs/xbit6/linux/x86_64/release/__cpp_Parser.cpp.cpp: Parser.cpp   Parser.hpp SymbolTable.hpp\
"
}