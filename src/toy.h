#pragma once

#include <string>
#include <iostream>

#include "Lexer/Errors.h"
#include "Lexer/Token.h"

class Lexer;
class Parser;
class Interpreter;


class Toy {
public:
	Toy() = default;

    [[maybe_unused]] void run(const std::string& source);

    [[maybe_unused]] void run_prompt();

    friend Lexer;
	friend Parser;
	friend Interpreter;
private:
	void runtime_error(RuntimeError error);

    void error(Token token, std::string message) {
		if (token.type() == TokenType::TOKEN_EOF) {
		  report(token.line(), " at end", message);
		} else {
		  report(token.line(), " at '" + token.lexeme() + "'", message);
		}
	  }
    void error(int line, const std::string& message) {
        report(line, "", message);
    }

    void report(int line, const std::string& where, const std::string& message) {
        std::cout << "[line " << line << "] Error" << where << ": " << message << "\n";
        m_has_error = true;
    }

private:
	bool m_has_error{ false };
	bool m_has_runtime_error{ false };
};
