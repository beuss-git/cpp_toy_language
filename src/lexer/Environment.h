#pragma once
#include <unordered_map>
#include <string>
#include "../Value.h"

class Environment {
public:
	Environment() = default;
	Environment(std::shared_ptr<Environment> enclosing) : m_enclosing(enclosing) {}
	//Environment(Environment&) = delete;
	//Environment(const Environment&) = delete;


	void define(std::string name, ValuePtr value) {
		m_values[name] = value;
	}

	ValuePtr get(Token name) {
		if (const auto it = m_values.find(name.lexeme()); it != m_values.end()) {
			return it->second;
		}
		if (m_enclosing != nullptr) {
			return m_enclosing->get(name);
		}
		throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
	}

	void assign(Token name, ValuePtr value) {
		if (auto it = m_values.find(name.lexeme()); it != m_values.end()) {
			it->second = value;
			return;
		}

		if (m_enclosing != nullptr) {
			m_enclosing->assign(name, value);
			return;
		}
		throw RuntimeError(name, "Undefined Variable '" + name.lexeme() + "'.");
	}
private:
	std::unordered_map<std::string, ValuePtr> m_values{};
	std::shared_ptr<Environment> m_enclosing{nullptr};
};