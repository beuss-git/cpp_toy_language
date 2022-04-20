#pragma once
#include <cassert>
#include <variant>
#include <string>

#include "Token.h"

class Visitor;
class Expr {
public:
	virtual std::string accept(Visitor* visitor) {
		assert(false, "Not implemented");
		return {};
	}
};

class Binary final : public Expr {
public:
	Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
		 : m_left(left), m_op(op), m_right(right) { }
	std::string accept(Visitor* visitor) override;

	std::shared_ptr<Expr> left() const {
		return m_left;
}
	Token op() const {
		return m_op;
}
	std::shared_ptr<Expr> right() const {
		return m_right;
}
private:
	std::shared_ptr<Expr> m_left{};
	Token m_op{};
	std::shared_ptr<Expr> m_right{};
};

class Grouping final : public Expr {
public:
	Grouping(std::shared_ptr<Expr> expression)
		 : m_expression(expression) { }
	std::string accept(Visitor* visitor) override;

	std::shared_ptr<Expr> expression() const {
		return m_expression;
}
private:
	std::shared_ptr<Expr> m_expression{};
};

class Literal final : public Expr {
public:
	Literal(std::any value)
		 : m_value(value) { }
	std::string accept(Visitor* visitor) override;

	std::any value() const {
		return m_value;
}
private:
	std::any m_value{};
};

class Unary final : public Expr {
public:
	Unary(Token op, std::shared_ptr<Expr> right)
		 : m_op(op), m_right(right) { }
	std::string accept(Visitor* visitor) override;

	Token op() const {
		return m_op;
}
	std::shared_ptr<Expr> right() const {
		return m_right;
}
private:
	Token m_op{};
	std::shared_ptr<Expr> m_right{};
};

class Visitor {
public:
	virtual std::string visit(Binary*) = 0;
	virtual std::string visit(Grouping*) = 0;
	virtual std::string visit(Literal*) = 0;
	virtual std::string visit(Unary*) = 0;
};

inline std::string Binary::accept(Visitor* visitor) {
	return visitor->visit(this);
}

inline std::string Grouping::accept(Visitor* visitor) {
	return visitor->visit(this);
}

inline std::string Literal::accept(Visitor* visitor) {
	return visitor->visit(this);
}

inline std::string Unary::accept(Visitor* visitor) {
	return visitor->visit(this);
}

