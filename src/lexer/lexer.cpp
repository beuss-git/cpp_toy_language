//
// Created by beuss on 20.04.22.
//

#include "lexer.h"

void Lexer::scan_token() {
    char c = advance();
    switch (c) {
        case '(':
            add_token(TokenType::LEFT_PAREN);
            break;
        case ')':
            add_token(TokenType::RIGHT_PAREN);
            break;
        case '{':
            add_token(TokenType::LEFT_BRACE);
            break;
        case '}':
            add_token(TokenType::RIGHT_BRACE);
            break;
        case ',':
            add_token(TokenType::COMMA);
            break;
        case '.':
            add_token(TokenType::DOT);
            break;
        case '-':
            add_token(TokenType::MINUS);
            break;
        case '+':
            add_token(TokenType::PLUS);
            break;
        case ';':
            add_token(TokenType::SEMICOLON);
            break;
        case '*':
            add_token(TokenType::STAR);
            break;

        case '!':
            add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
            break;
        case '=':
            add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '<':
            add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '/':
            if (match('/')) {  // Comment
                while (peek() != '\n' && !has_reached_end()) {
                    // Keep advancing until newline or end of file
                    advance();
                }
            } else {
                add_token(TokenType::SLASH);
            }
            break;

        // String literals
        case '"': string(); break;

        // Keywords
/*
        case 'o':
            if (match('r')) { add_token(TokenType::OR); }
            break;
*/
        case ' ':
        case '\r':
        case '\t':
            // Ignore whitespace
            break;
        case '\n':
            // New line
            m_line++;

        default:
            if (std::isdigit(c)){
                number();
            } else if(std::isalpha(c)){
                identifier();
            } else{
                m_toy.error(m_line, std::string("Unexpected character") + c);
            }
            break;
    }
}
