#include <iostream>
#include <algorithm>
#include "Day10.h"

int main()
{
	using namespace Day10;

	std::fstream fs("input.txt");

	// change to unordered set if needed for performance
	std::set<point_t> asteroids(readAsteroidMap(fs));

	int maxVisible = INT_MIN;
	for (auto point : asteroids)
	{
		auto visible = reduceToVisible(asteroids, point);
		maxVisible = std::max((int)visible.size(), maxVisible);
	}
	std::cout << "Part 1: " << maxVisible << std::endl;

}
