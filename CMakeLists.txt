cmake_minimum_required(VERSION 3.22)
project(cpp_toy_language)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_toy_language
        src/main.cpp
        src/lexer/lexer.cpp
        src/lexer/lexer.h
        src/lexer/token.cpp
        src/lexer/token.h src/toy.cpp src/toy.h external/magic_enum.hpp)
