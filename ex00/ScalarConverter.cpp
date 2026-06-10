#include "ScalarConverter.hpp"
#include <iostream>
#include <sstream>
#include <climits>
#include <iomanip>

static std::string toDecimalString(double v, const std::string& suffix)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(1) << v << suffix;
	return oss.str();
}

static std::string intToString(int n)
{
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

static std::string makeCharStr(double v)
{
	// TODO(human)
	if (!std::isprint(static_cast<unsigned char>(v)))
		return "Non displayable"
	else if (v < -128 || 127 < v)
		return ""
	return "";
}

static std::string makeIntStr(double v)
{
	if (v != v || v > INT_MAX || v < INT_MIN)
		return "impossible";
	return intToString(static_cast<int>(v));
}

static std::string makeFloatStr(double v)
{
	return toDecimalString(static_cast<float>(v), "f");
}

static std::string makeDoubleStr(double v)
{
	return toDecimalString(v, "");
}

static ConversionResult buildResult(double v)
{
	ConversionResult r;
	r.charStr = makeCharStr(v);
	r.intStr = makeIntStr(v);
	r.floatStr = makeFloatStr(v);
	r.doubleStr = makeDoubleStr(v);
	return r;
}


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
		case T_INVALID:
			break;
	}
	return r;
}

ConversionResult ScalarConverter::M_convertChar(const std::string& str)
{
	return buildResult(static_cast<double>(str[0]));
}

ConversionResult ScalarConverter::M_convertInt(const std::string& str)
{
	std::stringstream ss(str);
	double value = 0;
	ss >> value;
	return buildResult(value);
}

ConversionResult ScalarConverter::M_convertFloat(const std::string& str)
{
	std::stringstream ss(str);
	float value = 0;
	ss >> value;
	return buildResult(static_cast<double>(value));
}


void ScalarConverter::M_printConversionResult(const ConversionResult& r)
{
	std::cout << "char: " << r.charStr << std::endl;
	std::cout << "int: " << r.intStr << std::endl;
	std::cout << "float: " << r.floatStr << std::endl;
	std::cout << "double: " << r.doubleStr << std::endl;
}
