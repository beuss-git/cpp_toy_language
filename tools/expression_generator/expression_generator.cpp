#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

// trim from start (in place)
void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

std::vector<std::string> split(std::string str, std::string sep) {
	char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	int split_count = 0;
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;
}
std::string str_lower(std::string data) {
	std::transform(data.begin(), data.end(), data.begin(), [](unsigned char c) {return std::tolower(c); });
	return data;
}
void define_type(std::fstream& f, std::string base_name, std::string return_type, std::string class_name, std::string field_list) {
	// class [class_name] final : [base_name] {
	f << "class " << class_name << " final : public " << base_name << " {\n";
	f << "public:\n";


	const auto fields = split(field_list, ";");
	// constructor:
	// [class_name]([field_list])
	f << "\t" << class_name << "(";
	for (size_t i = 0; i < fields.size(); i++) {
		const auto& field = fields.at(i);
		f << field << (i == fields.size() - 1 ? "" : ",");
	}
	if (!fields.empty()) {
		f << ")\n";
		// Indent
		f << "\t\t : ";

		// Initialize fields
		for (size_t i = 0; i < fields.size(); i++) {
			const auto& field = fields.at(i);
			std::string field_name = split(field, " ").at(1);
			f << "m_" << field_name << "(" << field_name << ")" << (i == fields.size() - 1 ? "" : ", ");
		}
		f << " { }\n";
	}
	else {
		f << ") {} \n";
	}


	// Accept override
	f << "\t"<< return_type << " accept(" << base_name << "Visitor* visitor) override;\n\n";

	// Field getters
	for (const auto& field : fields) {
		const auto field_components = split(field, " ");
		const auto field_type = field_components.at(0);
		const auto field_name = field_components.at(1);
		
		f << "\t" << field_type << " " << field_name << "() const {\n";
		f << "\t\treturn m_" << field_name << ";\n";
		f << "\t}\n";
	}

	// Field declarations
	f << "private:\n";
	for (const auto& field : fields) {
		const auto field_components = split(field, " ");
		const auto field_type = field_components.at(0);
		const auto field_name = field_components.at(1);
		
		f << "\t" << field_type << " m_" << field_name << "{};\n";
	}

	f << "};\n\n";
}
void define_start(std::fstream& f, std::string base_name) {
	// #pragma once
	f << "#pragma once\n";
	f << "#include <cassert>\n";
	//f << "#include <variant>\n";
	f << "#include <memory>\n";
	f << "#include <string>\n\n";
	f << "#include \"../Value.h\"\n\n";
	// #include "token.h"
	f << "#include \"Token.h\"\n\n";
	//f << "namespace " << base_name << " {\n\n";
	f << "class " << base_name << "Visitor;\n";
}
void define_end(std::fstream& f) {
	//f << "}\n";
}
void define_base_class(std::fstream& f, std::string base_name, std::string return_type) {

	// class [base_name] {
	f << "class " << base_name << " {\n";
	f << "public:\n";
	f << "\tvirtual " << return_type << " accept(" << base_name << "Visitor * visitor) {\n";
	f << "\t\tassert(false, \"Not implemented\");\n";
	if (return_type != "void") {
		f << "\t\treturn {};\n";
	}
	f << "\t}\n";

	// }
	f << "};\n\n";

	// Define Ptr
	f << "using " << base_name << "Ptr = std::shared_ptr<" << base_name << ">;\n\n";
}

void define_base_visitor(std::fstream& f, std::string base_name, std::string return_type, std::vector<std::string> types) {
	f << "class " << base_name << "Visitor {\n";
	f << "public:\n";

	for (std::string type : types) {
		std::string class_name = split(type, "|")[0];
		trim(class_name);
		std::string fields = split(type, "|")[1];
		trim(fields);

		f << "\tvirtual " << return_type << " visit_" << str_lower(base_name) << "(" << class_name << "*) = 0;\n";
	}
	f << "};\n\n";
}

void define_visitors_accept(std::fstream& f, std::string base_name, std::string return_type, std::vector<std::string> types) {

	for (std::string type : types) {
		std::string class_name = split(type, "|")[0];
		trim(class_name);
		std::string fields = split(type, "|")[1];
		trim(fields);

		f << "inline " << return_type << " " << class_name << "::accept(" << base_name << "Visitor* visitor) {\n";
		f << "\t" << (return_type == "void" ? "" : "return ") << "visitor->visit_" << str_lower(base_name) << "(this);\n";
		f << "}\n\n";
	}
}

void define_ast(const std::string& output_dir, std::string base_name, std::string return_type, const std::vector<std::string>& types) {
	std::string path = output_dir + "/" + base_name + ".h";
	std::fstream f(path, std::fstream::out);
	if (!f.is_open()) {
		char error_message[256];
		strerror_s(error_message, errno);
		std::cerr << "Failed to open file! " << error_message << "\n";
		return;
	}

	define_start(f, base_name);

	define_base_class(f, base_name, return_type);

	for (std::string type : types) {
		std::string class_name = split(type, "|")[0];
		trim(class_name);
		std::string fields = split(type, "|")[1];
		trim(fields);
		define_type(f, base_name, return_type, class_name, fields);
	}

	define_base_visitor(f, base_name, return_type, types);
	define_visitors_accept(f, base_name, return_type, types);

	define_end(f);
}

int main() {

	static auto output_dir = R"(G:\repos\cpp_toy_language\src\lexer)";

	define_ast(output_dir, "Expr", "ValuePtr", std::vector<std::string>{
		"Assign   | Token name; ExprPtr value",
		"Binary   | ExprPtr left; Token op; ExprPtr right",
		"Call     | ExprPtr callee; Token paren; std::vector<ExprPtr> arguments",
		"Grouping | ExprPtr expression",
		"Literal  | ValuePtr value",
		"Logical  | ExprPtr left; Token op; ExprPtr right",
		"Unary    | Token op; ExprPtr right",
		"Variable | Token name"
	});
	define_ast(output_dir, "Stmt", "void", std::vector<std::string>{
		"Block		| std::vector<StmtPtr> statements",
		"Break		| ",
		"Continue	| ",
		"Expression	| ExprPtr expression",
		"Function   | Token name; std::vector<Token> params; std::vector<StmtPtr> body",
		"For		| StmtPtr initializer; ExprPtr condition; ExprPtr increment; StmtPtr body",
		"If			| ExprPtr condition; StmtPtr then_branch; StmtPtr else_branch",
		"Print		| ExprPtr expression",
		"Return     | Token keyword; ExprPtr value",
		"Sleep		| Token token; ExprPtr expression",
		"Var		| Token name; ExprPtr initializer",
		"While      | ExprPtr condition; StmtPtr body"
	});
	return 0;
}
