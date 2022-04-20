#include <iostream>
#include "toy.h"
#include "lexer/lexer.h"

int main() {
    Toy toy;
    toy.run_prompt();
    std::string source = R"(var language = "lox")";
    return 0;
}
