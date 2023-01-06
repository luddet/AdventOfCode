#include <array>
#include <iostream>
#include <string>

#include "../../Utilities/Utilities/utilities.h"

int main()
{
	auto input = split(getInput(2022, 10));
	
	std::array<std::string, 6> display{};
	int x{ 1 };
	size_t ic{ 0 };
	int cycle{ 1 };
	int signalSum{ 0 };

	for (auto ip = input.cbegin(); ip != input.cend(); )
	{
		if (cycle % 40 == 20)
			signalSum += cycle * x;

		int pos = (cycle-1) % 40;
		int row = (cycle-1) / 40;
		display[row] += (abs(pos - x) <= 1) ? '#' : ' ';

		if (ip->starts_with("addx"))
		{
			if (ic == 0)
				ic = 2;
			
			if (--ic == 0)
			{
				int operand = std::stoi(ip->substr(5));
				x += operand;
			}
		}

		++cycle;
		if (ic == 0) // get new instruction
			++ip;
	}
	
	std::cout << "Day10 Part 1: " << signalSum << '\n';
	std::cout << "Day10 Part 2: " << '\n';
	for (const auto& line : display)
		std::cout << line << '\n';

}
