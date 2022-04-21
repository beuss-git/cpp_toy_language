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

class Block final : public Stmt {
public:
	Block(std::vector<StmtPtr> statements)
		 : m_statements(statements) { }
	void accept(StmtVisitor* visitor) override;

	std::vector<StmtPtr> statements() const {
		return m_statements;
	}
private:
	std::vector<StmtPtr> m_statements{};
};

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

class Var final : public Stmt {
public:
	Var(Token name, ExprPtr initializer)
		 : m_name(name), m_initializer(initializer) { }
	void accept(StmtVisitor* visitor) override;

	Token name() const {
		return m_name;
	}
	ExprPtr initializer() const {
		return m_initializer;
	}
private:
	Token m_name{};
	ExprPtr m_initializer{};
};

class StmtVisitor {
public:
	virtual void visit_stmt(Block*) = 0;
	virtual void visit_stmt(Expression*) = 0;
	virtual void visit_stmt(Print*) = 0;
	virtual void visit_stmt(Var*) = 0;
};

inline void Block::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Expression::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Print::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Var::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

