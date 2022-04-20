#pragma once
#include <cassert>
#include <memory>
#include <string>

#include "Token.h"
#include "../Value.h"

class Visitor;
class Expr {
public:
	virtual std::string accept(Visitor* visitor) {
		assert(false, "Not implemented");
		return {};
	}
};

using ExprPtr = std::shared_ptr<Expr>;

class Binary final : public Expr {
public:
	Binary(ExprPtr left, Token op, ExprPtr right)
		 : m_left(left), m_op(op), m_right(right) { }
	std::string accept(Visitor* visitor) override;

	ExprPtr left() const {
		return m_left;
}
	Token op() const {
		return m_op;
}
	ExprPtr right() const {
		return m_right;
}
private:
	ExprPtr m_left{};
	Token m_op{};
	ExprPtr m_right{};
};

class Grouping final : public Expr {
public:
	Grouping(ExprPtr expression)
		 : m_expression(expression) { }
	std::string accept(Visitor* visitor) override;

	ExprPtr expression() const {
		return m_expression;
}
private:
	ExprPtr m_expression{};
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
	Unary(Token op, ExprPtr right)
		 : m_op(op), m_right(right) { }
	std::string accept(Visitor* visitor) override;

	Token op() const {
		return m_op;
}
	ExprPtr right() const {
		return m_right;
}
private:
	Token m_op{};
	ExprPtr m_right{};
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

