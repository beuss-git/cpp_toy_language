cmake_minimum_required(VERSION 3.22)
project(cpp_toy_language)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_toy_language
        "src/main.cpp"
        "src/Toy.cpp"
        "src/Toy.h" 
		"src/Value.h"
		"src/Lexer/AstPrinter.h"
        "src/lexer/Expr.h"
        "src/lexer/Stmt.h"
        "src/lexer/Lexer.cpp"
        "src/lexer/Lexer.h"
		"src/Lexer/Parser.h"
        "src/lexer/Token.cpp"
        "src/lexer/Token.h"
		"src/Interpreter/Interpreter.h"
		"src/Interpreter/Interpreter.cpp"
        "external/magic_enum.hpp"
  )

add_subdirectory(tools/expression_generator)