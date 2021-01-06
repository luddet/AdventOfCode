// https://adventofcode.com/2019/day/12
//

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>

struct vector_t
{
	int x;
	int y;
	int z;
};

int sign(int i)
{
	return (i > 0) - (i < 0);
}

vector_t sign(const vector_t& v)
{
	return { sign(v.x), sign(v.y), sign(v.z) };
}

vector_t& operator+=(vector_t& lhs, const vector_t& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	return lhs; 
}

vector_t operator-(const vector_t& v)
{
	return { -v.x,-v.y, -v.z };
}

vector_t operator-(const vector_t& lhs, const vector_t& rhs)
{
	return {lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
}

std::ostream& operator<<(std::ostream& os, const vector_t& v)
{
	return os << "<" << v.x << ", " << v.y << ", " << v.z << ">";
}

std::istream& operator>>(std::istream& is, vector_t& v)
{
	// format <x = -1, y = 5, z = 2>
	char c;
	return is >> std::skipws >> c >> c >> c >> v.x >> c >> c >> c >> v.y >> c >> c >> c >> v.z >> c;
}

std::string toString(std::vector<vector_t> positions, std::vector<vector_t> velocities)
{
	assert(positions.size() == velocities.size());
	std::ostringstream ss;
	for (size_t i = 0; i < positions.size(); ++i)
		ss << "pos=" << positions[i] << ", vel=" << velocities[i] << std::endl;
	return ss.str();
}


int main()
{
	std::fstream fs("input.txt");
	std::vector<vector_t> positions;
	std::vector<vector_t> velocities;

	std::istringstream testStream1("<x=-1, y=0, z=2>"
								   "< x = 2, y = -10, z = -7 >"
								   "<x = 4, y = -8, z = 8>"
								   "< x = 3, y = 5, z = -1>");

	std::istringstream testStream2("<x=-8, y=-10, z=0>"
								  "< x = 5, y = 5, z = 10 >"
								  "<x = 2, y = -7, z = 3>"
								  "< x = 9, y = -8, z = -3>");
	std::istream& is = fs;
	vector_t v;
	while (is >> v)
	{
		positions.push_back(v);
		velocities.push_back({ 0,0,0 });
	}

	std::vector<vector_t> originalPositions(positions);
	std::vector<vector_t> originalVelocities(velocities);

	constexpr int part1TimeSteps = 1000;
	std::vector<vector_t> part1Positions;
	std::vector<vector_t> part1Velocities;

	//std::cout << toString(positions, velocities) << std::endl;

	
	size_t periodX(0), periodY(0), periodZ(0);
	bool foundPeriodX = false, foundPeriodY = false, foundPeriodZ = false;

	for (size_t time = 0; time < part1TimeSteps || !foundPeriodX || !foundPeriodY || !foundPeriodZ; ++time)
	{
		// update velocities
		for (size_t p1i = 0; p1i < velocities.size() - 1; ++p1i)
		{
			for (size_t p2i = p1i + 1; p2i < velocities.size(); ++p2i)
			{
				auto p1 = positions[p1i];
				auto p2 = positions[p2i];
				auto dp = p2 - p1;

				dp = sign(dp);

				velocities[p1i] += dp;
				velocities[p2i] += -dp;
			}
		}

		// update positions
		for (size_t pi = 0; pi < positions.size(); ++pi)
		{
			positions[pi] += velocities[pi];
		}


		if (time == part1TimeSteps - 1)
		{
			std::copy(positions.begin(), positions.end(), std::back_inserter(part1Positions));
			std::copy(velocities.begin(), velocities.end(), std::back_inserter(part1Velocities));
		}

		if (!foundPeriodX)
		{
			bool allXAreAtOriginalState = true;
			for (size_t i = 0; i < positions.size() && allXAreAtOriginalState; ++i)
			{
				allXAreAtOriginalState &= positions[i].x == originalPositions[i].x;
				allXAreAtOriginalState &= velocities[i].x == originalVelocities[i].x;
			}
			if (allXAreAtOriginalState)
			{
				foundPeriodX = true;
				periodX = time + 1;
			}
		}

		if (!foundPeriodY)
		{
			bool allYAreAtOriginalState = true;
			for (size_t i = 0; i < positions.size() && allYAreAtOriginalState; ++i)
			{
				allYAreAtOriginalState &= positions[i].y == originalPositions[i].y;
				allYAreAtOriginalState &= velocities[i].y == originalVelocities[i].y;
			}
			if (allYAreAtOriginalState)
			{
				foundPeriodY = true;
				periodY = time + 1;
			}
		}

		if (!foundPeriodZ)
		{
			bool allZAreAtOriginalState = true;
			for (size_t i = 0; i < positions.size() && allZAreAtOriginalState; ++i)
			{
				allZAreAtOriginalState &= positions[i].z == originalPositions[i].z;
				allZAreAtOriginalState &= velocities[i].z == originalVelocities[i].z;
			}
			if (allZAreAtOriginalState)
			{
				foundPeriodZ = true;
				periodZ = time + 1;
			}
		}

		//std::cout << toString(positions, velocities) << std::endl;
	}

	// calculate energy
	int energy{ 0 };
	for (size_t i = 0; i < part1Positions.size(); ++i)
	{
		energy += (abs(part1Positions[i].x) + abs(part1Positions[i].y) + abs(part1Positions[i].z)) * (abs(part1Velocities[i].x) + abs(part1Velocities[i].y) + abs(part1Velocities[i].z));
	}

	std::cout << "Part 1: " << energy << std::endl;

	assert(periodX > 0 && periodY > 0 && periodZ > 0);

	// divide by common factors
	{
		size_t factor = 2;
		while (factor < periodX / factor && factor < periodY / factor && factor < periodZ /factor)
		{
			while (periodX % factor == 0 && periodY % factor == 0 && periodZ % factor == 0)
			{
				periodX /= factor;
				periodY /= factor;
				periodZ /= factor;
			}

			factor++;
		}
	}

	// multiply rest for total period
	long long totalPeriod = periodX * periodY * periodZ;
	std::cout << "Part 2: " << totalPeriod << std::endl;

}

