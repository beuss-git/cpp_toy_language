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

//    std::string source = R"(
//var a = "global a";
//{
//	//var a = "inner a";
//	print b;
//	print a;
//	var b;
//}
//print a;
//	)";
//
//    std::string source = R"(
//var a = 1;
//{
//  var a = a + 2;
//  print a;
//}
//print a;
//	)";

 //   std::string source = R"(
	////var a = 3;
	////if (a == 2 or 1 == 1)
	////	print "a == 1";
	//var counter = 1;
	//while (counter < 100){
	//	//sleep 100;
	//	counter = counter + 1;
	//	print "hello, world!: " + counter;
	//}
	//)";


 //   std::string source = R"(

	//for (;;) {
	//	print "hello, world!1: ";
	//	print "hello, world!2: ";
	//}
	//for (var i = 0; i < 10; i = i + 1) {
	//	print "hello, world!: " + i;
	//}

	//for (var i = 0; i < 10; ) {
	//	print "hello, world!: " + i;
	//}
	////for (var i = 0; ; i = i + 1) {
	////	print "hello, world!: " + i;
	////}
	//)";

 //   std::string source = R"(
	//for (var i = 0; i < 10; i = i + 1) {
	//	print "hello, world!: " + i;
	//	
	//	continue;
	//	print "hello, world!2: " + i;
	//}
	//)";


//    std::string source = R"(
//while (true) {
//	sleep 100;
//	print clock();
//}
//	)";

//    std::string source = R"(
//fun test(p) {
//	return p;
//}
////return;
//print test(3);
//	)";


//    std::string source = R"(
//fun fib(n) {
//	if (n <= 0) {
//		return 1;
//	}
//	var val = fib(n - 1);
//	print val;
//	return val;
//}
//
//var a = 5;
//fib(a);
////print a;
//	)";
//    std::string source = R"(
//fun fib(n) {
//	if (n <= 1) {
//		//print "returning " + n;
//		return n;
//	}
//	var a = fib(n - 2);
//	var b = fib(n - 1);
//	return a + b;
//}
//
////for (var i = 0; i < 20; i = i + 1) {
////  print fib(i);
////}
//print fib(9);
//	)";
//    std::string source = R"(
//fun makeCounter() {
//  var i = 0;
//  fun count() {
//    i = i + 1;
//    print i;
//  }
//
//  return count;
//}
//
//var counter = makeCounter();
//counter(); // "1".
//counter(); // "2".
//	)";
//std::string source = R"(
//fun invoke(a){
//	a();
//}
//invoke(fun (){print "hello";});
//)";


//    std::string source = R"(
//fun fib(n) {
//	var a = 0;
//	var b = 1;
//
//	//var tmp;
//	for (var i = 0; i < n; i = i + 1) {
//		var tmp = a;
//		a = b;
//		b = tmp + b;
//	}
//	return a;
//}
//
//var before = clock();
////print fib(10);
//fib(1000000);
//var after = clock();
//print after - before + " seconds";
//	)";
    std::string source = R"(
fun fib(n) {
	if (n <= 1) {
		return n;
	}
	return fib(n - 2) + fib (n - 1);
}

var before = clock();
print fib(35);
var after = clock();
print after - before + " seconds";
	)";
 //   std::string source = R"(
	//var a = 1;
	////print a;

	//fun hello_world(hello, world) {
	//	print hello + world;
	//}
	//fun test(aparam) {
	//	aparam = 3;
	//	print aparam;
	//}
	//hello_world("hello, ", "world!");

	//// This should error
	////a();
	////test(a);
	////print a;
	////print test();
	////print test;
	//)";
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
