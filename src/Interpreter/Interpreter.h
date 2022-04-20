#pragma once
#include <vector>

#include "../Lexer/Expr.h"
#include "../Lexer/Stmt.h"
#include "../Toy.h"

class RuntimeError : public std::runtime_error {
public:
	RuntimeError(Token token, std::string message) : runtime_error(message.c_str()), m_token(token) { }
	Token token() const {
		return m_token;
	}
private:
	Token m_token{};
};

class Interpreter final : public ExprVisitor, public StmtVisitor {
public:
	Interpreter(Toy& toy) : m_toy(toy){}


	void interpret(const std::vector<StmtPtr>& statements) {
		try {
			for (auto statement : statements) {
				execute(statement);
			}
		}
		catch (const RuntimeError& e) {
			m_toy.runtime_error(e);
		}
	}

private:
	void execute(StmtPtr stmt) {
		stmt->accept(this);
	}

	Value visit_expr(Literal* expr) override {
		return expr->value();
	}
	Value visit_expr(Grouping* expr) override {
		return evaluate(expr->expression());
	}
	Value visit_expr(Unary* expr) override {
		auto right = evaluate(expr->right());
		switch(expr->op().type()) {
			case TokenType::BANG:
				return !is_truthy(right);
			case TokenType::MINUS:
				check_number_operand(expr->op(), right);
				return -right.as_double();
		}
		return nullptr;
	}
	Value visit_expr(Binary* expr) override {
		const auto left = evaluate(expr->left());
		const auto right = evaluate(expr->right());

		switch (expr->op().type()) {
			// Comparison
			case TokenType::GREATER:
				check_number_operands(expr->op(), left, right);
				return left.as_double() > right.as_double();
			case TokenType::GREATER_EQUAL:
				check_number_operands(expr->op(), left, right);
				return left.as_double() >= right.as_double();
			case TokenType::LESS:
				check_number_operands(expr->op(), left, right);
				return left.as_double() < right.as_double();
			case TokenType::LESS_EQUAL:
				check_number_operands(expr->op(), left, right);
				return left.as_double() <= right.as_double();

			case TokenType::BANG_EQUAL: 
				return left != right;
			case TokenType::EQUAL_EQUAL: 
				return left == right;

			// Arithmetic
			case TokenType::MINUS:
				check_number_operands(expr->op(), left, right);
				return left.as_double() - right.as_double();
			case TokenType::PLUS:
				if (left.is_number() && right.is_number()) {
					return left.as_double() + right.as_double();
				}
				//if (left.is_string() && right.is_string()) {
				//	return left.as_string() + right.as_string();
				//}
				// Allow either to be strings
				if (left.is_string() || right.is_string()) {
					return left.as_string() + right.as_string();
				}
				//throw RuntimeError(expr->op(), "Operands must be two numbers or two strings.");
				throw RuntimeError(expr->op(), "Invalid operands.");
			case TokenType::SLASH:
				check_number_operands(expr->op(), left, right);
				if (right.as_double() == 0.0) {
					throw RuntimeError(expr->op(), "Division by zero.");
				}
				return left.as_double() / right.as_double();
			case TokenType::STAR:
				check_number_operands(expr->op(), left, right);
				return left.as_double() * right.as_double();
		}
		return nullptr;
	}

	void visit_stmt(Expression* stmt) override {
		evaluate(stmt->expression());
	}

	void visit_stmt(Print* stmt) override {
		auto value = evaluate(stmt->expression());
		std::cout << value.to_string();
	}

	Value evaluate(ExprPtr expr) {
		return expr->accept(this);
	}
	bool is_truthy(Value value) {
		if (value.is_bool()) return value.as_bool();

		if (value.is_nil()) return false;

		return true;

	}

	void check_number_operand(Token op, Value operand) const {
		if (operand.is_number()) return;
		throw RuntimeError(op, "Operand must be a number.");
	}
	void check_number_operands(Token op, Value left, Value right) const {
		if (left.is_number() && right.is_number()) return;
		throw RuntimeError(op, "Operands must be numbers.");
	}

private:
	Toy& m_toy;
};