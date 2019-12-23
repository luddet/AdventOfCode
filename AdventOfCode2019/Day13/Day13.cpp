// https://adventofcode.com/2019/day/13
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>

#include "Computer.h"

struct coord_t
{
	int x;
	int y;
};
coord_t& operator+=(coord_t& lhs, const coord_t& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

bool operator<(const coord_t& lhs, const coord_t& rhs)
{
	return (lhs.x < rhs.x) ? true : (lhs.x == rhs.x && lhs.y < rhs.y);
}

enum class Tile : uint8_t
{
	Empty = 0,
	Wall,
	Block,
	HorizontalPaddle,
	Ball,
	TiletypesCount
};

int main()
{
	std::fstream fs("input.txt");
	auto mem = readMemory(fs);
	Computer c(mem);

	while (!c.isDone())
		c.run();

	std::map<coord_t, Tile> tileMap;

	Computer::mem_item_t x, y, tile_id;
	while (c.getOutput(x))
	{
		c.getOutput(y);
		c.getOutput(tile_id);

		coord_t coord{ (int)x, (int)y };
		auto it = tileMap.find(coord);
		if (it == tileMap.end())
			tileMap.insert(std::make_pair(coord, (Tile)tile_id));
		else
			it->second = (Tile)tile_id;
	}

	auto blocks = std::count_if(tileMap.begin(), tileMap.end(), [](auto p) { return p.second == Tile::Block; });

	std::cout << "Part 1: " << blocks << std::endl;

}
