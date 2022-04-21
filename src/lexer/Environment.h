#pragma once
#include <unordered_map>
#include <string>
#include "../Value.h"

class Environment {
public:
	Environment() = default;
	Environment(const std::shared_ptr<Environment>& enclosing) : m_enclosing(enclosing) {}

	void define(std::string name, Value value) {
		m_values[name] = value;
	}

	Value get(Token name) {
		if (const auto it = m_values.find(name.lexeme()); it != m_values.end()) {
			return it->second;
		}
		throw RuntimeError(name, "Undefined variable '" + name.lexeme() + "'.");
	}

	void assign(Token name, Value value) {
		if (auto it = m_values.find(name.lexeme()); it != m_values.end()) {
			it->second = value;
			return;
		}
		throw RuntimeError(name, "Undefined Variable '" + name.lexeme() + "'.");
	}
private:
	std::unordered_map<std::string, Value> m_values{};
	std::shared_ptr<Environment> m_enclosing{nullptr};
};