#pragma once
#include "ScalarConverter.hpp"
#include <string>

std::string toDecimalString(double v, const std::string& suffix);
std::string intToString(int n);
std::string makeCharStr(double v);
std::string makeIntStr(double v);
std::string makeFloatStr(double v);
std::string makeDoubleStr(double v);
ConversionResult buildResult(double v);
