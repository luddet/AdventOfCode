// https://adventofcode.com/2019/day/13
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
#include <array>
#include <vector>

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

bool operator==(const coord_t& lhs, const coord_t& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
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

// indexed by Tile enum
std::array<char, (size_t)Tile::TiletypesCount> TileChars{ ' ', '|', '#', '-', 'O' };

using tile_map_t = std::map<coord_t, Tile>;

void draw(const tile_map_t& tileMap, int score)
{
	std::vector<std::vector<char>> grid;

	int maxRows = INT_MIN;
	int maxCols = INT_MIN;

	std::for_each(tileMap.begin(), tileMap.end(), [&maxRows, &maxCols](auto p)
				  {
					  maxRows = std::max(maxRows, p.first.y);
					  maxCols = std::max(maxCols, p.first.x);
				  });
	maxRows++;
	maxCols++;
	for (int row = 0; row < maxRows; ++row)
	{
		grid.push_back(std::vector<char>(maxCols));
	}

	std::for_each(tileMap.begin(), tileMap.end(), [&grid](auto p) { grid[p.first.y][p.first.x] = TileChars[(int)p.second];  });

	for (int row = 0; row < grid.size(); ++row)
	{
		for (int col = 0; col < grid[row].size(); ++col)
		{
			std::cout << grid[row][col];
		}
		std::cout << std::endl;
	}
	std::cout << "Score: " << score << std::endl << std::endl;
}

int main()
{
	std::fstream fs("input.txt");
	auto mem = readMemory(fs);
	// Part 1
	ptrdiff_t blocks;
	{
		Computer c(mem);

		while (!c.isDone())
			c.run();

		tile_map_t tileMap;
		Computer::mem_item_t x, y, tile_id;

		while (c.getOutput(x))
		{
			c.getOutput(y);

			coord_t coord{ (int)x, (int)y };
			c.getOutput(tile_id);
			auto it = tileMap.find(coord);
			if (it == tileMap.end())
				tileMap.insert(std::make_pair(coord, (Tile)tile_id));
			else
				it->second = (Tile)tile_id;
		}

		blocks = std::count_if(tileMap.begin(), tileMap.end(), [](auto p) { return p.second == Tile::Block; });
	}

	mem[0] = 2;
	// Part 2
	Computer::mem_item_t score(0);
	{
		Computer c(mem);

		std::map<coord_t, Tile> tileMap;
		Computer::mem_item_t x, y, tile_id;
		Computer::mem_item_t lastBallX(0), lastPaddleX(0);
		bool gameWon = false;
		while (!c.isDone() && !gameWon)
		{
			if (c.getState() == Computer::ComputerState::WAITING_FOR_INPUT)
			{
				int input = lastBallX > lastPaddleX ? 1 : lastBallX < lastPaddleX ? -1 : 0;
				c.addInput(input);
			}

			c.run();

			while (c.getOutput(x))
			{
				c.getOutput(y);

				coord_t coord{ (int)x, (int)y };
				if (coord == coord_t{ -1, 0 }) // read score
				{
					c.getOutput(score);
				}
				else
				{
					c.getOutput(tile_id);
					auto it = tileMap.find(coord);
					if (it == tileMap.end())
						tileMap.insert(std::make_pair(coord, (Tile)tile_id));
					else
						it->second = (Tile)tile_id;

					if (tile_id == (int)Tile::Ball)
						lastBallX = x;
					else if (tile_id == (int)Tile::HorizontalPaddle)
						lastPaddleX = x;
				}
			}
			auto blocks = std::count_if(tileMap.begin(), tileMap.end(), [](auto p) { return p.second == Tile::Block; });
			gameWon = (blocks == 0);
		}
		draw(tileMap, (int)score);
	}
	std::cout << "Part 1: " << blocks << std::endl;
	std::cout << "Part 2: " << score << std::endl;

}
