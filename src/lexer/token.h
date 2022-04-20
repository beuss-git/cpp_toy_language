#pragma once

#include <string>
#include <utility>
#include <any>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN,
    LEFT_BRACE, RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, OR,
    CLASS, SUPER, THIS,
    IF, ELSE,
    TRUE, FALSE, // Also literals
    FUN, RETURN,
    FOR, WHILE,
    VAR,
    NIL, // Also literal

    PRINT,

    TOKEN_EOF
};

class Token {
public:
    Token(TokenType type, std::string lexeme, std::any literal, int line) : m_type(type), m_lexeme(std::move(lexeme)),
                                                                            m_literal(std::move(literal)),
                                                                            m_line(line) {}

    [[nodiscard]] TokenType type() const { return m_type; }

    [[nodiscard]] std::string lexeme() const {
        return m_lexeme;
    }

private:
    TokenType m_type{TokenType::TOKEN_EOF};
    std::string m_lexeme{};
    std::any m_literal{};
    int m_line{-1};
};

std::ostream& operator<<(std::ostream& os, const Token& token);