#pragma once
#include "Token.h"

class RuntimeError : public std::runtime_error {
public:
	RuntimeError(Token token, std::string message) : runtime_error(message.c_str()), m_token(token) { }
	Token token() const {
		return m_token;
	}
private:
	Token m_token{};
};

class ParseError : public std::exception { };
