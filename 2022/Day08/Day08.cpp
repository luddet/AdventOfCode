#include <algorithm>
#include <array>
#include <deque>
#include <fstream>
#include <iostream>
#include <limits>
#include <numeric>
#include <string>
#include <vector>
#include "../../Utilities/Utilities/utilities.h"

const std::string testInput = 
R"(30373
25512
65332
33549
35390)";

int main()
{
	//const auto input{ split(testInput) };
	const auto input{ split(getInput(2022,8)) };
	const auto width{ int(input[0].length()) };
	const auto height{ int(input.size()) };

	std::deque<bool> visibilityGrid(height * width, false);
	std::vector<uint8_t> trees;
	for (const auto& line : input)
		std::transform(cbegin(line), cend(line), std::back_inserter(trees), [](auto c) { return uint8_t(c - '0'); });

	auto checkTree = [&visibilityGrid, &trees, width](int row, int col, int currentMax) -> int
	{
		size_t index = row * width + col;
		if (trees[index] > currentMax)
		{
			currentMax = trees[index];
			visibilityGrid[index] = true;
		}
		return currentMax;
	};

	// Left and Right
	for (int row = 0; row < height; ++row)
	{
		// Check from left
		int currentMax{ -1 };
		for (int col = 0; col < width; ++col)
		{
			currentMax = checkTree(row, col, currentMax);
			if (currentMax == 9)
				break;
		}
		// Check from right
		currentMax = -1;
		for (int col = width-1; col >= 0; --col)
		{
			currentMax = checkTree(row, col, currentMax);
			if (currentMax == 9)
				break;
		}
	}

	// Down and Up
	for (int col = 0; col < width; ++col)
	{
		// Check from top
		int currentMax{ -1 };
		for (int row = 0; row < height; ++row)
		{
			currentMax = checkTree(row, col, currentMax);
			if (currentMax == 9)
				break;
		}
		// Check from bottom
		currentMax = -1;
		for (int row = height-1; row >= 0; --row)
		{
			currentMax = checkTree(row, col, currentMax);
			if (currentMax == 9)
				break;
		}
	}

	const auto part1 = std::accumulate(cbegin(visibilityGrid), cend(visibilityGrid), 0);

	auto scoreTree = [&trees, width, height](auto row, auto col) -> int
	{
		auto currentTreeHeight = trees[row * width + col];
		std::vector<int> multipliers;
		
		{
			int r = row - 1;
			while (trees[r * width + col] < currentTreeHeight && r > 0)
				--r;
			multipliers.push_back(row - r);

			r = row + 1;
			while (trees[r * width + col] < currentTreeHeight && r < height - 1)
				++r;
			multipliers.push_back(r - row);
		}

		{
			int c = col - 1;
			while (trees[row * width + c] < currentTreeHeight && c > 0)
				--c;
			multipliers.push_back(col - c);

			c = col + 1;
			while (trees[row * width + c] < currentTreeHeight && c < width - 1)
				++c;
			multipliers.push_back(c - col);
		}

		return std::accumulate(cbegin(multipliers), cend(multipliers), 1, std::multiplies());
	};

	int part2{ 0 };
	for (int row = 1; row < height - 1; ++row)
	{
		for (int col = 1; col < width - 1; ++col)
		{
			int score = scoreTree(row, col);
			if (score > part2)
				part2 = score;
		}
	}

	std::cout << "Day08 Part 1: " << part1 << '\n';
	std::cout << "Day08 Part 2: " << part2 << '\n';
}
