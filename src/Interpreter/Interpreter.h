#pragma once
#include <utility>
#include <vector>
#include <thread>


#include "../Lexer/Errors.h"
#include "../Lexer/Expr.h"
#include "../Lexer/Stmt.h"
#include "../Lexer/Environment.h"
#include "../Toy.h"

class ToyCallable : public Value {
public:
	virtual int arity() = 0;
	virtual ValuePtr call(Interpreter*, std::vector<ValuePtr> arguments) = 0;
};

class ToyClock final : public ToyCallable {
public:
	int arity() override { return 0; }
	ValuePtr call(Interpreter*, std::vector<ValuePtr>) override {
		auto now = std::chrono::system_clock::now();
		auto seconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
		return create_value((double)seconds.count() / 1000.0);
	}
	std::string to_string() const override {
		return "<native fn>";
	}
};

class ReturnException : public std::exception {
public:
	ReturnException(ValuePtr value) : m_value(value) {}
	ValuePtr value() const {
		return m_value;
	}
private:
	ValuePtr m_value{nullptr};
};

class ToyFunction;

class Interpreter final : public ExprVisitor, public StmtVisitor {
public:
	Interpreter(Toy& toy) : m_toy(toy), m_globals({}), m_environment(&m_globals) {
		m_globals.define("clock", create_value<ToyClock>());
	}

	void interpret(const std::vector<StmtPtr>& statements) {
		try {
			for (auto statement : statements) {
				execute(statement);
			}
		}
		catch (const RuntimeError& e) {
			m_toy.runtime_error(e);
		}
		catch (const ReturnException&) {
			// TODO: print out value?
		}
	}
	Environment globals() const {
		return m_globals;
	}

	void execute(StmtPtr stmt) {
		stmt->accept(this);
	}

	// Takes copy of current environment
	void execute_block(std::vector<StmtPtr> statements, Environment environment) {
		auto previous = m_environment;
		try {
			m_environment = std::move(environment);
			for (auto statement : statements) {
				execute(statement);
			}
		}
		catch (const RuntimeError& err) {
			m_toy.runtime_error(err);
		}
		m_environment = previous;
	}
private:

	void visit_stmt(If* stmt) override {
		if (is_truthy(evaluate(stmt->condition()))) {
			execute(stmt->then_branch());
		}
		else if (stmt->else_branch()) {
			execute(stmt->else_branch());
		}
	}

	void visit_stmt(Block* stmt) override {
		execute_block(stmt->statements(), m_environment);
	}

	class BreakException : public std::exception {};
	void visit_stmt(Break*) override {
		throw BreakException{};
	}

	class ContinueException : public std::exception {};
	void visit_stmt(Continue*) override {
		throw ContinueException{};
	}

	ValuePtr visit_expr(Literal* expr) override {
		return expr->value();
	}
	ValuePtr visit_expr(Logical* expr) override {
		auto left = evaluate(expr->left());
		// Short circuit OR if lhs is true
		if (expr->op().type() == TokenType::OR) {
			if (is_truthy(left)) return left;
		}
		// Short circuit AND if lhs false
		else if (!is_truthy(left)) {
			return left;
		}
		return evaluate(expr->right());
	}
	ValuePtr visit_expr(Grouping* expr) override {
		return evaluate(expr->expression());
	}
	ValuePtr visit_expr(Unary* expr) override {
		auto right = evaluate(expr->right());
		switch(expr->op().type()) {
			case TokenType::BANG:
				return create_value(!is_truthy(right));
			case TokenType::MINUS:
				check_number_operand(expr->op(), right);
				return create_value(-right->as_double());
		}
		return nullptr;
	}
	ValuePtr visit_expr(Binary* expr) override {
		const auto left = evaluate(expr->left());
		const auto right = evaluate(expr->right());

		switch (expr->op().type()) {
			// Comparison
			case TokenType::GREATER:
				check_number_operands(expr->op(), left, right);
				return create_value(left->as_double() > right->as_double());
			case TokenType::GREATER_EQUAL:
				check_number_operands(expr->op(), left, right);
				return create_value(left->as_double() >= right->as_double());
			case TokenType::LESS:
				check_number_operands(expr->op(), left, right);
				return create_value(left->as_double() < right->as_double());
			case TokenType::LESS_EQUAL:
				check_number_operands(expr->op(), left, right);
				return create_value(left->as_double() <= right->as_double());

			case TokenType::BANG_EQUAL: 
				return create_value(left != right);
			case TokenType::EQUAL_EQUAL: 
				return create_value(left == right);

			// Arithmetic
			case TokenType::MINUS:
				check_number_operands(expr->op(), left, right);
				return create_value(left->as_double() - right->as_double());
			case TokenType::PLUS:
				if (left->is_number() && right->is_number()) {
					return create_value(left->as_double() + right->as_double());
				}
				//if (left.is_string() && right.is_string()) {
				//	return left.as_string() + right.as_string();
				//}
				// Allow either to be strings
				if (left->is_string() || right->is_string()) {
					return create_value(left->as_string() + right->as_string());
				}
				//throw RuntimeError(expr->op(), "Operands must be two numbers or two strings.");
				throw RuntimeError(expr->op(), "Invalid operands.");
			case TokenType::SLASH:
				check_number_operands(expr->op(), left, right);
				if (right->as_double() == 0.0) {
					throw RuntimeError(expr->op(), "Division by zero.");
				}
				return create_value(left->as_double() / right->as_double());
			case TokenType::STAR:
				check_number_operands(expr->op(), left, right);
				return create_value(left->as_double() * right->as_double());
		}
		return nullptr;
	}
	ValuePtr visit_expr(Call* expr) override {
		auto callee = evaluate(expr->callee());

		std::vector<ValuePtr> arguments{};
		arguments.reserve(expr->arguments().size());
		for (const auto& argument : expr->arguments()) {
			arguments.push_back(evaluate(argument));
		}

		if (auto* function = dynamic_cast<ToyCallable*>(callee.get()); function) {
			if (arguments.size() != function->arity()) {
				throw RuntimeError(expr->paren(),
					"Expected " + std::to_string(function->arity()) + " arguments but got "
					+ std::to_string(arguments.size()) + ".");
			}
			return function->call(this, arguments);
		}

		throw RuntimeError(expr->paren(), "Can only call functions and classes.");
	}

