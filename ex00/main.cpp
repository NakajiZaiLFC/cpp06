#include "ScalarConverter.hpp"
#include <iostream>

static void test(const std::string& label, const std::string& input)
{
	std::cout << "--- " << label << " [" << input << "] ---" << std::endl;
	ScalarConverter::convert(input);
	std::cout << std::endl;
}

int main(int argc, char **argv)
{
	if (argc == 2) {
		ScalarConverter::convert(argv[1]);
		return 0;
	}

	// TODO(human): add comprehensive test cases below
	// --- Category 1: char ---
	test("printable char", "a");
	test("special char", "*");

	// --- Category 2: int ---
	test("zero", "0");
	test("positive int", "42");
	test("negative int", "-42");

	// --- Category 3: float ---
	test("simple float", "42.0f");
	test("pi float", "3.14f");
	test("negative float", "-4.2f");

	// --- Category 4: double ---
	test("simple double", "42.0");
	test("pi double", "3.14");
	test("negative double", "-4.2");

	// --- Category 5: special literals ---
	test("nan", "nan");
	test("nanf", "nanf");
	test("+inf", "+inf");
	test("-inf", "-inf");
	test("+inff", "+inff");
	test("-inff", "-inff");

	// --- Category 6: edge cases ---
	test("int max", "2147483647");
	test("int overflow", "2147483648");
	test("int min", "-2147483648");
	test("int underflow", "-2147483649");

	// --- Category 7: boundary ---
	test("ascii 0 (NUL)", "0");
	test("ascii 31 (ctrl)", "31");
	test("ascii 32 (space)", "32");
	test("ascii 126 (~)", "126");
	test("ascii 127 (DEL)", "127");
	test("ascii 128 (over)", "128");

	// --- Category 8: invalid ---
	test("empty", "");
	test("word", "hello");
	test("double dot", "4.2.0");
	test("sign only +", "+");
	test("sign only -", "-");
	test("trailing f no dot", "42f");

	// TODO(human): add more edge case tests you think are missing
	test("inf", "inf");
	test("limit of float", "0.000000000000000000000000000001");
	test("limit of float", "0.00000000000000.0000000000000001");
	test("only dot", ".");
	test("only dot", ".f");
	test("only dot", "42.");
	test("only dot", "4..2");

	return 0;
}
