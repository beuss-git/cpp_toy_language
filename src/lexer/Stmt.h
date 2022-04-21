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

class Break final : public Stmt {
public:
	Break() {} 
	void accept(StmtVisitor* visitor) override;

private:
};

class Continue final : public Stmt {
public:
	Continue() {} 
	void accept(StmtVisitor* visitor) override;

private:
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

class For final : public Stmt {
public:
	For(StmtPtr initializer, ExprPtr condition, ExprPtr increment, StmtPtr body)
		 : m_initializer(initializer), m_condition(condition), m_increment(increment), m_body(body) { }
	void accept(StmtVisitor* visitor) override;

	StmtPtr initializer() const {
		return m_initializer;
	}
	ExprPtr condition() const {
		return m_condition;
	}
	ExprPtr increment() const {
		return m_increment;
	}
	StmtPtr body() const {
		return m_body;
	}
private:
	StmtPtr m_initializer{};
	ExprPtr m_condition{};
	ExprPtr m_increment{};
	StmtPtr m_body{};
};

class If final : public Stmt {
public:
	If(ExprPtr condition, StmtPtr then_branch, StmtPtr else_branch)
		 : m_condition(condition), m_then_branch(then_branch), m_else_branch(else_branch) { }
	void accept(StmtVisitor* visitor) override;

	ExprPtr condition() const {
		return m_condition;
	}
	StmtPtr then_branch() const {
		return m_then_branch;
	}
	StmtPtr else_branch() const {
		return m_else_branch;
	}
private:
	ExprPtr m_condition{};
	StmtPtr m_then_branch{};
	StmtPtr m_else_branch{};
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

class Sleep final : public Stmt {
public:
	Sleep(Token token, ExprPtr expression)
		 : m_token(token), m_expression(expression) { }
	void accept(StmtVisitor* visitor) override;

	Token token() const {
		return m_token;
	}
	ExprPtr expression() const {
		return m_expression;
	}
private:
	Token m_token{};
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

class While final : public Stmt {
public:
	While(ExprPtr condition, StmtPtr body)
		 : m_condition(condition), m_body(body) { }
	void accept(StmtVisitor* visitor) override;

	ExprPtr condition() const {
		return m_condition;
	}
	StmtPtr body() const {
		return m_body;
	}
private:
	ExprPtr m_condition{};
	StmtPtr m_body{};
};

class StmtVisitor {
public:
	virtual void visit_stmt(Block*) = 0;
	virtual void visit_stmt(Break*) = 0;
	virtual void visit_stmt(Continue*) = 0;
	virtual void visit_stmt(Expression*) = 0;
	virtual void visit_stmt(For*) = 0;
	virtual void visit_stmt(If*) = 0;
	virtual void visit_stmt(Print*) = 0;
	virtual void visit_stmt(Sleep*) = 0;
	virtual void visit_stmt(Var*) = 0;
	virtual void visit_stmt(While*) = 0;
};

inline void Block::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Break::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Continue::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Expression::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void For::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void If::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Print::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Sleep::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void Var::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

inline void While::accept(StmtVisitor* visitor) {
	visitor->visit_stmt(this);
}

