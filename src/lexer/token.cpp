#include "Token.h"
#include <sstream>
#include <iomanip>
#include "../../external/magic_enum.hpp"

std::ostream& operator<<(std::ostream& os, const Token& token) {
    const auto type_name = magic_enum::enum_name(token.type());
	const auto lexeme = token.lexeme();

    std::stringstream ss;
	ss << "Type: " << type_name;

	if (token.is_literal()) {
		 ss << std::setw(20ll - static_cast<std::streamsize>(type_name.length())) << "Lexeme: " << lexeme;
	}

    return os << ss.str();
}
