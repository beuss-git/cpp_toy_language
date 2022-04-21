#pragma once
#include <cassert>
#include <ostream>

class Value {
public:
	Value() { }

	Value(bool value) : m_type(Type::BOOL) {
		m_value.as_bool = value;
	}
	Value(double value) : m_type(Type::NUMBER) {
		m_value.as_double = value;
	}
	Value(float value) : m_type(Type::NUMBER) {
		m_value.as_double = static_cast<double>(value);
	}
	Value(std::string value) : m_str_value(std::move(value)), m_type(Type::STRING) { }
	Value(void*) : m_type { Type::NIL }{ }

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

	virtual std::string to_string() const {
		switch (m_type) {
			case Type::STRING: return "\"" + m_str_value + "\"";
			case Type::BOOL: return m_value.as_bool ? "true" : "false";
			case Type::NUMBER: return std::to_string(m_value.as_double);
			case Type::NIL: return "nil";
			default: return "unsupported type" ;
		}
	}

	double as_double() const {
		assert(is_number());
		return m_value.as_double;
	}
	bool as_bool() const {
		assert(is_bool());
		return m_value.as_bool;
	}
	std::string as_string() const {
		// TODO: Change this behavior? This could lead to hard to spot bugs
		if (!is_string()) {
			return stringify();
		}
		return m_str_value;
	}

	bool operator==(const Value& rhs) const {
		if (is_nil() && rhs.is_nil()) return true;
		if (is_nil()) return false;

		switch (m_type) {
			case Type::STRING: return as_string() == rhs.as_string();
			case Type::BOOL: return as_bool() == rhs.as_bool();
			case Type::NUMBER: return compare_double(as_double(), rhs.as_double());
			// throw?
			default: return false;
		}
	}
private:

	std::string stringify() const {
		switch (m_type) {
			case Type::STRING: return m_str_value;
			case Type::BOOL: return m_value.as_bool ? "true" : "false";
			case Type::NUMBER: return std::to_string(m_value.as_double);
			case Type::NIL: return "nil";
			default: return "unsupported type" ;
		}
	}
	bool compare_double(double a, double b) const {
		// TODO: fix this
		return abs(a - b) < std::numeric_limits<double>::min();
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

using ValuePtr = std::shared_ptr<Value>;


template<typename T, typename... Args>
inline ValuePtr create_value(Args&&... args) {
	return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename... Args>
inline ValuePtr create_value(Args&&... args) {
	return std::make_shared<Value>(std::forward<Args>(args)...);
}
