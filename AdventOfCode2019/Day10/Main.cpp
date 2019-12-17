#include "Day10.h"

#include <iostream>
#include <algorithm>
#include <fstream>

int main()
{
	using namespace Day10;

	std::fstream fs("input.txt");

	// change to unordered set if needed for performance
	std::set<point_t> asteroids(readAsteroidMap(fs));

	point_t topLeft, bottomRight;
	findBounds(asteroids, topLeft, bottomRight);

	int maxVisible = INT_MIN;
	for (auto point : asteroids)
	{
		auto visible = countVisible(asteroids, point, topLeft, bottomRight);
		maxVisible = std::max((int)visible.size(), maxVisible);
	}
	std::cout << "Part 1: " << maxVisible << std::endl;

}
