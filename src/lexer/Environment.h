#pragma once
#include <unordered_map>
#include <string>
#include "../Value.h"

class Environment {
public:
	Environment() = default;
	Environment(Environment* enclosing) : m_enclosing(enclosing) {}

	void define(std::string name, Value value) {
		m_values[name] = value;
	}

	Value get(Token name) {
		if (const auto it = m_values.find(name.lexeme()); it != m_values.end()) {
			return it->second;
		}
		if (m_enclosing != nullptr) {
			return m_enclosing->get(name);
		}
		throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
	}

	void assign(Token name, Value value) {
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
	std::unordered_map<std::string, Value> m_values{};
	Environment* m_enclosing{nullptr};
};