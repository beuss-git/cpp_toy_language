#pragma once
#include <cassert>
#include <memory>
#include <string>

#include "../Value.h"

#include "Token.h"

class ExprVisitor;
class Expr {
public:
	virtual ValuePtr accept(ExprVisitor * visitor) {
		assert(false, "Not implemented");
		return {};
	}
};

using ExprPtr = std::shared_ptr<Expr>;

class Assign final : public Expr {
public:
	Assign(Token name, ExprPtr value)
		 : m_name(name), m_value(value) { }
	ValuePtr accept(ExprVisitor* visitor) override;

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
	ValuePtr accept(ExprVisitor* visitor) override;

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

class Call final : public Expr {
public:
	Call(ExprPtr callee, Token paren, std::vector<ExprPtr> arguments)
		 : m_callee(callee), m_paren(paren), m_arguments(arguments) { }
	ValuePtr accept(ExprVisitor* visitor) override;

	ExprPtr callee() const {
		return m_callee;
	}
	Token paren() const {
		return m_paren;
	}
	std::vector<ExprPtr> arguments() const {
		return m_arguments;
	}
private:
	ExprPtr m_callee{};
	Token m_paren{};
	std::vector<ExprPtr> m_arguments{};
};

class Grouping final : public Expr {
public:
	Grouping(ExprPtr expression)
		 : m_expression(expression) { }
	ValuePtr accept(ExprVisitor* visitor) override;

	ExprPtr expression() const {
		return m_expression;
	}
private:
	ExprPtr m_expression{};
};

class Literal final : public Expr {
public:
	Literal(ValuePtr value)
		 : m_value(value) { }
	ValuePtr accept(ExprVisitor* visitor) override;

	ValuePtr value() const {
		return m_value;
	}
private:
	ValuePtr m_value{};
};

class Logical final : public Expr {
public:
	Logical(ExprPtr left, Token op, ExprPtr right)
		 : m_left(left), m_op(op), m_right(right) { }
	ValuePtr accept(ExprVisitor* visitor) override;

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

class Unary final : public Expr {
public:
	Unary(Token op, ExprPtr right)
		 : m_op(op), m_right(right) { }
	ValuePtr accept(ExprVisitor* visitor) override;

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
	ValuePtr accept(ExprVisitor* visitor) override;

	Token name() const {
		return m_name;
	}
private:
	Token m_name{};
};

class ExprVisitor {
public:
	virtual ValuePtr visit_expr(Assign*) = 0;
	virtual ValuePtr visit_expr(Binary*) = 0;
	virtual ValuePtr visit_expr(Call*) = 0;
	virtual ValuePtr visit_expr(Grouping*) = 0;
	virtual ValuePtr visit_expr(Literal*) = 0;
	virtual ValuePtr visit_expr(Logical*) = 0;
	virtual ValuePtr visit_expr(Unary*) = 0;
	virtual ValuePtr visit_expr(Variable*) = 0;
};

inline ValuePtr Assign::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Binary::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Call::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Grouping::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Literal::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Logical::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Unary::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

inline ValuePtr Variable::accept(ExprVisitor* visitor) {
	return visitor->visit_expr(this);
}

