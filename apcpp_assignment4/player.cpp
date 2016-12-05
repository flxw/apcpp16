
#include <random>
#include <string>
#include <thread>
#include <chrono>
#include <iostream>

#include "ipc_player.h"

using namespace std::chrono_literals;


int main(int argc, char const * argv[])
{
	if (argc != 2)
	{
		return EXIT_FAILURE;
	}

	try
	{
		const auto id = std::stoi(argv[1]);

		auto prng = std::mt19937{std::random_device{}()};
		auto randomDist = std::uniform_int_distribution<std::uint32_t>{1, 6};

		auto score = readScore(id);

		while (true)
		{
			score += randomDist(prng);
			updateScore(id, score);

			std::this_thread::sleep_for(100ms);
		}
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}