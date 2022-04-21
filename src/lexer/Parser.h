#pragma once
#include <memory>
#include <vector>
#include <algorithm>
#include "Expr.h"
#include "Stmt.h"
#include "Token.h"
#include "Errors.h"
#include "../Toy.h"

/*
	program        → statement* EOF ;

	declaration    → varDecl
				   | statement ;

	varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;

	statement      → exprStmt
				   | ifStmt
				   | printStmt
				   | sleepStmt
				   | whileStmt
				   | block ;

	whileStmt      → "while" "(" expression ")" statement ;

	ifStmt         → "if" "(" expression ")" statement
				   ( "else" statement )? ;

	block          → "{" declaration* "}" ;

	exprStmt       → expression ";" ;
	printStmt      → "print" expression ";" ;
	sleepStmt      → "sleep" expression ";" ;

	expression     → assignment ;
	assignment     → IDENTIFIER "=" assignment
				   | logic_or ;

    logic_or       → logic_and ( "or" logic_and )* ;
	logic_and      → equality ( "and" equality )* ;

	equality       → comparison ( ( "!=" | "==" ) comparison )* ;
	comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
	term           → factor ( ( "-" | "+" ) factor )* ;
	factor         → unary ( ( "/" | "*" ) unary )* ;
	unary          → ( "!" | "-" ) unary
				   | primary ;
	primary        → NUMBER | STRING | "true" | "false" | "nil"
				   | "(" expression ")"
				   | IDENTIFIER ;
 */


