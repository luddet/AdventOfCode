#include <iostream>
#include <string>

#include "../../2021/Utilities/utilities.h"

int main()
{
	auto input = split(getInput(2022, 10));
	int x{ 1 };
	size_t ic{ 0 };
	int cycle{ 1 };
	int signalSum{ 0 };
	for (auto ip = input.cbegin(); ip != input.cend(); )
	{
		if (cycle % 40 == 20)
			signalSum += cycle * x;

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
}
