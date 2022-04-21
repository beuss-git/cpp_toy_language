#include <iostream>
#include "Toy.h"
#include "Lexer/Lexer.h"
#include "Lexer/AstPrinter.h"
#include "Lexer/Parser.h"

int main() {

	//auto token = Token(TokenType::MINUS, "-", { Nil::NIL }, 1);
	//auto expression = create_expression<Binary>(
	//	create_expression<Unary>(
	//		Token(TokenType::MINUS, "-", {}, 1),
	//		create_expression<Literal>(123)
	//	),
	//	Token(TokenType::STAR, "*", {}, 1),
	//	create_expression<Grouping>(
	//		create_expression<Literal>(45.67)
	//	)
	//);

	//////auto expression = create_expression<Grouping>(
	//////	create_expression<Literal>(45.67)
	//////	);
	//AstPrinter printer{ };
	//std::cout << printer.print(expression) << "\n";
	//return 1;
    Toy toy;

    //std::string source = R"(var language = "lox";)";
    //std::string source = R"(4+6)";
    //std::string source = R"("Hello, " + "World!")";
    //std::string source = R"("Hello: " + 5)";
    //std::string source = R"(1 + 1)";
    //std::string source = R"(6 / 2*(1+2))";
    //std::string source = R"(6 / 0)";
    //std::string source = R"(print "true";)";
 //   std::string source = R"(
	//a = "after";
	//var a = "before";
	//print a;
	//a = "after";
	//print a;
	//)";

    std::string source = R"(
var a = "global a";
var b = "global b";
var c = "global c";
{
  var a = "outer a";
  var b = "outer b";
  {
    var a = "inner a";
    print a;
    print b;
    print c;
  }
  print a;
  print b;
  print c;
}
print a;
print b;
print c;
	)";
    //std::string source = R"("test" == "othertest")";
    //std::string source = R"("test" == "othertest" != 3 * 3 > 4)";
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
