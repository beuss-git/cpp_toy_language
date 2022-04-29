cmake_minimum_required(VERSION 3.22)
project(cpp_toy_language)

set(CMAKE_CXX_STANDARD 20)

add_executable(cpp_toy_language
        "src/main.cpp"
        "src/Toy.cpp"
        "src/Toy.h" 
		"src/Value.h"
		"src/Lexer/AstPrinter.h"
        "src/Lexer/Environment.h"
		"src/Lexer/Errors.h"
        "src/Lexer/Expr.h"
        "src/Lexer/Stmt.h"
        "src/Lexer/Lexer.cpp"
        "src/Lexer/Lexer.h"
		"src/Lexer/Parser.h"
        "src/Lexer/Token.cpp"
        "src/Lexer/Token.h"
		"src/Interpreter/Interpreter.h"
		"src/Interpreter/Interpreter.cpp"
        "external/magic_enum.hpp"
	   )

#add_subdirectory(tools/expression_generator)
#target_compile_options(cpp_toy_language PRIVATE /W4 /EHa /GL /O2 /DEBUG)
target_compile_options(
    cpp_toy_language PRIVATE 
    /W3 /nologo /EHsc
    "$<$<CONFIG:Debug>:/MTd;/Od;/Ob0;/Zi;/RTC1;/DDEBUG;/D_DEBUG>"
    "$<$<CONFIG:Release>:/MT;/O2;/Ob2;/DNDEBUG>"
)
#target_compile_options(cpp_toy_language PRIVATE /DEBUG:FULL)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FULL /Zi /OPT:REF /OPT:ICF")
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /DEBUG:FULL")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Zi")
#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL /OPT:REF /OPT:ICF /PROFILE")
#set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DEBUG:FULL")
#set_target_properties(${BUILD_TARGET} PROPERTIES LINK_FLAGS "/PROFILE")