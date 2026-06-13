#include "ScalarConverter.hpp"
#include "ConversionHelper.hpp"
#include <iostream>
#include <iomanip>
#include <cstdlib>

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
	return (str == "nan" || \
			str == "nanf" || \
			str == "+inf" || \
			str == "-inf" || \
			str == "+inff" || \
			str == "-inff");
}

bool ScalarConverter::M_isChar(const std::string& str)
{
	return (str.length() == 1 && !std::isdigit(static_cast<unsigned char>(str[0])));
}

bool ScalarConverter::M_isInt(const std::string& str)
{
	size_t i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (i == str.length()) return false;
	for (; i < str.length(); i++) {
		if (!std::isdigit(static_cast<unsigned char>(str[i]))) return false;
	}
	return true;
}

bool ScalarConverter::M_isFloat(const std::string& str)
{
	size_t i = 0;
	bool hasDigit = false;
	bool hasDot = false;
	bool hasExp = false;

	if (str[i] == '-' || str[i] == '+') 
		i++;
	while (i < str.length() && std::isdigit(static_cast<unsigned char>(str[i])))
	{
		hasDigit = true;
		i++;
	}
	if (i < str.length() && str[i] == '.')
	{
		hasDot = true;
		i++;
	}
	while (i < str.length() && std::isdigit(static_cast<unsigned char>(str[i])))
	{
		hasDigit = true;
		i++;
	}
	if (i < str.length() && (str[i] == 'e' || str[i] == 'E'))
	{
		hasExp = true;
		i++;
		if (i < str.length() && (str[i] == '+' || str[i] == '-'))
			i++;
		bool hasExpDigit = false;
		while (i < str.length() && std::isdigit(static_cast<unsigned char>(str[i])))
		{
			hasExpDigit = true;
			i++;
		}
		if (!hasExpDigit) return false;
	}
	if (i >= str.length() || str[i] != 'f') return false;
	i++;
	return (i == str.length() && hasDigit && (hasDot || hasExp));
}

bool ScalarConverter::M_isDouble(const std::string& str)
{
	size_t i = 0;
	bool hasDigit = false;
	bool hasDot = false;
	bool hasExp = false;

	if (str[i] == '-' || str[i] == '+') i++;
	while (i < str.length() && std::isdigit(static_cast<unsigned char>(str[i])))
	{
		hasDigit = true;
		i++;
	}
	if (i < str.length() && str[i] == '.')
	{
		hasDot = true;
		i++;
	}
	while (i < str.length() && std::isdigit(static_cast<unsigned char>(str[i])))
	{
		hasDigit = true;
		i++;
	}
	if (i < str.length() && (str[i] == 'e' || str[i] == 'E')) 
	{
		hasExp = true;
		i++;
		if (i < str.length() && (str[i] == '+' || str[i] == '-')) i++;
		bool hasExpDigit = false;
		while (i < str.length() && std::isdigit(static_cast<unsigned char>(str[i]))) 
		{
			hasExpDigit = true;
			i++;
		}
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
