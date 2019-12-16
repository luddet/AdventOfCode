#include <iostream>

#include "Day10.h"

int main()
{
	using namespace Day10;
	// change to unordered set if needed for performance

	std::fstream fs("input.txt");

	std::set<point_t> asteroids(readAsteroidMap(fs));
	std::cout << "WTF!" << std::endl;

}
