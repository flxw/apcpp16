
#include <string>
#include <fstream>
#include <iostream>

#include "tea_filebuf.h"


void testEncryption()
{
	auto input = std::ifstream{};
	input.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	input.open("tea_test1_plain.dat", std::ios_base::binary | std::ios_base::ate);

	auto data = std::string(input.tellg(), '\0');
	input.seekg(0);
	input.read(&data[0], data.size());

	auto output = std::ofstream{};
	output.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	output.open("tea_test1_enc.dat", std::ios_base::binary);

	output << data;

	output.close();
}


void testDecryption()
{
	auto input = std::ifstream{};
	input.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	input.open("tea_test2_enc.dat", std::ios_base::binary | std::ios_base::ate);

	auto data = std::string(input.tellg(), '\0');
	input.seekg(0);
	input.read(&data[0], data.size());

	auto output = std::ofstream{};
	output.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	output.open("tea_test2_plain.dat", std::ios_base::binary);

	output << data;

	output.close();
}


int main(int argc, char const * argv[])
{
	try
	{
		testEncryption();
		testDecryption();
	}
	catch (std::ios_base::failure & e)
	{
		std::cout << e.what() << std::endl;
	}
}