	void visit_stmt(Expression* stmt) override {
		evaluate(stmt->expression());
	}

	void visit_stmt(Function* stmt) {
		m_environment.define(stmt->name().lexeme(), create_value<ToyFunction>(stmt));
	}

	void visit_stmt(For* stmt) override {
		if (stmt->initializer()) {
			execute(stmt->initializer());
		}
		while (!stmt->condition() || is_truthy(evaluate(stmt->condition()))) {
			try {
				execute(stmt->body());
			}
			catch (const BreakException&) {
				break;
			}
			catch (const ContinueException&) { }

			if (stmt->increment()) {
				evaluate(stmt->increment());
			}
		}
	}

	void visit_stmt(Print* stmt) override {
		auto value = evaluate(stmt->expression());
		std::cout << value->to_string() << "\n";
	}


	void visit_stmt(Return* stmt) override {
		ValuePtr value = nullptr;
		if (stmt->value()) {
			value = evaluate(stmt->value());
		}
		else {
			value = create_value(nullptr);
		}
		throw ReturnException(value);
	}

	void visit_stmt(Sleep* stmt) override {
		auto value = evaluate(stmt->expression());
		if (!value->is_number()) {
			throw RuntimeError(stmt->token(), "sleep only accepts numbers");
		}
		std::this_thread::sleep_for(std::chrono::milliseconds((int)value->as_double()));
	}

	void visit_stmt(Var* stmt) override {
		// Don't require initializer, set to nil
		ValuePtr value = nullptr;
		if (stmt->initializer()) {
			value = evaluate(stmt->initializer());
		} else {
			value = create_value(nullptr);
		}
		m_environment.define(stmt->name().lexeme(), value);
	}

	void visit_stmt(While* stmt) override {
		while (is_truthy(evaluate(stmt->condition()))) {
			try {
				execute(stmt->body());
			}
			catch (const BreakException&) {
				break;
			}
			catch (const ContinueException&) { }
		}
	}

	ValuePtr visit_expr(Variable* expr) override {
		return m_environment.get(expr->name());
	}

	ValuePtr visit_expr(Assign* expr) {
		auto value = evaluate(expr->value());
		m_environment.assign(expr->name(), value);
		return value;
	}

	ValuePtr evaluate(ExprPtr expr) {
		return expr->accept(this);
	}
	bool is_truthy(ValuePtr value) {
		if (value->is_bool()) return value->as_bool();

		if (value->is_nil()) return false;

		return true;

	}

	void check_number_operand(Token op, ValuePtr operand) const {
		if (operand->is_number()) return;
		throw RuntimeError(op, "Operand must be a number.");
	}
	void check_number_operands(Token op, ValuePtr left, ValuePtr right) const {
		if (left->is_number() && right->is_number()) return;
		throw RuntimeError(op, "Operands must be numbers.");
	}

private:
	Toy& m_toy;
	Environment m_globals{};
	Environment m_environment{};
};

class ToyFunction final : public ToyCallable {
public:
	ToyFunction(Function* declaration) {
		m_declaration = declaration;
	}
	int arity() override { return m_declaration->params().size(); }
	ValuePtr call(Interpreter* interpreter, std::vector<ValuePtr> arguments) override {
		Environment environment(interpreter->globals());
		
		for (int i = 0; i < m_declaration->params().size(); i++) {
			environment.define(m_declaration->params().at(i).lexeme(), arguments.at(i));
		}
		try {
			interpreter->execute_block(m_declaration->body(), environment);
		} catch (const ReturnException& e) { 
			return e.value();
		}
		return create_value(nullptr);
	}
	std::string to_string() const override {
		return "<fn " + m_declaration->name().lexeme() + ">";
	}
private:
	Function* m_declaration;
};
