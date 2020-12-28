#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>

const char* EXAMPLE =
"sesenwnenenewseeswwswswwnenewsewsw\n"
"neeenesenwnwwswnenewnwwsewnenwseswesw\n"
"seswneswswsenwwnwse\n"
"nwnwneseeswswnenewneswwnewseswneseene\n"
"swweswneswnenwsewnwneneseenw\n"
"eesenwseswswnenwswnwnwsewwnwsene\n"
"sewnenenenesenwsewnenwwwse\n"
"wenwwweseeeweswwwnwwe\n"
"wsweesenenewnwwnwsenewsenwwsesesenwne\n"
"neeswseenwwswnwswswnw\n"
"nenwswwsewswnenenewsenwsenwnesesenew\n"
"enewnwewneswsewnwswenweswnenwsenwsw\n"
"sweneswneswneneenwnewenewwneswswnese\n"
"swwesenesewenwneswnwwneseswwne\n"
"enesenwswwswneneswsenwnewswseenwsese\n"
"wnwnesenesenenwwnenwsewesewsesesew\n"
"nenewswnwewswnenesenwnesewesw\n"
"eneswnwswnwsenenwnwnwwseeswneewsenese\n"
"neswnwewnwnwseenwseesewsenwsweewe\n"
"wseweeenwnesenwwwswnew\n";

struct HexCoord
{
	using val_t = int32_t;
	val_t x;
	val_t y;

	bool operator==(const HexCoord& o) const
	{
		return x == o.x && y == o.y;
	}

	HexCoord& operator+=(const HexCoord& o)
	{
		x += o.x;
		y += o.y;
		return *this;
	}
};

HexCoord operator+(const HexCoord& a, const HexCoord& b)
{
	return HexCoord{ a } += b;
}

namespace std
{
	template<>
	struct hash<HexCoord>
	{
		size_t operator()(const HexCoord& c) const
		{
			return (hash<HexCoord::val_t>()(c.x) << 0)
				^ (hash<HexCoord::val_t>()(c.y) << 1);
		}
	};
}

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");
	std::string line;
	std::vector<std::vector<HexCoord>> paths{};
	while (std::getline(is, line))
	{
		if (line.empty()) continue;
		paths.push_back({});
		std::istringstream ls(line);
		char c;
		while (ls >> c)
		{
			std::ostringstream i;
			i << c;
			if (c == 's' || c == 'n')
			{
				ls >> c;
				i << c;
			}

			HexCoord coord{};
			if (i.str() == "e")
				coord = { 1, 0 };
			else if (i.str() == "w")
				coord = { -1, 0 };
			else if (i.str() == "ne")
				coord = { 1, -1 };
			else if (i.str() == "nw")
				coord = { 0, -1 };
			else if (i.str() == "sw")
				coord = { -1, 1 };
			else if (i.str() == "se")
				coord = { 0, 1 };
			else
				throw std::exception("Parse fail");
			paths.back().push_back(coord);
		}
	}

	std::unordered_set<HexCoord> tiles{};
	for (auto path : paths)
	{
		HexCoord c{};
		for (auto coord : path)
			c += coord;

		auto it = tiles.find(c);
		if (it == tiles.end())
			tiles.insert(c);
		else
			tiles.erase(c);
	}
	auto part1 = tiles.size();
	std::cout << "Day24 Part 1: " << part1 << std::endl;


}

