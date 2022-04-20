#pragma once
#include <ostream>

class Value {
public:
	Value() { }

	Value(bool value) {
		m_type = Type::BOOL;
		m_value.as_bool = value;
	}
	Value(double value) {
		m_type = Type::NUMBER;
		m_value.as_double = value;
	}
	Value(float value) {
		m_type = Type::NUMBER;
		m_value.as_double = static_cast<double>(value);
	}
	Value(std::string value) {
		m_type = Type::STRING;
		m_str_value = value.c_str();
	}
	Value(void*) {
		m_type = Type::NIL;
	}
	enum class Type {
		STRING,
		BOOL,
		NUMBER,
		NIL,
	};

	bool is_nil() const {
		return m_type == Type::NIL;
	}
	bool is_number() const {
		return m_type == Type::NUMBER;
	}
	bool is_string() const {
		return m_type == Type::STRING;
	}
	bool is_bool() const {
		return m_type == Type::BOOL;
	}

	std::string to_string() const {
		switch (m_type) {
			case Type::STRING: return "\"" + m_str_value + "\"";
			case Type::BOOL: return std::to_string(m_value.as_bool);
			case Type::NUMBER: return std::to_string(m_value.as_double);
			case Type::NIL: return "nil";
			default: return "unsupported type" ;
		}
	}
private:
	union {
		bool as_bool;
		double as_double;

		uint64_t encoded;
	} m_value {.encoded = 0};
	// TODO: don't use this
	std::string m_str_value{};
	Type m_type{Type::NIL};
};

inline std::ostream& operator<<(std::ostream& os, const Value& value) {
	return os << value.to_string();
}
