#include "token.h"
#include <sstream>
#include "../../external/magic_enum.hpp"

std::ostream& operator<<(std::ostream& os, const Token& token) {
    auto type_name = magic_enum::enum_name(token.type());
    std::stringstream ss;
    ss << type_name << "[" << token.lexeme() << "]";
    return os << ss.str();
}
