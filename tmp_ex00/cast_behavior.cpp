#include <iostream>
#include <climits>
#include <cfloat>
#include <iomanip>
#include <cmath>

int main()
{
	std::cout << std::setprecision(20);

	// =============================================
	// 1. double -> int: truncation (toward zero)
	// =============================================
	std::cout << "=== 1. double -> int (static_cast) ===" << std::endl;
	std::cout << "Rule: fractional part discarded, toward zero" << std::endl;
	std::cout << std::endl;

	double d1 = 3.99;
	double d2 = -3.99;
	double d3 = 0.9999999;
	std::cout << "  3.99   -> " << static_cast<int>(d1) << std::endl;
	std::cout << "  -3.99  -> " << static_cast<int>(d2) << std::endl;
	std::cout << "  0.9999 -> " << static_cast<int>(d3) << std::endl;
	std::cout << std::endl;

	// =============================================
	// 2. double -> int: overflow = UNDEFINED BEHAVIOR
	// =============================================
	std::cout << "=== 2. double -> int overflow (UB!) ===" << std::endl;
	std::cout << "INT_MAX     = " << INT_MAX << std::endl;
	std::cout << "INT_MAX + 1 as double = " << static_cast<double>(INT_MAX) + 1.0 << std::endl;

	double safe = static_cast<double>(INT_MAX);
	double unsafe = safe + 1.0;
	std::cout << "  (double)INT_MAX       -> int: " << static_cast<int>(safe) << std::endl;
	std::cout << "  (double)INT_MAX + 1.0 -> int: " << static_cast<int>(unsafe)
			  << "  <-- UB: result is garbage" << std::endl;
	std::cout << std::endl;

	// =============================================
	// 3. int -> double: precision loss
	// =============================================
	std::cout << "=== 3. int -> double precision ===" << std::endl;
	std::cout << "double has 53-bit mantissa = 2^53 = 9007199254740992" << std::endl;
	std::cout << "int (32-bit) max = 2^31-1 = 2147483647" << std::endl;
	std::cout << "-> All 32-bit ints fit EXACTLY in double" << std::endl;
	std::cout << std::endl;

	int n1 = INT_MAX;
	double dn1 = static_cast<double>(n1);
	int back1 = static_cast<int>(dn1);
	std::cout << "  INT_MAX         = " << n1 << std::endl;
	std::cout << "  (double)INT_MAX = " << dn1 << std::endl;
	std::cout << "  back to int     = " << back1 << std::endl;
	std::cout << "  roundtrip ok?   : " << (n1 == back1 ? "YES" : "NO") << std::endl;
	std::cout << std::endl;

	// =============================================
	// 4. int -> float: precision LOSS
	// =============================================
	std::cout << "=== 4. int -> float precision LOSS ===" << std::endl;
	std::cout << "float has 24-bit mantissa = 2^24 = 16777216" << std::endl;
	std::cout << "ints > 16777216 may lose precision in float" << std::endl;
	std::cout << std::endl;

	int n2 = 16777216;
	int n3 = 16777217;
	float f2 = static_cast<float>(n2);
	float f3 = static_cast<float>(n3);
	std::cout << "  16777216 -> float: " << f2 << "  exact? " << (static_cast<int>(f2) == n2 ? "YES" : "NO") << std::endl;
	std::cout << "  16777217 -> float: " << f3 << "  exact? " << (static_cast<int>(f3) == n3 ? "YES" : "NO") << std::endl;

	int n4 = INT_MAX;
	float f4 = static_cast<float>(n4);
	std::cout << "  INT_MAX  -> float: " << f4 << std::endl;
	std::cout << "  back to int: UB! (float > INT_MAX after rounding up)" << std::endl;
	std::cout << std::endl;

	// =============================================
	// 5. double -> float: precision loss + overflow
	// =============================================
	std::cout << "=== 5. double -> float ===" << std::endl;
	double d4 = 3.141592653589793;
	float f5 = static_cast<float>(d4);
	std::cout << "  pi as double: " << d4 << std::endl;
	std::cout << "  pi as float : " << static_cast<double>(f5) << std::endl;
	std::cout << "  lost digits : " << (d4 - static_cast<double>(f5)) << std::endl;
	std::cout << std::endl;

	double d5 = static_cast<double>(FLT_MAX) * 2.0;
	float f6 = static_cast<float>(d5);
	std::cout << "  FLT_MAX * 2 as double: " << d5 << std::endl;
	std::cout << "  cast to float        : " << f6 << "  (overflow -> inf)" << std::endl;
	std::cout << std::endl;

	// =============================================
	// 6. double -> char: the ex00 pipeline
	// =============================================
	std::cout << "=== 6. double -> char (ex00 pipeline) ===" << std::endl;
	std::cout << "Step: string -> double -> static_cast<char>" << std::endl;
	std::cout << std::endl;

	double values[] = {65.0, 97.0, 42.0, 32.0, 0.0, 127.0, 128.0, -1.0, 65.7};
	for (int i = 0; i < 9; i++) {
		double v = values[i];
		std::cout << "  " << std::setw(6) << v << " -> ";
		if (v < 0 || v > 127) {
			std::cout << "impossible (out of ASCII range)" << std::endl;
		} else {
			unsigned char uc = static_cast<unsigned char>(v);
			if (std::isprint(uc))
				std::cout << "'" << static_cast<char>(uc) << "'";
			else
				std::cout << "Non displayable";
			std::cout << "  (truncated to " << static_cast<int>(uc) << ")" << std::endl;
		}
	}
	std::cout << std::endl;

	// =============================================
	// 7. NaN and Inf behavior with casts
	// =============================================
	std::cout << "=== 7. NaN / Inf cast behavior ===" << std::endl;
	double nan_val = std::sqrt(-1.0);
	double inf_val = 1.0 / 0.0;
	double ninf_val = -1.0 / 0.0;

	std::cout << "  NaN == NaN       : " << (nan_val == nan_val ? "true" : "false")
			  << "  (always false!)" << std::endl;
	std::cout << "  NaN != NaN       : " << (nan_val != nan_val ? "true" : "false")
			  << "  (the v != v trick)" << std::endl;
	std::cout << "  NaN > 0          : " << (nan_val > 0 ? "true" : "false") << std::endl;
	std::cout << "  NaN < 0          : " << (nan_val < 0 ? "true" : "false") << std::endl;
	std::cout << "  NaN > INT_MAX    : " << (nan_val > INT_MAX ? "true" : "false") << std::endl;
	std::cout << "  NaN < INT_MIN    : " << (nan_val < INT_MIN ? "true" : "false") << std::endl;
	std::cout << std::endl;
	std::cout << "  +Inf > INT_MAX   : " << (inf_val > INT_MAX ? "true" : "false") << std::endl;
	std::cout << "  -Inf < INT_MIN   : " << (ninf_val < INT_MIN ? "true" : "false") << std::endl;
	std::cout << "  -> NaN fails ALL comparisons, so guard with v != v FIRST" << std::endl;

	return 0;
}
