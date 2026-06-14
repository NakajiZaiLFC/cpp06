#pragma once
#include <string>

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

class ScalarConverter {
public:
	static void convert(const std::string& str);

private:
    ScalarConverter();
    ScalarConverter(const ScalarConverter& other);
    ScalarConverter& operator=(const ScalarConverter& other);
    ~ScalarConverter();

	static LiteralType M_detectType(const std::string& str);
	static bool M_isChar(const std::string& str);
	static bool M_isInt(const std::string& str);
	static bool M_isFloat(const std::string& str);
	static bool M_isDouble(const std::string& str);
	static bool M_isSpecialLiteral(const std::string& str);
	static void M_printConversionResult(const ConversionResult& r);
	static ConversionResult M_convertToFormats(const std::string& str, LiteralType type);
	static ConversionResult M_convertChar(const std::string& str);
	static ConversionResult M_convertInt(const std::string& str);
	static ConversionResult M_convertFloat(const std::string& str);
	static ConversionResult M_convertDouble(const std::string& str);
	static ConversionResult M_convertSpecialLiteral(const std::string& str);

	static std::string M_toDecimalString(double v, const std::string& suffix);
	static std::string M_makeCharStr(double v);
	static std::string M_makeIntStr(double v);
	static std::string M_makeFloatStr(double v);
	static std::string M_makeDoubleStr(double v);
	static ConversionResult M_buildResult(double v);
};

