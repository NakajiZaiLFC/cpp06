#include "ConversionHelper.hpp"
#include <iostream>
#include <sstream>
#include <limits>

std::string toDecimalString(double v, const std::string& suffix)
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

std::string makeCharStr(double v)
{
	std::ostringstream oss;
	if (v != v || v < 0 || std::numeric_limits<char>::max() < v) return "impossible";
	if (!std::isprint(static_cast<unsigned char>(v))) return "Non displayable";
	oss << "'" << static_cast<char>(v) << "'";
	return oss.str();
}

std::string makeIntStr(double v)
{
	std::ostringstream oss;
	if (v != v || v > std::numeric_limits<int>::max() || v < std::numeric_limits<int>::min()) return "impossible";
	oss << static_cast<int>(v);
	return oss.str();
}

std::string makeFloatStr(double v)
{
	return toDecimalString(static_cast<float>(v), "f");
}

std::string makeDoubleStr(double v)
{
	return toDecimalString(v, "");
}

ConversionResult buildResult(double v)
{
	ConversionResult r;
	r.charStr = makeCharStr(v);
	r.intStr = makeIntStr(v);
	r.floatStr = makeFloatStr(v);
	r.doubleStr = makeDoubleStr(v);
	return r;
}
