#pragma once
#include <cassert>
#include <memory>
#include <string>

#include "../Value.h"

#include "Token.h"

class ExprVisitor;
class Expr {
public:
	virtual Value accept(ExprVisitor * visitor) {
		assert(false, "Not implemented");
return {};
	}
};

using ExprPtr = std::shared_ptr<Expr>;

class Assign final : public Expr {
public:
	Assign(Token name, ExprPtr value)
		 : m_name(name), m_value(value) { }
	Value accept(ExprVisitor* visitor) override;

	Token name() const {
		return m_name;
	}
	ExprPtr value() const {
		return m_value;
	}
private:
	Token m_name{};
	ExprPtr m_value{};
};

class Binary final : public Expr {
public:
	Binary(ExprPtr left, Token op, ExprPtr right)
		 : m_left(left), m_op(op), m_right(right) { }
	Value accept(ExprVisitor* visitor) override;

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
	Value accept(ExprVisitor* visitor) override;

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
	Value accept(ExprVisitor* visitor) override;

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
	Value accept(ExprVisitor* visitor) override;

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

class Variable final : public Expr {
public:
	Variable(Token name)
		 : m_name(name) { }
	Value accept(ExprVisitor* visitor) override;

	Token name() const {
		return m_name;
	}
private:
	Token m_name{};
};

class ExprVisitor {
public:
	virtual Value visit_expr(Assign*) = 0;
	virtual Value visit_expr(Binary*) = 0;
	virtual Value visit_expr(Grouping*) = 0;
	virtual Value visit_expr(Literal*) = 0;
	virtual Value visit_expr(Unary*) = 0;
	virtual Value visit_expr(Variable*) = 0;
};

inline Value Assign::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline Value Binary::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline Value Grouping::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline Value Literal::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline Value Unary::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline Value Variable::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

