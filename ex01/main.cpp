#include "Serializer.hpp"
#include <iostream>

int main()
{
	Data original;
	original.id = 42;
	original.name = "ecole";
	original.intra = "snakajim";

	std::cout << "=== Original Data ===" << std::endl;
	std::cout << "address: " << &original << std::endl;
	std::cout << "id:      " << original.id << std::endl;
	std::cout << "name:    " << original.name << std::endl;
	std::cout << "intra:   " << original.intra << std::endl;

	uintptr_t raw = Serializer::serialize(&original);
	std::cout << "\n=== Serialized ===" << std::endl;
	std::cout << "raw: " << raw << std::endl;

	Data* restored = Serializer::deserialize(raw);
	std::cout << "\n=== Deserialized ===" << std::endl;
	std::cout << "address: " << restored << std::endl;
	std::cout << "id:      " << restored->id << std::endl;
	std::cout << "name:    " << restored->name << std::endl;
	std::cout << "intra:   " << restored->intra << std::endl;

	std::cout << "\n=== Comparison ===" << std::endl;
	if (restored == &original)
		std::cout << "OK: pointers match" << std::endl;
	else
		std::cout << "KO: pointers differ" << std::endl;

	return 0;
}
