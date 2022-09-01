#include <algorithm>
#include <concepts>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <stdio.h>
#include <string>
#include <vector>

#include "../Utilities/utilities.h"

struct Point
{
	int x{};
	int y{};
};

struct Line
{
	Point start{};
	Point end{};
};

std::istream& operator>>(std::istream& is, Line& line)
{
	std::string str;
	std::getline(is, str);
	auto nRead = sscanf_s(str.c_str(), "%d,%d -> %d,%d", &line.start.x, &line.start.y, &line.end.x, &line.end.y);
	if (nRead != 4)
		is.setstate(std::ios_base::failbit);

	return is;
}

class Grid
{
public:
	Grid(size_t width, size_t height)
		: m_width(width)
		, m_height(height)
		, m_grid(width * height)
	{}

	void plot(const Line& line)
	{
		if (line.start.x == line.end.x) // Vertical
		{
			auto [start, end] = line.start.y <= line.end.y ? std::make_pair(line.start.y, line.end.y) : std::make_pair(line.end.y, line.start.y);
			size_t stride = m_width;
			size_t offset = m_width * start + line.start.x;
			size_t endOffset = offset + (end - start) * stride;
			for (auto index = offset; index <= endOffset; index += stride)
				++m_grid[index];
		}
		else if (line.start.y == line.end.y) // Horizontal
		{
			auto [start, end] = line.start.x <= line.end.x ? std::make_pair(line.start.x, line.end.x) : std::make_pair(line.end.x, line.start.x);
			size_t stride = 1;
			size_t offset = m_width * line.start.y + start;
			size_t endOffset = offset + (end - start) * stride;
			for (auto index = offset; index <= endOffset; index += stride)
				++m_grid[index];
		}
		else
		{
			throw 1;
		}
	}
	
	int count_if(std::predicate<uint16_t> auto pred)
	{
		return std::count_if(begin(m_grid), end(m_grid), pred);
	}
		
private:
	size_t m_width;
	size_t m_height;
	std::vector<uint16_t> m_grid;

public:
	friend std::ostream& operator<<(std::ostream& os, const Grid& grid)
	{
		for (size_t i = 0; i < grid.m_grid.size(); ++i)
		{
			auto v = grid.m_grid[i];
			os << (v > 0 ? std::to_string(v) : ".");
			if ((i + 1) % grid.m_width == 0)
				os << '\n';
		}
		return os;
	}
};

int main()
{
	std::ifstream ifs("input.txt");
	auto lines = readItems<Line>(ifs);

	std::vector<Line> horzAndVertLines;
	std::copy_if(begin(lines), end(lines), std::back_inserter(horzAndVertLines), [](auto& line)
	{
		return line.start.x == line.end.x || line.start.y == line.end.y;
	});

	const auto [xMax, yMax] = std::accumulate(begin(lines), end(lines), Point{0,0}, [](auto max, auto& line) -> Point
	{
		return { std::max({max.x, line.start.x, line.end.x}), std::max({max.y, line.start.y, line.end.y}) };
	});

	Grid grid(xMax+1, yMax+1);

	for (auto& line : horzAndVertLines)
		grid.plot(line);

	auto part1 = grid.count_if([](auto v) {return v > 1; });
	std::cout << "Day05 Part1: " << part1 << '\n';
	//std::cout << grid;

}
