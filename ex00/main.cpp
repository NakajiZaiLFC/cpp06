#include <iostream>
#include <limits>

int main(){
	testCase();
	
}

void testCase(){
	std::cout<< (double)42.0 << std::endl;
	std::cout<< (double)0.0 << std::endl;
	std::cout<< (double)3.14 << std::endl;
	std::cout<< (float)42.0f << std::endl;
	std::cout<< (char)42 << std::endl;
	std::cout<< (char)200 << std::endl;
	std::cout<< (int)3.99 << std::endl;
	std::cout<< static_cast<char>(300) << std::endl;
	std::cout<< std::numeric_limits<double>::quiet_NaN() << std::endl;
}