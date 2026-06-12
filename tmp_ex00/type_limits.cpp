#include <iostream>
#include <climits>
#include <cfloat>
#include <iomanip>

int main()
{
	std::cout << "=== C++98 Fundamental Types ===" << std::endl;
	std::cout << std::endl;

	// --- sizeof ---
	std::cout << "--- sizeof (bytes) ---" << std::endl;
	std::cout << "bool          : " << sizeof(bool) << std::endl;
	std::cout << "char          : " << sizeof(char) << std::endl;
	std::cout << "signed char   : " << sizeof(signed char) << std::endl;
	std::cout << "unsigned char : " << sizeof(unsigned char) << std::endl;
	std::cout << "short         : " << sizeof(short) << std::endl;
	std::cout << "unsigned short: " << sizeof(unsigned short) << std::endl;
	std::cout << "int           : " << sizeof(int) << std::endl;
	std::cout << "unsigned int  : " << sizeof(unsigned int) << std::endl;
	std::cout << "long          : " << sizeof(long) << std::endl;
	std::cout << "unsigned long : " << sizeof(unsigned long) << std::endl;
	std::cout << "float         : " << sizeof(float) << std::endl;
	std::cout << "double        : " << sizeof(double) << std::endl;
	std::cout << "long double   : " << sizeof(long double) << std::endl;
	std::cout << std::endl;

	// --- size ordering guarantee ---
	std::cout << "--- Size ordering (standard guarantee) ---" << std::endl;
	std::cout << "1 == sizeof(char) <= sizeof(short) <= sizeof(int) <= sizeof(long)" << std::endl;
	std::cout << "sizeof(float) <= sizeof(double) <= sizeof(long double)" << std::endl;
	std::cout << "Actual: " << sizeof(char) << " <= " << sizeof(short)
			  << " <= " << sizeof(int) << " <= " << sizeof(long) << std::endl;
	std::cout << "Actual: " << sizeof(float) << " <= " << sizeof(double)
			  << " <= " << sizeof(long double) << std::endl;
	std::cout << std::endl;

	// --- <climits> ---
	std::cout << "--- <climits> macro constants ---" << std::endl;
	std::cout << "CHAR_BIT   : " << CHAR_BIT << "  (bits per byte)" << std::endl;
	std::cout << std::endl;

	std::cout << "[char]" << std::endl;
	std::cout << "  CHAR_MIN   : " << CHAR_MIN << std::endl;
	std::cout << "  CHAR_MAX   : " << CHAR_MAX << std::endl;
	std::cout << "  SCHAR_MIN  : " << SCHAR_MIN << std::endl;
	std::cout << "  SCHAR_MAX  : " << SCHAR_MAX << std::endl;
	std::cout << "  UCHAR_MAX  : " << UCHAR_MAX << std::endl;
	std::cout << std::endl;

	std::cout << "[short]" << std::endl;
	std::cout << "  SHRT_MIN   : " << SHRT_MIN << std::endl;
	std::cout << "  SHRT_MAX   : " << SHRT_MAX << std::endl;
	std::cout << "  USHRT_MAX  : " << USHRT_MAX << std::endl;
	std::cout << std::endl;

	std::cout << "[int]" << std::endl;
	std::cout << "  INT_MIN    : " << INT_MIN << std::endl;
	std::cout << "  INT_MAX    : " << INT_MAX << std::endl;
	std::cout << "  UINT_MAX   : " << UINT_MAX << std::endl;
	std::cout << std::endl;

	std::cout << "[long]" << std::endl;
	std::cout << "  LONG_MIN   : " << LONG_MIN << std::endl;
	std::cout << "  LONG_MAX   : " << LONG_MAX << std::endl;
	std::cout << "  ULONG_MAX  : " << ULONG_MAX << std::endl;
	std::cout << std::endl;

	// --- <cfloat> ---
	std::cout << "--- <cfloat> macro constants ---" << std::endl;
	std::cout << "FLT_RADIX    : " << FLT_RADIX << "  (base of float)" << std::endl;
	std::cout << std::endl;

	std::cout << std::setprecision(20);

	std::cout << "[float]  (standard: >= 6 decimal digits)" << std::endl;
	std::cout << "  FLT_DIG      : " << FLT_DIG << "  (decimal digits of precision)" << std::endl;
	std::cout << "  FLT_MANT_DIG : " << FLT_MANT_DIG << "  (binary digits in mantissa)" << std::endl;
	std::cout << "  FLT_MIN      : " << FLT_MIN << "  (smallest positive normalized)" << std::endl;
	std::cout << "  FLT_MAX      : " << FLT_MAX << "  (largest finite)" << std::endl;
	std::cout << "  FLT_EPSILON  : " << FLT_EPSILON << "  (1.0 + eps != 1.0)" << std::endl;
	std::cout << std::endl;

	std::cout << "[double]  (standard: >= 10 decimal digits)" << std::endl;
	std::cout << "  DBL_DIG      : " << DBL_DIG << "  (decimal digits of precision)" << std::endl;
	std::cout << "  DBL_MANT_DIG : " << DBL_MANT_DIG << "  (binary digits in mantissa)" << std::endl;
	std::cout << "  DBL_MIN      : " << DBL_MIN << "  (smallest positive normalized)" << std::endl;
	std::cout << "  DBL_MAX      : " << DBL_MAX << "  (largest finite)" << std::endl;
	std::cout << "  DBL_EPSILON  : " << DBL_EPSILON << "  (1.0 + eps != 1.0)" << std::endl;
	std::cout << std::endl;

	std::cout << "[long double]  (standard: >= 10 decimal digits)" << std::endl;
	std::cout << "  LDBL_DIG      : " << LDBL_DIG << "  (decimal digits of precision)" << std::endl;
	std::cout << "  LDBL_MANT_DIG : " << LDBL_MANT_DIG << "  (binary digits in mantissa)" << std::endl;
	std::cout << "  LDBL_MIN      : " << LDBL_MIN << "  (smallest positive normalized)" << std::endl;
	std::cout << "  LDBL_MAX      : " << LDBL_MAX << "  (largest finite)" << std::endl;
	std::cout << "  LDBL_EPSILON  : " << LDBL_EPSILON << "  (1.0 + eps != 1.0)" << std::endl;
	std::cout << std::endl;

	// --- practical note for ex00 ---
	std::cout << "--- ex00 relevance ---" << std::endl;
	std::cout << "char printable range : 32 (' ') to 126 ('~')" << std::endl;
	std::cout << "char valid range     : 0 to 127 (ASCII)" << std::endl;
	std::cout << "int overflow check   : v > " << INT_MAX << " || v < " << INT_MIN << std::endl;

	return 0;
}
