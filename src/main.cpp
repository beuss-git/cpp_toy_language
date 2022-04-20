#include <iostream>
#include "Toy.h"
#include "Lexer/Lexer.h"
#include "Lexer/AstPrinter.h"

template<class T, class... Args>
static std::shared_ptr<T> create_expression(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}
int main() {

	//auto token = Token(TokenType::MINUS, "-", { Nil::NIL }, 1);
	auto expression = create_expression<Binary>(
		create_expression<Unary>(
			Token(TokenType::MINUS, "-", {}, 1),
			create_expression<Literal>(123)
		),
		Token(TokenType::STAR, "*", {}, 1),
		create_expression<Grouping>(
			create_expression<Literal>(45.67)
		)
	);

	////auto expression = create_expression<Grouping>(
	////	create_expression<Literal>(45.67)
	////	);
	AstPrinter printer{ };
	std::cout << printer.print(expression) << "\n";
	return 1;
    Toy toy;

    std::string source = R"(var language = "lox";)";
    //std::string source = R"(
///* This is a comment*/
    //var language = "lox";
//
///* this
//is another comment
//*/

    //var langu= "lox2";
    //)";
    toy.run(source);
    //toy.run_prompt();
    return 0;
}
