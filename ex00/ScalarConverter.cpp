#include "ScalarConverter.hpp"
#include <iostream>
#include <sstream>

enum LiteralType {
	T_CHAR,
	T_INT,
	T_FLOAT,
	T_DOUBLE,
	T_SPECIAL_LITERAL,
	T_INVALID
};

struct ConversionResult{
	std::string charStr;
	std::string intStr;
	std::string floatStr;
	std::string doubleStr;
};

void ScalarConverter::convert(const std::string& str) 
{
	ConversionResult r;
	LiteralType type = M_detectType(str);

	if (type == T_INVALID) {
		std::cerr << "Error: Invalid literal" << std::endl;
		return;
	}
	r = M_convertToFormats(str, type);
	M_printConversionResult(r);
}

LiteralType ScalarConverter::M_detectType(const std::string& str)
{
	if (M_isSpecialLiteral(str)) return T_SPECIAL_LITERAL;
	if (M_isChar(str)) return T_CHAR;
	if (M_isInt(str)) return T_INT;
	if (M_isFloat(str)) return T_FLOAT;
	if (M_isDouble(str)) return T_DOUBLE;
	return T_INVALID;
}

bool ScalarConverter::M_isSpecialLiteral(const std::string& str)
{
	return (str == "nan" || str == "nanf" || str == "+inf" || str == "-inf" || str == "+inff" || str == "-inff");
}

bool ScalarConverter::M_isChar(const std::string& str)
{
	return (str.length() == 1 && !std::isdigit(str[0]));
}

bool ScalarConverter::M_isInt(const std::string& str)
{
	size_t i = 0;
	if (str[i] == '-' || str[i] == '+') i++;
	for (; i < str.length(); i++) {
		if (!std::isdigit(str[i])) return false;
	}
	return true;
}

bool ScalarConverter::M_isFloat(const std::string& str)
{
	size_t i = 0;
	if (str[i] == '-' || str[i] == '+') i++;
	bool hasDecimalPoint = false;
	for (; i < str.length(); i++) {
		if (str[i] == '.') {
			if (hasDecimalPoint) return false;
			hasDecimalPoint = true;
		} else if (!std::isdigit(str[i]) && str[i] != 'f') {
			return false;
		}
	}
	return (str.size() - 1 == 'f' && hasDecimalPoint);
}

bool ScalarConverter::M_isDouble(const std::string& str)
{
	size_t i = 0;
	if (str[i] == '-' || str[i] == '+') i++;
	bool hasDecimalPoint = false;
	for (; i < str.length(); i++) {
		if (str[i] == '.') {
			if (hasDecimalPoint) return false;
			hasDecimalPoint = true;
		} else if (!std::isdigit(str[i])) {
			return false;
		}
	}
	return hasDecimalPoint;
}

ConversionResult ScalarConverter::M_convertToFormats(const std::string& str, LiteralType type)
{
	ConversionResult r;
	switch (type){
		case T_CHAR:
			r = M_convertChar(str);
			break;
		case T_INT:
			r = M_convertInt(str);
			break;
		case T_FLOAT:
			r = M_convertFloat(str);
			break;
		case T_DOUBLE:
			r = M_convertDouble(str);
			break;
		case T_SPECIAL_LITERAL:
			r = M_convertSpecialLiteral(str);
			break;
	}
	return r;
}

ConversionResult ScalarConverter::M_convertChar(const std::string& str)
{
	ConversionResult r;
	std::ostringstream os;
	char c = str[0];
	os << c;
	r.charStr = os.str();
	os.str("");
	os << static_cast<int>(c);
	r.intStr = os.str();
	os.str("");
	os << static_cast<float>(c) << "f";
	r.floatStr = os.str();
	os.str("");
	os << static_cast<double>(c);
	r.doubleStr = os.str();
	return r;	
}

void ScalarConverter::M_printConversionResult(const ConversionResult& r)
{
	std::cout << "char: " << r.charStr << std::endl;
	std::cout << "int: " << r.intStr << std::endl;
	std::cout << "float: " << r.floatStr << std::endl;
	std::cout << "double: " << r.doubleStr << std::endl;
}


