cmake_minimum_required(VERSION 3.22)
project(cpp_toy_language)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_toy_language
        "external/magic_enum.hpp"
        "src/main.cpp"
        "src/lexer/Expr.h"
        "src/lexer/Lexer.cpp"
        "src/lexer/Lexer.h"
        "src/lexer/token.cpp"
        "src/lexer/token.h"
        "src/Toy.cpp"
        "src/Toy.h" 
		"src/Lexer/AstPrinter.h")

add_subdirectory(tools/expression_generator)