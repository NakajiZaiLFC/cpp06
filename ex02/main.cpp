#include "Functions.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	for (int i = 0; i < 5; i++)
	{
		Base* p = generate();
		std::cout << "ptr: ";
		identify(p);
		std::cout << "ref: ";
		identify(*p);
		delete p;
		std::cout << std::endl;
	}
	return 0;
}
