// https://adventofcode.com/2019/day/10

#include <vector>
#include <functional>
#include <set>
#include <algorithm>

#include "Day10.h"
namespace Day10
{


int sign(int x)
{
	return (x > 0) - (x < 0);
}

point_t operator+(const point_t a, const point_t b)
{
	return { a.first + b.first, a.second + b.second };
}

point_t operator-(const point_t a, const point_t b)
{
	return { a.first - b.first, a.second - b.second };
}


int manhattanDistance(point_t p1, point_t p2)
{
	return std::abs(p2.first - p1.first) + std::abs(p2.second - p1.second);
}

std::ostream& operator<<(std::ostream& os, const point_t& p)
{
	return os << "{" << p.first << ", " << p.second << "}";
}


// Probably not the most efficient algorithm
point_t reduceByLCD(point_t p)
{
	int signX = sign(p.first);
	int signY = sign(p.second);

	p.first *= signX;
	p.second *= signY;

	if (p.first == 0 || p.second == 0)
	{
		p.first = signX;
		p.second = signY;
		return p;
	}

	int i = 2;
	while (i <= p.first && i <= p.second)
	{
		int first = p.first % i;
		int second = p.second % i;

		while (first == 0 && second == 0)
		{
			p.first = p.first / i;
			p.second = p.second / i;
			first = p.first % i;
			second = p.second % i;
		}
		++i;
	}
	p.first *= signX;
	p.second *= signY;
	return p;
}

std::set<point_t> readAsteroidMap(std::istream& stream)
{
	std::set<point_t> asteroids;

	std::string line;

	int y = 0;
	while (std::getline(stream, line))
	{
		for (int x = 0; x < line.length(); ++x)
		{
			if (line[x] == '#')
				asteroids.insert({ x,y });
		}
		++y;
	}
	return asteroids;
}

std::vector<point_t> reduceToVisible(const std::set<point_t>& asteroids, point_t origin)
{
	std::set<point_t> culledPoints;
	std::vector<point_t> result;

	int minX = INT_MAX;
	int minY = INT_MAX;
	int maxX = INT_MIN;
	int maxY = INT_MIN;

	// Find bounds
	std::for_each(asteroids.cbegin(), asteroids.cend(), [&](Day10::point_t p)
				  {
					  minX = std::min(p.first, minX);
					  minY = std::min(p.second, minY);
					  maxX = std::max(p.first, maxX);
					  maxY = std::max(p.second, maxY);
				  }
	);

	auto sorter = [&](point_t p1, point_t p2) { return manhattanDistance(p1, origin) < manhattanDistance(p2, origin); };
	std::vector<point_t> sorted(asteroids.begin(), asteroids.end());
	std::sort(sorted.begin(), sorted.end(), sorter);

	for (auto point : sorted)
	{
		if (point == origin)
			continue;
		if (culledPoints.find(point) != culledPoints.end())
			continue;

		auto step = reduceByLCD(point-origin);
		auto currentCoord = point + step;
		while (currentCoord.first >= minX && currentCoord.first <= maxX && currentCoord.second >= minY && currentCoord.second <= maxY)
		{
			if (asteroids.find(currentCoord) != asteroids.end())
				culledPoints.insert(currentCoord);
			currentCoord = currentCoord + step;
		}

		result.push_back(point);

	}
	return result;
}



} // namespace Day10


