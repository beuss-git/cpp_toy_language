#pragma once

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include "Token.h"
#include "../Toy.h"
#include "../Value.h"


class Lexer {
public:
    Lexer(Toy& toy, std::string source) : m_toy(toy), m_source(std::move(source)) {}

    const std::vector<Token>& scan_tokens() {
        while (!has_reached_end()) {
            m_start = m_current;
            scan_token();
        }
        m_tokens.emplace_back(TokenType::TOKEN_EOF, "", nullptr, m_line);

        return m_tokens;
    }

private:
    char advance() {
        return m_source.at(m_current++);
    }

    bool match(char expected) {
        if (has_reached_end()) return false;
        if (m_source.at(m_current) != expected) return false;
        m_current++;
        return true;
    }

    char peek() const {
        if (has_reached_end()) return '\0';
        return m_source.at(m_current);
    }

    char peek_next() const {
        if (m_current + 1 >= m_source.length()) return '\0';
        return m_source.at(m_current + 1);
    }

    // TODO: use stack to allow nested comments
    void block_comment() {
        // Advance until we find the closing */
        while (peek() != '*' && peek_next() != '/' && !has_reached_end()) {
            // We allow newlines in comment
            if (peek() == '\n') m_line++;
            advance();
        }
        if (has_reached_end()) {
            m_toy.error(m_line, "Unterminated string");
            return;
        }
        // Advance closing * /
        advance();
        advance();

        std::string value = m_source.substr(m_start + 1, m_current - 1 - m_start);
        add_token(TokenType::STRING, value);
    }

    void string() {
        // Advance until we find the closing "
        while (peek() != '"' && !has_reached_end()) {
            // We allow newlines in string
            if (peek() == '\n') m_line++;
            advance();
        }
        if (has_reached_end()) {
            m_toy.error(m_line, "Unterminated string");
            return;
        }
        // Advance closing "
        advance();

        std::string value = m_source.substr(m_start + 1, m_current - 1 - m_start - 1);
        add_token(TokenType::STRING, value);
    }

    void number() {
        while (std::isdigit(peek())) { advance(); }

        if (peek() == '.' && std::isdigit(peek_next())) {
            advance();
            while (std::isdigit(peek())) { advance(); }
        }

        std::string number_str = m_source.substr(m_start, m_current - m_start);

        add_token(TokenType::NUMBER, std::stod(number_str));
    }

    bool isalphanumeric(char c) {
        return std::isalpha(c) || std::isdigit(c);
    }

    void identifier() {
        while (isalphanumeric(peek())) {
            advance();
        }
        std::string text = m_source.substr(m_start, m_current - m_start);
        TokenType type = TokenType::IDENTIFIER;

        if (auto it = m_keywords.find(text); it != m_keywords.end()) {
            type = it->second;
        }

        add_token(type);
    }

    void scan_token();

    void add_token(TokenType type) {
        add_token(type, nullptr);
    }

    void add_token(TokenType type, Value literal) {
        std::string text = m_source.substr(m_start, m_current - m_start);
        m_tokens.emplace_back(type, text, literal, m_line);
    }

    bool has_reached_end() const {
        return m_current >= m_source.length();
    }

private:
    Toy& m_toy;
    std::string m_source{};
    std::vector<Token> m_tokens{};

    int m_start{0};
    int m_current{0};
    int m_line{0};

    std::unordered_map<std::string, TokenType> m_keywords{
            {"and",    TokenType::AND},
            {"or",     TokenType::OR},
            {"class",  TokenType::CLASS},
            {"super",  TokenType::SUPER},
            {"this",   TokenType::THIS},
            {"if",     TokenType::IF},
            {"else",   TokenType::ELSE},
            {"true",   TokenType::TRUE},
            {"false",  TokenType::FALSE},
            {"fun",    TokenType::FUN},
            {"return", TokenType::RETURN},
            {"for",    TokenType::FOR},
            {"while",  TokenType::WHILE},
            {"var",    TokenType::VAR},
            {"nil",    TokenType::NIL},
            {"print",  TokenType::PRINT},
    };
};
