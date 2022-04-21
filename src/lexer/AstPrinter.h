#pragma once
#include "Expr.h"
#include <sstream>
#include "Parser.h"


class AstPrinter final : public ExprVisitor {
public:
	template <typename ... Exprs>
	ValuePtr parenthesize(std::string name, Exprs&& ... exprs) {
		std::stringstream ss;
		ss << "(" << name;

		([&](auto& expr) {
			ss << " " << expr->accept(this);
			}(exprs), ...);

		ss << ")";
		return create_value(ss.str());
	}

	ValuePtr print(ExprPtr expr) {
		return expr->accept(this);
	}

	ValuePtr visit_expr(Binary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->left(), expr->right());
	}
	ValuePtr visit_expr(Grouping* expr) override {
		return parenthesize("group", expr->expression());
	}
	ValuePtr visit_expr(Literal* expr) override {
		return expr->value();
	}
	ValuePtr visit_expr(Unary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->right());
	}
};
