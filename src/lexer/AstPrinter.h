#pragma once
#include "Expr.h"
#include <sstream>
#include "Parser.h"


class AstPrinter final : public Visitor {
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

	Value visit(Binary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->left(), expr->right());
	}
	Value visit(Grouping* expr) override {
		return parenthesize("group", expr->expression());
	}
	Value visit(Literal* expr) override {
		return expr->value().to_string();
	}
	Value visit(Unary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->right());
	}
};
