#include "Toy.h"

#include <iostream>
#include "Lexer/Lexer.h"
#include "Lexer/Parser.h"
#include "Lexer/AstPrinter.h"

#include "../external/magic_enum.hpp"

void Toy::run(const std::string& source) {
    Lexer lexer(*this, source);

    const auto tokens = lexer.scan_tokens();
	Parser parser(*this, tokens);
	ExprPtr expression = parser.parse();

    //for (const auto& token: tokens) {
    //    std::cout << token << "\n";
    //}
	if (m_has_error) {
		return;
	}

	AstPrinter printer{};
	std::cout << printer.print(expression) << "\n";

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
