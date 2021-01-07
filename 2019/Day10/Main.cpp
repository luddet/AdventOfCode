
#define _USE_MATH_DEFINES // must be defined before anything else potentially includes <cmath>
#include "Day10.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
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
	point_t maxVisiblePoint{};
	for (auto point : asteroids)
	{
		auto visible = countVisible(asteroids, point, topLeft, bottomRight);
		if ((int)visible.size() > maxVisible)
		{
			maxVisible = (int)visible.size();
			maxVisiblePoint = point;
		}
	}
	std::cout << "Part 1: " << maxVisible << std::endl;

	struct lengthComparer_t
	{
		bool operator()(const point_t& p1, const point_t& p2) const
		{
			return length(p1) < length(p2);
		}
	};

	using sortedAsteroidsSet_t = std::set<point_t, lengthComparer_t>;
	using sortedAsteroids_t = std::map<double, sortedAsteroidsSet_t>;
	sortedAsteroids_t sortedAsteroids;

	auto calculateAngleForSorting = [&](const point_t& p1)
	{
		double a = -angle(p1) + M_PI_2;
		if (a < 0) a += 2 * M_PI;
		return a;
	};

	auto flipY = [](const point_t& p)
	{
		return point_t{ p.x, -p.y };
	};

	for (auto t : asteroids)
	{
		if (t == maxVisiblePoint)
			continue;

		double a = calculateAngleForSorting(flipY(t - maxVisiblePoint));

		sortedAsteroidsSet_t* set = nullptr;
		auto s = sortedAsteroids.find(a);
		if (s == sortedAsteroids.end())
		{
			auto inserted = sortedAsteroids.insert(std::make_pair(a, sortedAsteroidsSet_t{}));
			set = &inserted.first->second;
		}
		else
		{
			set = &s->second;
		}

		set->insert(t);
	}

	size_t asteroidsDestroyed = 0;
	point_t lastDestroyed{};
	while (!sortedAsteroids.empty() && asteroidsDestroyed < 200)
	{
		std::vector<double> emptySets;
		auto pair_it = sortedAsteroids.begin();
		while (pair_it != sortedAsteroids.end() && asteroidsDestroyed < 200)
		{
			auto& set = pair_it->second;
			assert(!set.empty());
			auto currentPoint_it = set.begin();
			lastDestroyed = *currentPoint_it;
			set.erase(currentPoint_it);
			if (set.empty())
				emptySets.push_back(pair_it->first);
			++asteroidsDestroyed;
			++pair_it;
		}

		for (auto a : emptySets)
			sortedAsteroids.erase(sortedAsteroids.find(a));
		emptySets.clear();
	}

	std::cout << "num destroyed: " << asteroidsDestroyed << std::endl;
	std::cout << "num 200: " << lastDestroyed << std::endl;
	std::cout << "Part 2: " << lastDestroyed.x * 100 + lastDestroyed.y << std::endl;

}

