#include <iostream>
#include <iomanip>
#include <cmath>
#include <climits>
#include <cfloat>

static void test_double(const char* label, double v)
{
	std::cout << "  " << std::setw(16) << label << " : ";
	std::cout << "value=" << std::setw(22) << v;
	std::cout << "  v==v:" << (v == v ? "Y" : "N");
	std::cout << "  v>0:" << (v > 0 ? "Y" : "N");
	std::cout << "  v<0:" << (v < 0 ? "Y" : "N");
	std::cout << "  v==0:" << (v == 0.0 ? "Y" : "N");
	std::cout << std::endl;
}

int main()
{
	std::cout << std::setprecision(17);

	// =============================================
	// 1. IEEE 754 special values
	// =============================================
	std::cout << "=== 1. IEEE 754 Special Values ===" << std::endl;

	double pos_inf = 1.0 / 0.0;
	double neg_inf = -1.0 / 0.0;
	double nan_val = 0.0 / 0.0;
	double neg_zero = -0.0;
	double pos_zero = 0.0;

	test_double("+Inf", pos_inf);
	test_double("-Inf", neg_inf);
	test_double("NaN", nan_val);
	test_double("+0.0", pos_zero);
	test_double("-0.0", neg_zero);
	std::cout << std::endl;

	// =============================================
	// 2. -0.0 vs +0.0
	// =============================================
	std::cout << "=== 2. Negative Zero ===" << std::endl;
	std::cout << "  -0.0 == +0.0      : " << (neg_zero == pos_zero ? "true" : "false") << std::endl;
	std::cout << "  -0.0 < 0          : " << (neg_zero < 0 ? "true" : "false") << std::endl;
	std::cout << "  1.0 / +0.0        : " << (1.0 / pos_zero) << std::endl;
	std::cout << "  1.0 / -0.0        : " << (1.0 / neg_zero) << std::endl;
	std::cout << "  static_cast<int>  : " << static_cast<int>(neg_zero) << std::endl;
	std::cout << "  static_cast<char> : " << static_cast<int>(static_cast<char>(neg_zero)) << std::endl;
	std::cout << std::endl;

	// =============================================
	// 3. NaN properties
	// =============================================
	std::cout << "=== 3. NaN: All Comparisons Fail ===" << std::endl;
	std::cout << "  NaN == NaN   : " << (nan_val == nan_val ? "T" : "F") << std::endl;
	std::cout << "  NaN != NaN   : " << (nan_val != nan_val ? "T" : "F") << std::endl;
	std::cout << "  NaN >  0     : " << (nan_val > 0 ? "T" : "F") << std::endl;
	std::cout << "  NaN <  0     : " << (nan_val < 0 ? "T" : "F") << std::endl;
	std::cout << "  NaN >= 0     : " << (nan_val >= 0 ? "T" : "F") << std::endl;
	std::cout << "  NaN <= 0     : " << (nan_val <= 0 ? "T" : "F") << std::endl;
	std::cout << "  NaN >  NaN   : " << (nan_val > nan_val ? "T" : "F") << std::endl;
	std::cout << "  NaN + 1      : " << (nan_val + 1) << std::endl;
	std::cout << "  NaN * 0      : " << (nan_val * 0) << std::endl;
	std::cout << std::endl;

	// =============================================
	// 4. Inf arithmetic
	// =============================================
	std::cout << "=== 4. Inf Arithmetic ===" << std::endl;
	std::cout << "  Inf + 1      : " << (pos_inf + 1) << std::endl;
	std::cout << "  Inf + Inf    : " << (pos_inf + pos_inf) << std::endl;
	std::cout << "  Inf - Inf    : " << (pos_inf - pos_inf) << "  (becomes NaN!)" << std::endl;
	std::cout << "  Inf * 0      : " << (pos_inf * 0) << "  (becomes NaN!)" << std::endl;
	std::cout << "  Inf * -1     : " << (pos_inf * -1) << std::endl;
	std::cout << "  1 / Inf      : " << (1.0 / pos_inf) << std::endl;
	std::cout << std::endl;

	// =============================================
	// 5. How special values are generated
	// =============================================
	std::cout << "=== 5. How Special Values Arise ===" << std::endl;
	std::cout << "  0.0 / 0.0          : " << (0.0 / 0.0) << "  (NaN)" << std::endl;
	std::cout << "  sqrt(-1.0)         : " << std::sqrt(-1.0) << "  (NaN)" << std::endl;
	std::cout << "  log(-1.0)          : " << std::log(-1.0) << "  (NaN)" << std::endl;
	std::cout << "  1.0 / 0.0          : " << (1.0 / 0.0) << "  (+Inf)" << std::endl;
	std::cout << "  -1.0 / 0.0         : " << (-1.0 / 0.0) << "  (-Inf)" << std::endl;
	std::cout << "  DBL_MAX * 2        : " << (DBL_MAX * 2) << "  (+Inf by overflow)" << std::endl;
	std::cout << "  -DBL_MAX * 2       : " << (-DBL_MAX * 2) << "  (-Inf by overflow)" << std::endl;
	std::cout << std::endl;

	// =============================================
	// 6. Denormalized (subnormal) numbers
	// =============================================
	std::cout << "=== 6. Denormalized Numbers ===" << std::endl;
	std::cout << "  DBL_MIN            : " << DBL_MIN << "  (smallest normalized)" << std::endl;
	std::cout << "  DBL_MIN / 2        : " << (DBL_MIN / 2) << "  (denormalized)" << std::endl;
	std::cout << "  DBL_MIN / 1e300    : " << (DBL_MIN / 1e300) << "  (underflow to 0)" << std::endl;
	std::cout << "  denorm == 0?       : " << ((DBL_MIN / 2) == 0 ? "true" : "false") << std::endl;
	std::cout << "  denorm > 0?        : " << ((DBL_MIN / 2) > 0 ? "true" : "false") << std::endl;
	std::cout << "  cast to int        : " << static_cast<int>(DBL_MIN / 2) << std::endl;
	std::cout << std::endl;

	// =============================================
	// 7. ex00 guard conditions summary
	// =============================================
	std::cout << "=== 7. Guard Matrix for ex00 ===" << std::endl;
	std::cout << std::endl;

	double cases[] = {42.0, -1.0, 0.0, neg_zero, pos_inf, neg_inf, nan_val,
					  128.0, 2147483648.0, -2147483649.0, DBL_MIN / 2};
	const char* names[] = {"42.0", "-1.0", "+0.0", "-0.0", "+Inf", "-Inf", "NaN",
						   "128.0", "INT_MAX+1", "INT_MIN-1", "denorm"};

	std::cout << "  " << std::setw(12) << "value"
			  << std::setw(14) << "char"
			  << std::setw(14) << "int" << std::endl;
	std::cout << "  " << std::string(40, '-') << std::endl;

	for (int i = 0; i < 11; i++) {
		double v = cases[i];
		std::cout << "  " << std::setw(12) << names[i];

		// char guard
		if (v != v || v < 0 || v > 127)
			std::cout << std::setw(14) << "impossible";
		else if (!std::isprint(static_cast<unsigned char>(v)))
			std::cout << std::setw(14) << "Non display";
		else {
			std::string s = std::string("'") + static_cast<char>(static_cast<int>(v)) + "'";
			std::cout << std::setw(14) << s;
		}

		// int guard
		if (v != v || v > INT_MAX || v < INT_MIN)
			std::cout << std::setw(14) << "impossible";
		else
			std::cout << std::setw(14) << static_cast<int>(v);

		std::cout << std::endl;
	}

	return 0;
}
