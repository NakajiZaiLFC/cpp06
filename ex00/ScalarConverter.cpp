#include "ScalarConverter.hpp"
#include <iostream>
#include <sstream>
#include <limits>
#include <iomanip>
#include <cstdlib>

static std::string toDecimalString(double v, const std::string& suffix)
{
	std::ostringstream oss;
	oss << v;
	std::string s = oss.str();
	if (s == "inf")
		s = "+inf";
	else if (s.find('.') == std::string::npos && s.find('e') == std::string::npos
		&& s.find('n') == std::string::npos && s.find('i') == std::string::npos)
		s += ".0";
	s += suffix;
	return s;
}

static std::string intToString(int n)
{
	std::ostringstream oss;
	oss << n;
	return oss.str();
}

static std::string makeCharStr(double v)
{
	std::ostringstream oss;
	if (v < 0 || std::numeric_limits<char>::max() < v)
		return "impossible";
	if (!std::isprint(static_cast<unsigned char>(v)))
		return "Non displayable";
	oss << "'" << static_cast<char>(v) << "'";
	return oss.str();
}

static std::string makeIntStr(double v)
{
	if (v != v || v > std::numeric_limits<int>::max() || v < std::numeric_limits<int>::min())
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
	if (str.empty()) return false;
	size_t i = 0;
	if (str[i] == '-' || str[i] == '+') i++;
	if (i == str.length()) return false;
	for (; i < str.length(); i++) {
		if (!std::isdigit(str[i])) return false;
	}
	return true;
}

// [sign] digits [. digits] [e/E [sign] digits] f
bool ScalarConverter::M_isFloat(const std::string& str)
{
	if (str.empty()) return false;
	size_t i = 0;
	bool hasDigit = false;
	bool hasDot = false;
	bool hasExp = false;

	if (str[i] == '-' || str[i] == '+') i++;
	while (i < str.length() && std::isdigit(str[i])) { hasDigit = true; i++; }
	if (i < str.length() && str[i] == '.') { hasDot = true; i++; }
	while (i < str.length() && std::isdigit(str[i])) { hasDigit = true; i++; }
	if (i < str.length() && (str[i] == 'e' || str[i] == 'E')) {
		hasExp = true;
		i++;
		if (i < str.length() && (str[i] == '+' || str[i] == '-')) i++;
		bool hasExpDigit = false;
		while (i < str.length() && std::isdigit(str[i])) { hasExpDigit = true; i++; }
		if (!hasExpDigit) return false;
	}
	if (i >= str.length() || str[i] != 'f') return false;
	i++;
	return (i == str.length() && hasDigit && (hasDot || hasExp));
}

// [sign] digits [. digits] [e/E [sign] digits]
bool ScalarConverter::M_isDouble(const std::string& str)
{
	if (str.empty()) return false;
	size_t i = 0;
	bool hasDigit = false;
	bool hasDot = false;
	bool hasExp = false;

	if (str[i] == '-' || str[i] == '+') i++;
	while (i < str.length() && std::isdigit(str[i])) { hasDigit = true; i++; }
	if (i < str.length() && str[i] == '.') { hasDot = true; i++; }
	while (i < str.length() && std::isdigit(str[i])) { hasDigit = true; i++; }
	if (i < str.length() && (str[i] == 'e' || str[i] == 'E')) {
		hasExp = true;
		i++;
		if (i < str.length() && (str[i] == '+' || str[i] == '-')) i++;
		bool hasExpDigit = false;
		while (i < str.length() && std::isdigit(str[i])) { hasExpDigit = true; i++; }
		if (!hasExpDigit) return false;
	}
	return (i == str.length() && hasDigit && (hasDot || hasExp));
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
	double value = std::strtod(str.c_str(), NULL);
	return buildResult(value);
}

ConversionResult ScalarConverter::M_convertFloat(const std::string& str)
{
	double value = std::strtod(str.c_str(), NULL);
	return buildResult(value);
}

ConversionResult ScalarConverter::M_convertDouble(const std::string& str)
{
	double value = std::strtod(str.c_str(), NULL);
	return buildResult(value);
}

ConversionResult ScalarConverter::M_convertSpecialLiteral(const std::string& str)
{
	ConversionResult r;

	if (str == "nan" || str == "nanf")
	{
		r.charStr = "impossible";
		r.intStr = "impossible";
		r.floatStr = "nanf";
		r.doubleStr = "nan";
	}
	else if (str == "+inf" || str == "+inff")
	{
		r.charStr = "impossible";
		r.intStr = "impossible";
		r.floatStr = "+inff";
		r.doubleStr = "+inf";
	}
	else if (str == "-inf" || str == "-inff")
	{
		r.charStr = "impossible";
		r.intStr = "impossible";
		r.floatStr = "-inff";
		r.doubleStr = "-inf";
	}
	return r;
}

void ScalarConverter::M_printConversionResult(const ConversionResult& r)
{
	std::cout << std::setw(8) <<  "char: " << r.charStr << std::endl;
	std::cout << std::setw(8) <<  "int: " << r.intStr << std::endl;
	std::cout << std::setw(8) << "float: " << r.floatStr << std::endl;
	std::cout << std::setw(8) << "double: " <<  r.doubleStr << std::endl;
}
