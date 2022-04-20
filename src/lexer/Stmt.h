#pragma once
#include <cassert>
#include <memory>
#include <string>

#include "../Value.h"

#include "Token.h"

class StmtVisitor;
class Stmt {
public:
	virtual void accept(StmtVisitor * visitor) {
		assert(false, "Not implemented");
	}
};

using StmtPtr = std::shared_ptr<Stmt>;

class Expression final : public Stmt {
public:
	Expression(ExprPtr expression)
		 : m_expression(expression) { }
	void accept(StmtVisitor* visitor) override;

	ExprPtr expression() const {
		return m_expression;
	}
private:
	ExprPtr m_expression{};
};

class Print final : public Stmt {
public:
	Print(ExprPtr expression)
		 : m_expression(expression) { }
	void accept(StmtVisitor* visitor) override;

	ExprPtr expression() const {
		return m_expression;
	}
private:
	ExprPtr m_expression{};
};

class StmtVisitor {
public:
	virtual void visit_stmt(Expression*) = 0;
	virtual void visit_stmt(Print*) = 0;
};

inline void Expression::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Print::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

