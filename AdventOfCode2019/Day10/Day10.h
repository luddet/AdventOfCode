#pragma once
#include <set>
#include <vector>
#include <string>
#include <fstream>

namespace Day10
{

using point_t = std::pair<int, int>;

int sign(int x);

int manhattanDistance(point_t p1, point_t p2);

// Probably not the most efficient algorithm
point_t reduceByLCD(point_t p);

std::set<point_t> readAsteroidMap(std::istream& stream);

std::vector<point_t> reduceToVisible(const std::set<point_t>& asteroids, point_t origin);

}