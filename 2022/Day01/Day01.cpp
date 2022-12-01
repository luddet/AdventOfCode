#include <fstream>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <array>
#include <numeric>


#include "../../2021/Utilities/utilities.h"

int main()
{
	auto lines = readLines("input.txt");

	std::vector<int> calories;
	int currentCalories{ 0 };
	for (auto& line : lines)
	{
		if (line.size() == 0)
		{
			calories.push_back(currentCalories);
			std::push_heap(begin(calories), end(calories));
			currentCalories = 0;
		}
		else
			currentCalories += std::atoi(line.c_str());
	}

	std::array<int, 3> topCals{};
	for (int i = 0; i < topCals.size(); ++i)
	{
		topCals[i] = calories.front();
		std::pop_heap(begin(calories), end(calories));
		calories.pop_back();
	}

	std::cout << "Day01 Part 1: " << topCals.front() << '\n';
	std::cout << "Day01 Part 2: " << std::accumulate(begin(topCals), end(topCals), 0) << '\n';
}
