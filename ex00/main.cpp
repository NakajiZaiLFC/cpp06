#include "ScalarConverter.hpp"
#include <iostream>

static void test(const std::string& input)
{
	std::cout << "--- [" << input << "] ---" << std::endl;
	ScalarConverter::convert(input);
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	if (argc == 2) {
		ScalarConverter::convert(argv[1]);
		return 0;
	}

	// char型
	test("a");
	test("*");
	test(" ");

	// int型
	test("0");
	test("42");
	test("-42");

	// float型
	test("42.0f");
	test("3.14f");
	test("-4.2f");

	// double型
	test("42.0");
	test("3.14");
	test("-4.2");

	// 特殊文字
	test("nan");
	test("nanf");
	test("+inf");
	test("-inf");
	test("+inff");
	test("-inff");

	// int型閾値
	test("2147483647");
	test("2147483648");
	test("-2147483648");
	test("-2147483649");

	// ASCII 
	test("31");
	test("32");
	test("126");
	test("127");
	test("128");

	// 無効
	test("");
	test("hello");
	test("4.2.0");
	test(".f");
	test("42f");
	test("inf");

	// 特殊値
	test("-0.0");
	test("-0.0f");
	test("0.000000000000000000000000000001");
	test("42.");
	test(".");

	// 指数表現
	test("2.14748e+09f");
	test("2.14748e+09");
	test("1e10");
	test("1e10f");
	test("1.5e-3f");
	test("1e309");

	// float型閾値
	test("16777216");
	test("16777217");

	return 0;
}
