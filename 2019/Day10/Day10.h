#pragma once
#include <istream>
#include <set>
#include <vector>
#include <algorithm>


namespace Day10
{

struct point_t
{
	int x;
	int y;
};

bool operator==(const point_t lhs, const point_t rhs);
point_t operator+(const point_t lhs, const point_t rhs);
point_t operator-(const point_t lhs, const point_t rhs);

// "arbitrary" less than, to enable use in ordered containers
bool operator<(const point_t lhs, const point_t rhs);

std::ostream& operator<<(std::ostream& os, const point_t& p);

//using point_t = std::pair<int, int>;

int sign(int x);

int manhattanDistance(point_t p1, point_t p2);
double length(point_t p);
double angle(point_t p);

// Probably not the most efficient algorithm
point_t reduceByLCD(point_t p);

std::set<point_t> readAsteroidMap(std::istream& stream);

std::vector<point_t> countVisible(const std::set<point_t>& asteroids, point_t origin, point_t boundTopLeft, point_t boundBottomRight);


template<class T>
void findBounds(const T& pointContainer, point_t& boundTopLeft, point_t& boundBottomRight)
{
	boundTopLeft = { INT_MAX, INT_MAX };
	boundBottomRight = { INT_MIN, INT_MIN };

	// Find bounds
	std::for_each(pointContainer.cbegin(), pointContainer.cend(), [&](Day10::point_t p)
				  {
					  boundTopLeft.x = std::min(p.x, boundTopLeft.x);
					  boundTopLeft.y = std::min(p.y, boundTopLeft.y);
					  boundBottomRight.x = std::max(p.x, boundBottomRight.x);
					  boundBottomRight.y = std::max(p.y, boundBottomRight.y);
				  }
	);
}

}