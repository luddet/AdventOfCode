
#define _USE_MATH_DEFINES // must be defined before anything else potentially includes <cmath>
#include "Day10.h"

#include <iostream>
#include <algorithm>
#include <fstream>
#include <set>
#include <cmath>

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

	point_t origin{};
	auto angleComparer = [&](const point_t& p1, const point_t& p2) 
	{
		point_t op1 = p1 - origin;
		point_t op2 = p2 - origin;
		double angle_op1 = -angle(op1) - M_PI_2;
		double angle_op2 = -angle(op2) - M_PI_2;
		if (angle_op1 < 0) angle_op1 += 2 * M_PI;
		if (angle_op2 < 0) angle_op2 += 2 * M_PI;

		return angle_op1 < angle_op2 ? true : (angle_op1 == angle_op2 && length(op1) < length(op2)) ? true : false;
	};

	std::set<point_t, decltype(angleComparer)> sortedAsteroids(angleComparer);

	sortedAsteroids.insert({ 1, 1 });
	sortedAsteroids.insert({ 1, 0 });
	sortedAsteroids.insert({ 1, -1 });
	sortedAsteroids.insert({ -2, -2 });
	sortedAsteroids.insert({ -1, -1 });
	sortedAsteroids.insert({ -1, 0 });
	sortedAsteroids.insert({ -1, 1 });
	sortedAsteroids.insert({ 0, 2 });
	sortedAsteroids.insert({ 0, 1 });


	for (auto a : sortedAsteroids)
	{
		std::cout << a << std::endl;
	}

}

