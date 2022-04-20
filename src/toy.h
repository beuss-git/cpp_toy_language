#pragma once

#include <string>
#include <iostream>

class Lexer;

class Toy {
public:
    Toy() {
        m_has_error = false;
    }


    [[maybe_unused]] void run(const std::string& source);
    [[maybe_unused]] void run_prompt();

    friend Lexer;
private:
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
