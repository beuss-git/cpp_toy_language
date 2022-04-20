#pragma once

#include <string>
#include <iostream>

class Lexer;

class Toy {
public:
    Toy() {
        m_has_error = false;
    }

    void run_prompt() {
        for (;;) {
            std::cout << "> ";
            std::string line;
            std::getline(std::cin, line);
            if (line.length() == 0) break;
            run(line);
        }
    }

    friend Lexer;
private:
    void run(const std::string& source);

    void error(int line, const std::string& message) {
        report(line, "", message);
    }

    void report(int line, const std::string& where, const std::string& message) {
        std::cout << "[line " << line << "] Error" << where << ": " << message << "\n";
        m_has_error = true;
    }

private:
    bool m_has_error;
};
