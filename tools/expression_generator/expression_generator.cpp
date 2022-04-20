#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

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
void define_type(std::fstream& f, std::string base_name, std::string class_name, std::string field_list) {
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

	// Accept override
	f << "\tstd::string accept(Visitor* visitor) override;\n\n";

	// Field getters
	for (const auto& field : fields) {
		const auto field_components = split(field, " ");
		const auto field_type = field_components.at(0);
		const auto field_name = field_components.at(1);
		
		f << "\t" << field_type << " " << field_name << "() const {\n";
		f << "\t\treturn m_" << field_name << ";\n";
		f << "}\n";
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
void define_base_class(std::fstream& f, std::string base_name) {
	// #pragma once
	f << "#pragma once\n";
	f << "#include <cassert>\n";
	//f << "#include <variant>\n";
	f << "#include <memory>\n";
	f << "#include <string>\n\n";
	// #include "token.h"
	f << "#include \"Token.h\"\n\n";
	f << "class Visitor;\n";

	// class [base_name] {
	f << "class " << base_name << " {\n";
	f << "public:\n";
	f << 
R"(	virtual std::string accept(Visitor* visitor) {
		assert(false, "Not implemented");
		return {};
	}
)";

	// }
	f << "};\n\n";

	// Define ExprPtr
	f << "using ExprPtr = std::shared_ptr<Expr>;\n\n";
}

void define_base_visitor(std::fstream& f, std::string base_name, std::vector<std::string> types) {
	f << "class Visitor {\n";
	f << "public:\n";

	for (std::string type : types) {
		std::string class_name = split(type, "|")[0];
		trim(class_name);
		std::string fields = split(type, "|")[1];
		trim(fields);

		f << "\tvirtual std::string visit(" << class_name << "*) = 0;\n";
	}
	f << "};\n\n";
}

void define_visitors_accept(std::fstream& f, std::string base_name, std::vector<std::string> types) {

	for (std::string type : types) {
		std::string class_name = split(type, "|")[0];
		trim(class_name);
		std::string fields = split(type, "|")[1];
		trim(fields);

		f << "inline std::string " << class_name << "::accept(Visitor* visitor) {\n";
		f << "\treturn visitor->visit(this);\n";
		f << "}\n\n";
	}
}

void define_ast(const std::string& output_dir, std::string base_name, std::vector<std::string> types) {
	std::string path = output_dir + "/" + base_name + ".h";
	std::fstream f(path, std::fstream::out);
	if (!f.is_open()) {
		char error_message[256];
		strerror_s(error_message, errno);
		std::cerr << "Failed to open file! " << error_message << "\n";
		return;
	}

	define_base_class(f, base_name);

	for (std::string type : types) {
		std::string class_name = split(type, "|")[0];
		trim(class_name);
		std::string fields = split(type, "|")[1];
		trim(fields);
		define_type(f, base_name, class_name, fields);
	}

	define_base_visitor(f, base_name, types);
	define_visitors_accept(f, base_name, types);
}

int main() {

	static auto output_dir = R"(G:\repos\cpp_toy_language\src\lexer)";

	define_ast(output_dir, "Expr", std::vector<std::string>{
		"Binary   | ExprPtr left; Token op; ExprPtr right",
		"Grouping | ExprPtr expression",
		"Literal  | std::any value",
		"Unary    | Token op; ExprPtr right"
	});
	return 0;
}
