// https://adventofcode.com/2019/day/10
#define _USE_MATH_DEFINES // must be defined before anything else potentially includes <cmath>

#include "Day10.h"

#include <functional>
#include <algorithm>
#include <string>
#include <cmath>

namespace Day10
{


int sign(int x)
{
	return (x > 0) - (x < 0);
}

bool operator==(const point_t lhs, const point_t rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

point_t operator+(const point_t lhs, const point_t rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

point_t operator-(const point_t lhs, const point_t rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

bool operator<(const point_t lhs, const point_t rhs)
{
	return lhs.x < rhs.x || lhs.x == rhs.x && lhs.y < rhs.y;
}

int manhattanDistance(point_t p1, point_t p2)
{
	return std::abs(p2.x - p1.x) + std::abs(p2.y - p1.y);
}

double length(point_t p)
{
	return sqrt(p.x * p.x + p.y * p.y);
}

double angle(point_t p)
{
	double angle = atan2(p.y, p.x);
	return (angle >= 0) ? angle : angle + 2 * M_PI;
}

std::ostream& operator<<(std::ostream& os, const point_t& p)
{
	return os << "{" << p.x << ", " << p.y << "}";
}

// Probably not the most efficient algorithm
point_t reduceByLCD(point_t p)
{
	int signX = sign(p.x);
	int signY = sign(p.y);

	p.x *= signX;
	p.y *= signY;

	if (p.x == 0 || p.y == 0)
	{
		p.x = signX;
		p.y = signY;
		return p;
	}

	int i = 2;
	while (i <= p.x && i <= p.y)
	{
		int first = p.x % i;
		int second = p.y % i;

		while (first == 0 && second == 0)
		{
			p.x = p.x / i;
			p.y = p.y / i;
			first = p.x % i;
			second = p.y % i;
		}
		++i;
	}
	p.x *= signX;
	p.y *= signY;
	return p;
}

std::set<point_t> readAsteroidMap(std::istream& stream)
{
	std::set<point_t> asteroids;

	std::string line;

	int y = 0;
	while (std::getline(stream, line))
	{
		for (size_t x = 0; x < line.length(); ++x)
		{
			if (line[x] == '#')
				asteroids.insert({(int)x,y });
		}
		++y;
	}
	return asteroids;
}


std::vector<point_t> countVisible(const std::set<point_t>& asteroids, point_t origin, point_t boundTopLeft, point_t boundBottomRight)
{
	std::set<point_t> culledPoints;
	std::vector<point_t> result;

	int minX = boundTopLeft.x;
	int minY = boundTopLeft.y;
	int maxX = boundBottomRight.x;
	int maxY = boundBottomRight.y;

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
		while (currentCoord.x >= minX && currentCoord.x <= maxX && currentCoord.y >= minY && currentCoord.y <= maxY)
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


