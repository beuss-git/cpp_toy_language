#include <iostream>
#include "toy.h"
#include "lexer/lexer.h"

int main() {
    Toy toy;

    //std::string source = R"(var language = "lox";)";
    std::string source = R"(
/* This is a comment*/
    var language = "lox";

/* this
is another comment
*/

    var langu= "lox2";
    )";
    //toy.run(source);
    toy.run_prompt();
    return 0;
}
