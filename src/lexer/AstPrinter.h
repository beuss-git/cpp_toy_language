#pragma once
#include "Expr.h"
#include <sstream>
#include <variant>


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

	std::string print(std::shared_ptr<Expr> expr) {
		return expr->accept(this);
	}

	std::string visit(Binary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->left(), expr->right());
	}
	std::string visit(Grouping* expr) override {
		return parenthesize("group", expr->expression());
	}
	std::string visit(Literal* expr) override {
		if (!expr->value().has_value()) {
			return "nil";
		}
		const auto& type = expr->value().type();
		if (type == typeid(std::string)) {
			return std::any_cast<std::string>(expr->value());
		}
		if (type == typeid(bool)) {
			return std::to_string(std::any_cast<bool>(expr->value()));
		}
		if (type == typeid(double)) {
			return std::to_string(std::any_cast<double>(expr->value()));
		} 

		return "unsupported type";
	}
	std::string visit(Unary* expr) override {
		return parenthesize(expr->op().lexeme(), expr->right());
	}
};
