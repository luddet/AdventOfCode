#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <unordered_set>
#include <array>
#include <algorithm>
#include <iomanip>

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

class HexCoord
{
public:
	using val_t = int32_t;

	HexCoord() : m_x(0), m_y(0) {}
	HexCoord(const std::string& dir)
	{
		if (dir == "e")
		{
			m_x = 1;
			m_y = 0;
		}
		else if (dir == "w")
		{
			m_x = -1;
			m_y = 0;
		}
		else if (dir == "ne")
		{
			m_x = 1;
			m_y = -1;
		}
		else if (dir == "nw")
		{
			m_x = 0;
			m_y = -1;
		}
		else if (dir == "sw")
		{
			m_x = -1;
			m_y = 1;
		}
		else if (dir == "se")
		{
			m_x = 0;
			m_y = 1;
		}
		else
			throw std::exception("Parse fail");
	}

	val_t x() const { return m_x; }
	val_t y() const { return m_y; }

	bool operator==(const HexCoord& o) const
	{
		return m_x == o.m_x && m_y == o.m_y;
	}

	HexCoord& operator+=(const HexCoord& o)
	{
		m_x += o.m_x;
		m_y += o.m_y;
		return *this;
	}

private:
	val_t m_x;
	val_t m_y;
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
			return (hash<HexCoord::val_t>()(c.x()) << 0)
				^ (hash<HexCoord::val_t>()(c.y()) << 1);
		}
	};
}

std::array<HexCoord, 6> getNeighbours(const HexCoord& c)
{
	const static std::array<HexCoord, 6> n{ HexCoord("nw"), HexCoord("ne"),HexCoord("w"),HexCoord("e"),HexCoord("sw"),HexCoord("se") };
	return {c + n[0], c + n[1], c + n[2], c + n[3], c + n[4], c + n[5]};
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

			
			HexCoord coord{ i.str() };
			paths.back().push_back(coord);
		}
	}

	std::unordered_set<HexCoord> blackTiles{};
	for (auto path : paths)
	{
		HexCoord c{};
		for (auto coord : path)
			c += coord;

		auto it = blackTiles.find(c);
		if (it == blackTiles.end())
			blackTiles.insert(c);
		else
			blackTiles.erase(c);
	}
	auto part1 = blackTiles.size();

	std::unordered_set<HexCoord> blackNeighbours{}, whiteToFlip{}, blackToFlip{};

	for (size_t i = 0; i < 100; ++i)
	{
		for (auto& black : blackTiles)
		{
			auto n = getNeighbours(black);
			auto nCount = std::count_if(n.begin(), n.end(), [&](const HexCoord& c) { return blackTiles.find(c) != blackTiles.end(); });
			if (nCount == 0 || nCount > 2)
				blackToFlip.insert(black);
		}

		for (auto& blackTile : blackTiles)
		{
			auto neighbours = getNeighbours(blackTile);
			for (auto& n : neighbours)
				if (blackTiles.find(n) == blackTiles.end())
					blackNeighbours.insert(n);
		}

		for (auto& white : blackNeighbours)
		{
			auto n = getNeighbours(white);
			auto nCount = std::count_if(n.begin(), n.end(), [&](const HexCoord& c) { return blackTiles.find(c) != blackTiles.end(); });
			if (nCount == 2)
				whiteToFlip.insert(white);
		}

		for (auto w : whiteToFlip)
			blackTiles.insert(w);
		for (auto b : blackToFlip)
			blackTiles.erase(b);

		blackNeighbours.clear();
		whiteToFlip.clear();
		blackToFlip.clear();
	}

	auto part2 = blackTiles.size();

	std::cout << "Day24 Part 1: " << part1 << std::endl;
	std::cout << "Day24 Part 2: " << part2 << std::endl;
}