template<typename T, typename... Args>
static ExprPtr create_expression(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T, typename... Args>
static StmtPtr create_statement(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

class Parser {
public:
	Parser(Toy& toy, const std::vector<Token> tokens) : m_toy(toy), m_tokens(tokens) {}

	std::vector<StmtPtr> parse() {

		try {
			std::vector<StmtPtr> statements{};

			while (!has_reached_end()) {
				statements.push_back(declaration());
			}
			return statements;
		} catch (const ParseError& error) {
			return {};
		}
		//try {
		//	return expression();
		//}
		//catch (const ParseError& error) {
		//	return nullptr;
		//}
	}

private:

	bool has_reached_end() const {
		return m_tokens.at(m_current).type() == TokenType::TOKEN_EOF;
	}
	Token peek() const {
		return m_tokens.at(m_current);
	}
	Token previous() const {
		return m_tokens.at(m_current - 1);
	}
	Token advance() {
		if (!has_reached_end()) m_current++;
		return previous();
	}
	bool check(TokenType type) {
		if (has_reached_end()) return false;
		return peek().type() == type;
	}
	template <typename... Types>
	bool match(Types&&... types) {
		return (... || [&](const TokenType& type) {
			if (check(type)) {
				advance();
				return true;
			}
			return false;
		}(types));
	}
	Token consume(TokenType type, std::string message) {
		if (check(type)) return advance();

		throw error(peek(), message);
	}
	ParseError error(Token token, std::string message) {
		m_toy.error(token, message);
		return ParseError{};
	}
	/*
	 * Binary operators
	 */

	// expression     → assignment ;
	ExprPtr expression() {
		return assignment();
	}
	// assignment     → IDENTIFIER "=" assignment
	//				| logic_or ;
	ExprPtr assignment() {
		auto expr = logic_or();
		if (match(TokenType::EQUAL)) {
			Token equals = previous();
			auto value = assignment();

			if (typeid(*expr).name() == typeid(Variable).name()) {
				Token name = ((Variable*)(expr.get()))->name();
				return create_expression<Assign>(name, value);
			}

			error(equals, "Invalid assignment target.");
		}
		return expr;
	}

	// logic_or       → logic_and ( "or" logic_and )* ;
	ExprPtr logic_or() {
		auto expr = logic_and();
		while (match(TokenType::OR)) {
			Token op = previous();
			auto right = logic_and();
			expr = create_expression<Logical>(expr, op, right);
		}
		return expr;
	}

	// logic_and      → equality ( "and" equality )* ;
	ExprPtr logic_and() {
		auto expr = equality();
		while (match(TokenType::AND)) {
			Token op = previous();
			auto right = equality();
			expr = create_expression<Logical>(expr, op, right);
		}
		return expr;
	}

	// equality       → comparison ( ( "!=" | "==" ) comparison )* ;
	ExprPtr equality() {
		ExprPtr expr = comparison();

		while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
			Token op = previous();
			ExprPtr right = comparison();
			expr = create_expression<Binary>(expr, op, right);
		}
		return expr;
	}

	// comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
	ExprPtr comparison() {
		ExprPtr expr = term();

		while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
			Token op = previous();
			ExprPtr right = term();
			expr = create_expression<Binary>(expr, op, right);
		}
		return expr;
	}
	// term           → factor ( ( "-" | "+" ) factor )* ;
	ExprPtr term() {
		ExprPtr expr = factor();
		while (match(TokenType::MINUS, TokenType::PLUS)) {
			Token op = previous();
			ExprPtr right = factor();
			expr = create_expression<Binary>(expr, op, right);
		}
		return expr;
	}

	// factor         → unary ( ( "/" | "*" ) unary )* ;
	ExprPtr factor() {
		ExprPtr expr = unary();
		while (match(TokenType::SLASH, TokenType::STAR)) {
			Token op = previous();
			ExprPtr right = unary();
			expr = create_expression<Binary>(expr, op, right);
		}
		return expr;
	}

	/*
	 * Unary operators
	 */
	// unary          → ( "!" | "-" ) unary
	//				| primary ;
	ExprPtr unary() {
		if (match(TokenType::BANG, TokenType::MINUS)) {
			Token op = previous();
			ExprPtr right = unary();
			return create_expression<Unary>(op, right);
		}
		return primary();
	}


	// primary        → NUMBER | STRING | "true" | "false" | "nil"
	//				   | "(" expression ")"
	//				   | IDENTIFIER ;
	ExprPtr primary() {
		if (match(TokenType::FALSE)) return create_expression<Literal>(false);
		if (match(TokenType::TRUE)) return create_expression<Literal>(true);
		if (match(TokenType::NIL)) return create_expression<Literal>(nullptr);

		if (match(TokenType::NUMBER, TokenType::STRING)) {
			return create_expression<Literal>(previous().literal());
		}

		if (match(TokenType::IDENTIFIER)) {
			return create_expression<Variable>(previous());
		}

		if (match(TokenType::LEFT_PAREN)) {
			ExprPtr expr = expression();
			consume(TokenType::RIGHT_PAREN, "Expect ')' after expression");
			return create_expression<Grouping>(expr);
		}

		throw error(peek(), "Expect expression.");
	}

	// declaration    → varDecl
	//				| statement ;
	StmtPtr declaration() {
		try {
			if (match(TokenType::VAR)) return var_declaration();
			return statement();
		} catch (const ParseError& error) {
			synchronize();
			return nullptr;
		}
	}
	// varDecl        → "var" IDENTIFIER ( "=" expression )? ";" ;
	StmtPtr var_declaration() {
		Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");
		ExprPtr initializer = nullptr;
		if (match(TokenType::EQUAL)) {
			initializer = expression();
		}
		consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
		return create_statement<Var>(name, initializer);
	}

	// statement      → exprStmt
	//				| ifStmt
	//				| printStmt
	//				| whileStmt
	//				| block ;
	StmtPtr statement() {
		if (match(TokenType::IF)) return if_statement();
		if (match(TokenType::PRINT)) return print_statement();
		if (match(TokenType::SLEEP)) return sleep_statement();
		if (match(TokenType::WHILE)) return while_statement();
		if (match(TokenType::LEFT_BRACE)) return block();
		return expression_statement();
	}


	// ifStmt         → "if" "(" expression ")" statement
	//				   ( "else" statement )? ;
	StmtPtr if_statement() {
		consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
		ExprPtr expr = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

		StmtPtr if_branch = statement();
		
		StmtPtr else_branch = nullptr;

		if (match(TokenType::ELSE)) {
			else_branch = statement();
		}

		return create_statement<If>(expr, if_branch, else_branch);
	}
	// whileStmt      → "while" "(" expression ")" statement ;
	StmtPtr while_statement() {
		consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
		auto condition = expression();
		consume(TokenType::RIGHT_PAREN, "Expect ')' after while condition.");

		auto body = statement();
		return create_statement<While>(condition, body);
	}

	// block          → "{" declaration* "}" ;
	StmtPtr block() {
		std::vector<StmtPtr> statements{};
		while (!check(TokenType::RIGHT_BRACE) && !has_reached_end()) {
			statements.push_back(declaration());
		}

		consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
		return create_statement<Block>(statements);
	}

	// exprStmt       → expression ";" ;
	StmtPtr expression_statement() {
		auto expr = expression();
		consume(TokenType::SEMICOLON, "Expect ';' after expression.");
		return create_statement<Expression>(expr);
	}

	// printStmt      → "print" expression ";" ;
	StmtPtr print_statement() {
		auto value = expression();
		consume(TokenType::SEMICOLON, "Expect ';' after value.");
		return create_statement<Print>(value);
	}

	// sleepStmt      → "sleep" expression ";" ;
	StmtPtr sleep_statement() {
		auto value = expression();
		consume(TokenType::SEMICOLON, "Expect ';' after value.");
		return create_statement<Sleep>(previous(), value);
	}

	// Synchronize to a 'valid' state
	void synchronize() {
		advance();

		// We keep advancing until we get to the start of a new statement or hit end of statement (semicolon)
		while (!has_reached_end()) {
			if (previous().type() == TokenType::SEMICOLON) return;
			switch (peek().type()) {
				case TokenType::CLASS:
				case TokenType::FUN:
				case TokenType::VAR:
				case TokenType::FOR:
				case TokenType::IF:
				case TokenType::WHILE:
				case TokenType::PRINT:
				case TokenType::RETURN:
					return;
			}
			advance();
		}

	}

private:
	Toy& m_toy;
	std::vector<Token> m_tokens{};
	int m_current{ 0 };
};