// https://adventofcode.com/2019/day/3

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <limits>

struct Coord
{
	int x;
	int y;
};

struct Line
{
	Coord start;
	Coord end;
};

// Manhattan length
int length(const Coord& coord)
{
	return std::abs(coord.x) + std::abs(coord.y);
}

bool intersects(const Line& line1, const Line& line2, Coord& intersecionPoint)
{
	int minX1 = std::min(line1.start.x, line1.end.x);
	int minY1 = std::min(line1.start.y, line1.end.y);
	int maxX1 = std::max(line1.start.x, line1.end.x);
	int maxY1 = std::max(line1.start.y, line1.end.y);

	int minX2 = std::min(line2.start.x, line2.end.x);
	int minY2 = std::min(line2.start.y, line2.end.y);
	int maxX2 = std::max(line2.start.x, line2.end.x);
	int maxY2 = std::max(line2.start.y, line2.end.y);

	if (maxX1 < minX2 || minX1 > maxX2 || maxY1 < minY2 || minY1 > maxY2)
		return false;

	intersecionPoint.x = minX1 == maxX1 ? minX1 : minX2;
	intersecionPoint.y = minY1 == maxY1 ? minY1 : minY2;
	return true;
}

std::vector<Line> parseLine(std::string& line)
{
	std::stringstream lineStream(line);
	std::vector<Line> result;
	Coord p1{ 0,0 };
	char direction;
	int distance;
	while (lineStream >> direction >> distance)
	{
		Coord p2{ p1.x, p1.y };
		switch (direction)
		{
		case 'U':
			p2.y += distance;
			break;
		case 'R':
			p2.x += distance;
			break;
		case 'D':
			p2.y -= distance;
			break;
		case 'L':
			p2.x -= distance;
			break;
		}
		result.push_back(Line{ p1, p2 });
		p1 = p2;

		// eat comma
		char _; lineStream >> _;
	}
	return result;
}

int main()
{
	std::ifstream fs("input.txt");

	std::string line1, line2;
	std::getline(fs, line1);
	std::getline(fs, line2);

	std::vector<Line> wire1, wire2;
	wire1 = parseLine(line1);
	wire2 = parseLine(line2);

	int closestDistance = INT_MAX;
	for (size_t i = 0; i < wire1.size(); ++i)
	{
		for (size_t j = 0; j < wire2.size(); ++j)
		{
			Coord intersection;
			if (intersects(wire1[i], wire2[j], intersection))
				closestDistance = std::min(closestDistance, length(intersection));
		}
	}

	std::cout << "Part 1: " << closestDistance << std::endl;
}

