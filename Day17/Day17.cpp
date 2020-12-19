#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <array>
#include <unordered_set>
#include <algorithm>

const char* EXAMPLE = 
	".#.\n"
	"..#\n"
	"###";

using coordinate_val_t = int64_t;
struct Coordinate
{
	coordinate_val_t x, y, z;
};

bool operator==(const Coordinate& c1, const Coordinate& c2) { return c1.x == c2.x && c1.y == c2.y && c1.z == c2.z; }
Coordinate operator+(const Coordinate& a, const Coordinate& b)
{
	return { a.x + b.x, a.y + b.y, a.z + b.z };
}

bool operator<(const Coordinate& c1, const Coordinate& c2)
{
	return c1.z < c2.z
		|| (c1.z == c2.z && c1.y < c2.y)
		|| (c1.z == c2.z && c1.y == c2.y && c1.x < c2.x);
};

namespace std
{
	template<> struct hash<Coordinate>
	{
		std::size_t operator()(Coordinate const& c) const noexcept
		{
			std::size_t h1 = std::hash<coordinate_val_t>{}(c.x);
			std::size_t h2 = std::hash<coordinate_val_t>{}(c.y);
			std::size_t h3 = std::hash<coordinate_val_t>{}(c.z);
			return h1 ^ (h2 << 1) ^ (h3 << 2); // or use boost::hash_combine (see Discussion)
		}
	};
}


using coordinates_t = std::unordered_set<Coordinate>;


std::ostream& operator<<(std::ostream& os, const coordinates_t& state)
{
	coordinate_val_t minX(INT64_MAX), minY(INT64_MAX), minZ(INT64_MAX);
	coordinate_val_t maxX(INT64_MIN), maxY(INT64_MIN), maxZ(INT64_MIN);
	std::for_each(state.begin(), state.end(),
		[&](const auto& c)
		{
			minX = std::min(minX, c.x);
			minY = std::min(minY, c.y);
			minZ = std::min(minZ, c.z);
			maxX = std::max(maxX, c.x);
			maxY = std::max(maxY, c.y);
			maxZ = std::max(maxZ, c.z);
		});

	for (coordinate_val_t z = minZ; z <= maxZ; ++z)
	{
		os << "z=" << z << std::endl;
		for (coordinate_val_t y = minY; y <= maxY; ++y)
		{
			for (coordinate_val_t x = minX; x <= maxX; ++x)
			{
				if (state.find({ x,y,z }) != state.end())
					os << '#';
				else
					os << '.';
			}
			os << std::endl;
		}
		os << std::endl;
	}
	return os;
}


coordinates_t parseInitialState(const std::string& input)
{
	coordinates_t coords;
	coordinate_val_t x(0), y(0);
	for (auto c : input)
	{
		if (c == '#')
		{
			coords.insert({ x, y, 0 });
		}
		else if (c == '\n')
		{
			x = 0;
			++y;
			continue;
		}
		++x;
	}
	return coords;
}

template<class F>
void iterateNeighbourhood(Coordinate coord, const F& function)
{
	using C = Coordinate;
	std::array<Coordinate, 26> offsets
	{
		C{1, 1, 1}, C{0, 1, 1}, C{-1, 1, 1}, C{1, 0, 1}, C{0, 0, 1}, C{-1, 0, 1}, C{1, -1, 1}, C{0, -1, 1}, C{-1, -1, 1},
		C{1, 1, 0}, C{0, 1, 0}, C{-1, 1, 0}, C{1, 0, 0},            C{-1, 0, 0}, C{1, -1, 0}, C{0, -1, 0}, C{-1, -1, 0},
		C{1, 1,-1}, C{0, 1,-1}, C{-1, 1,-1}, C{1, 0,-1}, C{0, 0,-1}, C{-1, 0, - 1}, C{1, -1,-1}, C{0, -1,-1}, C{-1, -1,-1}
	};
	std::for_each(offsets.begin(), offsets.end(), [&](const Coordinate o)
		{
			function(o + coord);
		});
}

uint64_t countNeighbourhood(Coordinate coord, const coordinates_t& state)
{
	uint64_t count(0);
	iterateNeighbourhood(coord, [&](const Coordinate& c) 
		{
			if (state.find(c) != state.end()) 
				count++; 
		});
	return count;
}

coordinates_t expandStateToNeighbours(const coordinates_t& state)
{
	coordinates_t result;
	for (auto& coord : state)
	{
		iterateNeighbourhood(coord, [&](const auto& c)
			{
				result.insert(c);
			});
	}
	return result;
}

void execute(coordinates_t& state, uint64_t iterations, std::ostream* os = nullptr)
{
	for (size_t currentIt = 0; currentIt < iterations; ++currentIt)
	{
		coordinates_t lastActive(state);
		coordinates_t expandedState{ expandStateToNeighbours(state) };
		coordinates_t lastInactive;
		std::copy_if(expandedState.begin(), expandedState.end(), std::inserter(lastInactive, lastInactive.begin()),
			[&](auto& c) { return lastActive.find(c) == lastActive.end(); });

		for (auto& coord : lastActive)
		{
			auto count = countNeighbourhood(coord, lastActive);
			if (count < 2 || count > 3)
				state.erase(coord);
		}

		for (auto& coord : lastInactive)
		{
			if (countNeighbourhood(coord, lastActive) == 3)
				state.insert(coord);
		}
		if (os != nullptr)
		{
			*os << "Iteration: " << currentIt + 1 << std::endl;
			*os << state << std::endl;
		}
	}
}

int main()
{
	
	std::string input = (std::ostringstream{} << (std::ifstream( "input.txt" )).rdbuf()).str();
	
	auto state = parseInitialState(input);

	std::cout << state;
	execute(state, 6, &std::cout);

	auto part1 = state.size();
	std::cout << "Day17 Part 1: " << part1 << std::endl;

}
