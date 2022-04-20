#include "toy.h"

#include <iostream>
#include "lexer/lexer.h"
#include "../external/magic_enum.hpp"

void Toy::run(const std::string& source) {
    Lexer lexer(*this, source);
    const auto tokens = lexer.scan_tokens();

    for (const auto& token: tokens) {
        std::cout << token << "\n";
    }
}

void Toy::run_prompt() {
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.length() == 0) break;
        run(line);
    }
}
