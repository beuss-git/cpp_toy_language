#include "toy.h"

#include <iostream>
#include "lexer/lexer.h"
#include "../external/magic_enum.hpp"

void Toy::run(const std::string& source) {
    Lexer lexer(*this, source);
    const auto tokens = lexer.scan_tokens();

    for (const auto& token : tokens) {
        std::cout << token << "\n";
    }
}