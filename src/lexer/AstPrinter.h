#pragma once
#include "Expr.h"
#include <sstream>
#include "Parser.h"


class AstPrinter final : public ExprVisitor {
public:
	template <typename ... Exprs>
	std::string parenthesize(std::string name, Exprs&& ... exprs) {
		std::stringstream ss;
		ss << "(" << name;

		([&](auto& expr) {
			ss << " " << expr->accept(this);
			}(exprs), ...);

		ss << ")";
		return ss.str();
	}

	Value print(ExprPtr expr) {
		return expr->accept(this);
	}

	Value visit_expr(Binary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->left(), expr->right());
	}
	Value visit_expr(Grouping* expr) override {
		return parenthesize("group", expr->expression());
	}
	Value visit_expr(Literal* expr) override {
		return expr->value();
	}
	Value visit_expr(Unary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->right());
	}
};
