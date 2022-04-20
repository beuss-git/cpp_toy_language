#pragma once
#include <cassert>
#include <memory>
#include <string>

#include "../Value.h"

#include "Token.h"

class Visitor;
class Expr {
public:
	virtual Value accept(Visitor* visitor) {
		assert(false, "Not implemented");
		return {};
	}
};

using ExprPtr = std::shared_ptr<Expr>;

class Binary final : public Expr {
public:
	Binary(ExprPtr left, Token op, ExprPtr right)
		 : m_left(left), m_op(op), m_right(right) { }
	Value accept(Visitor* visitor) override;

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
	Value accept(Visitor* visitor) override;

	ExprPtr expression() const {
		return m_expression;
	}
private:
	ExprPtr m_expression{};
};

class Literal final : public Expr {
public:
	Literal(Value value)
		 : m_value(value) { }
	Value accept(Visitor* visitor) override;

	Value value() const {
		return m_value;
	}
private:
	Value m_value{};
};

class Unary final : public Expr {
public:
	Unary(Token op, ExprPtr right)
		 : m_op(op), m_right(right) { }
	Value accept(Visitor* visitor) override;

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
	virtual Value visit(Binary*) = 0;
	virtual Value visit(Grouping*) = 0;
	virtual Value visit(Literal*) = 0;
	virtual Value visit(Unary*) = 0;
};

inline Value Binary::accept(Visitor* visitor) {
	return visitor->visit(this);
}

inline Value Grouping::accept(Visitor* visitor) {
	return visitor->visit(this);
}

inline Value Literal::accept(Visitor* visitor) {
	return visitor->visit(this);
}

inline Value Unary::accept(Visitor* visitor) {
	return visitor->visit(this);
}

