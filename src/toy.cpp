#include "Toy.h"

#include <iostream>
#include "Lexer/Lexer.h"
#include "Lexer/Parser.h"
#include "Lexer/AstPrinter.h"

#include "../external/magic_enum.hpp"
#include "Interpreter/Interpreter.h"

void Toy::run(const std::string& source) {
    Lexer lexer(*this, source);

    const auto tokens = lexer.scan_tokens();
	Parser parser(*this, tokens);
	//auto expression = parser.parse();

 //   for (const auto& token: tokens) {
 //       std::cout << token << "\n";
 //   }
	//if (m_has_error) {
	//	// exit with code 65
	//	return;
	//}

	////AstPrinter printer{};
	////std::cout << printer.print(expression) << "\n";

	//if (m_has_runtime_error) {
	//	// exit with code 70
	//	return;
	//}
	//Interpreter interpreter(*this);
	//interpreter.interpret(expression);



	auto statements = parser.parse();

	if (m_has_error) {
		// exit with code 65
		return;
	}

	if (m_has_runtime_error) {
		// exit with code 70
		return;
	}
	Interpreter interpreter(*this);
	interpreter.interpret(statements);
}

void Toy::run_prompt() {
    for (;;) {
        std::cout << "> ";
        std::string line;
        std::getline(std::cin, line);
        if (line.length() == 0) break;
        run(line);

		m_has_error = false;
		m_has_runtime_error = false;
    }
}

void Toy::runtime_error(RuntimeError error) {
	std::cout << "\n[line " << error.token().line() << "] " << error.what();
	m_has_runtime_error = true;
}
