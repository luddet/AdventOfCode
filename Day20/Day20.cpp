#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <bitset>
#include <array>
#include <algorithm>
#include <set>
#include <map>
#include <regex>

#pragma warning(disable: 26812)

const char* EXAMPLE =
"Tile 2311:\n"
"..##.#..#.\n"
"##..#.....\n"
"#...##..#.\n"
"####.#...#\n"
"##.##.###.\n"
"##...#.###\n"
".#.#.#..##\n"
"..#....#..\n"
"###...#.#.\n"
"..###..###\n"
"\n"
"Tile 1951:\n"
"#.##...##.\n"
"#.####...#\n"
".....#..##\n"
"#...######\n"
".##.#....#\n"
".###.#####\n"
"###.##.##.\n"
".###....#.\n"
"..#.#..#.#\n"
"#...##.#..\n"
"\n"
"Tile 1171:\n"
"####...##.\n"
"#..##.#..#\n"
"##.#..#.#.\n"
".###.####.\n"
"..###.####\n"
".##....##.\n"
".#...####.\n"
"#.##.####.\n"
"####..#...\n"
".....##...\n"
"\n"
"Tile 1427:\n"
"###.##.#..\n"
".#..#.##..\n"
".#.##.#..#\n"
"#.#.#.##.#\n"
"....#...##\n"
"...##..##.\n"
"...#.#####\n"
".#.####.#.\n"
"..#..###.#\n"
"..##.#..#.\n"
"\n"
"Tile 1489:\n"
"##.#.#....\n"
"..##...#..\n"
".##..##...\n"
"..#...#...\n"
"#####...#.\n"
"#..#.#.#.#\n"
"...#.#.#..\n"
"##.#...##.\n"
"..##.##.##\n"
"###.##.#..\n"
"\n"
"Tile 2473:\n"
"#....####.\n"
"#..#.##...\n"
"#.##..#...\n"
"######.#.#\n"
".#...#.#.#\n"
".#########\n"
".###.#..#.\n"
"########.#\n"
"##...##.#.\n"
"..###.#.#.\n"
"\n"
"Tile 2971:\n"
"..#.#....#\n"
"#...###...\n"
"#.#.###...\n"
"##.##..#..\n"
".#####..##\n"
".#..####.#\n"
"#..#.#..#.\n"
"..####.###\n"
"..#.#.###.\n"
"...#.#.#.#\n"
"\n"
"Tile 2729:\n"
"...#.#.#.#\n"
"####.#....\n"
"..#.#.....\n"
"....#..#.#\n"
".##..##.#.\n"
".#.####...\n"
"####.#.#..\n"
"##.####...\n"
"##..#.##..\n"
"#.##...##.\n"
"\n"
"Tile 3079:\n"
"#.#.#####.\n"
".#..######\n"
"..#.......\n"
"######....\n"
"####.#..#.\n"
".#...#.##.\n"
"#.#####.##\n"
"..#.###...\n"
"..#.......\n"
"..#.###...\n"
"\n";


const std::vector<std::string> MONSTER
{
"                  # ",
"#    ##    ##    ###",
" #  #  #  #  #  #   "
};

class Tile
{
public:
	struct MatchResult
	{
		enum Edge : size_t
		{
			TOP = 0,
			RIGHT = 1,
			BOTTOM = 2,
			LEFT = 3
		};

		bool matched;
		size_t thisEdge;
		size_t otherEdge;
		bool reversed;
		operator bool() const { return matched; }
	};

	Tile():m_rows(0), m_cols(0) {}
	Tile(size_t index, const std::string& data, size_t rows, size_t cols);

	char& operator()(size_t row, size_t col);
	uint64_t	count(char c) const;
	MatchResult match(const Tile& other) const;
	size_t		getIndex() const { return m_index; }
	size_t		getNumCols() const { return m_cols; }
	size_t		getNumRows() const { return m_rows; }
	void		flipH();
	void		flipV();
	void		rotate(size_t n = 1);
	void		matchupEdges(size_t fixedEdge, size_t thisEdge, bool reversed);

	void		printRow(std::ostream& os, size_t row, bool includeEdges) const;

	static Tile merge(const std::vector<std::vector<Tile>>& tiles, bool includeEdges);

private:
	void		updateEdges();

	size_t m_index { SIZE_MAX };
	size_t m_rows;
	size_t m_cols;
	std::string m_data{};
	std::array < std::string, 4> m_edges{ };

	friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
};

Tile::Tile(size_t index, const std::string& data, size_t rows, size_t cols)
	: m_index(index), m_data(data), m_rows(rows), m_cols(cols), 
	m_edges{ std::string(cols, ' '), std::string(rows, ' '),std::string(cols, ' '), std::string(rows, ' ') }
{
	updateEdges();
}

char& Tile::operator()(size_t row, size_t col)
{
	return m_data[row * m_cols + col];
}

uint64_t Tile::count(char c) const
{
	return std::count(m_data.begin(), m_data.end(), c);
}

void Tile::matchupEdges(size_t fixedEdge, size_t thisEdge, bool reversed)
{
	auto edgeDiff = (thisEdge - fixedEdge) % 4;

	if ( (edgeDiff == 2 || edgeDiff == -2) && reversed)
	{
		if (thisEdge == Tile::MatchResult::TOP || thisEdge == Tile::MatchResult::BOTTOM)
			flipH();
		else
			flipV();
	}
	else if (edgeDiff == 1 || edgeDiff == -3)
	{
		rotate();
		if (reversed && (thisEdge == Tile::MatchResult::TOP || thisEdge == Tile::MatchResult::BOTTOM))
			flipV();
		else if(!reversed && (thisEdge == Tile::MatchResult::LEFT || thisEdge == Tile::MatchResult::RIGHT))
			flipH();
	}
	else if (edgeDiff == 0)
	{
		flipH();
		flipV();

		if (!reversed && (thisEdge == Tile::MatchResult::TOP || thisEdge == Tile::MatchResult::BOTTOM))
			flipH();
		else if (!reversed && (thisEdge == Tile::MatchResult::LEFT || thisEdge == Tile::MatchResult::RIGHT))
			flipV();
	}
	else if (edgeDiff == 3 || edgeDiff == -1)
	{
		flipH();
		flipV();
		rotate();
		if (!reversed && (thisEdge == Tile::MatchResult::TOP || thisEdge == Tile::MatchResult::BOTTOM))
			flipV();
		else if (reversed && (thisEdge == Tile::MatchResult::LEFT || thisEdge == Tile::MatchResult::RIGHT))
			flipH();
	}

}

void Tile::updateEdges()
{
	std::copy(m_data.begin(), m_data.begin() + m_cols, m_edges[Tile::MatchResult::TOP].begin());
	std::copy(m_data.begin() + (m_cols - 1) * m_cols, m_data.end(), m_edges[Tile::MatchResult::BOTTOM].begin());

	for (size_t i = 0; i < m_cols; ++i)
	{
		m_edges[Tile::MatchResult::RIGHT][i] = m_data[(i+1) * m_cols - 1];
		m_edges[Tile::MatchResult::LEFT][i] = m_data[i * m_cols];
	}
}

Tile::MatchResult Tile::match(const Tile& other) const
{
	for (size_t i = 0; i < m_edges.size(); ++i)
		for (size_t j = 0; j < other.m_edges.size(); ++j)
			if (std::equal(m_edges[i].begin(), m_edges[i].end(), other.m_edges[j].begin()))
				return { true, i, j, false };
			else if (std::equal(m_edges[i].begin(), m_edges[i].end(), other.m_edges[j].rbegin()))
				return { true, i, j, true };
	return { false };
}

void Tile::flipH()
{
	for (size_t row = 0; row < m_rows; ++row)
		for (size_t col = 0; col < m_cols / 2; ++col)
			std::swap(m_data[row * m_cols + col], m_data[(row+1) * m_cols - col - 1]);
	updateEdges();
}

void Tile::flipV()
{
	for (size_t row = 0; row < m_rows / 2; ++row)
		std::swap_ranges(&m_data[row * m_cols], &m_data[row * m_cols] + m_cols, &m_data[(m_rows - 1) * m_cols - row * m_cols]);
	updateEdges();
}

void Tile::rotate(size_t n)
{
	n %= 4;
	if (n == 0)
		return;

	for (; n > 0; --n)
	{
		auto oldData{ m_data };
		size_t sourceStartIndex = (m_rows - 1) * m_cols;
		size_t currentSourceIndex{ sourceStartIndex };
		for (size_t i = 0; i < m_data.size(); ++i)
		{
			m_data[i] = oldData[currentSourceIndex];
			if (i % m_cols == m_cols - 1)
				currentSourceIndex = ++sourceStartIndex;
			else
				currentSourceIndex -= m_cols;
		}
	}
	updateEdges();
}

void Tile::printRow(std::ostream& os, size_t row, bool includeEdges) const
{
	size_t edge{ includeEdges ? 0ull : 1ull };
	os << m_data.substr(row * m_cols + edge, m_cols - 2*edge);
}

Tile Tile::merge(const std::vector<std::vector<Tile>>& tiles, bool includeEdges)
{
	std::ostringstream ss;
	const size_t edge{ includeEdges ? 0ull : 1ull };
	size_t numCols{ tiles[0].size() * (tiles[0][0].getNumCols() - 2*edge)};
	size_t numRows{ tiles.size() * (tiles[0][0].getNumRows() - 2*edge)};

	for (auto& tilesRow : tiles)
	{
		for (size_t tileRowIndex = edge; tileRowIndex < tiles[0][0].getNumRows() - edge; ++tileRowIndex)
		{
			for (auto& tile : tilesRow)
			{
				tile.printRow(ss, tileRowIndex, includeEdges);
			}
		}
	}

	return {0ull, ss.str(), numRows, numCols};
}

std::ostream& operator<<(std::ostream& os, const Tile& tile)
{
	os << "tile " << tile.m_index << ":" << std::endl;
	for (size_t i = 0; i < tile.m_data.size(); ++i)
	{
		os << tile.m_data[i];
		if (i % tile.m_cols == tile.m_cols - 1)
			os << std::endl;
	}
	return os;
}

std::map<size_t, Tile> parseTiles(std::istream& is)
{
	std::map<size_t, Tile> tiles;
	std::string line;
	
	size_t tileIndex{ 0 };
	std::ostringstream data{};
	size_t numRows{ 0 }, numCols{ 0 };
	while (std::getline(is, line))
	{
		if (line.empty())
		{
			tiles[tileIndex] = Tile{ tileIndex, data.str(), numRows, numCols };
			continue;
		}

		std::string indexPrefix = "Tile ";
		if (line.find(indexPrefix) == 0)
		{
			tileIndex = std::stoull(line.substr(indexPrefix.length(), line.find_last_of(':') - indexPrefix.length()));
			data = {};
			numRows = 0;
			numCols = 0;
			data = {};
			continue;
		}
		
		data << line;
		numCols = line.length();
		++numRows;
	}
	return tiles;
}


std::map<size_t, std::vector<size_t>> findMatchIndices(const std::map<size_t, Tile>& tiles)
{
	std::map<size_t, std::vector<size_t>> result{};

	for (auto i = tiles.begin(); i != std::prev(tiles.end()); ++i)
	{
		for (auto j = std::next(i); j != tiles.end(); ++j)
		{
			auto& [tile1Index, tile1] = *i;
			auto& [tile2Index, tile2] = *j;

			if (tile1.match(tile2))
			{
				if (result.find(tile1Index) == result.end())
					result[tile1Index] = {};
				if (result.find(tile2Index) == result.end())
					result[tile2Index] = {};

				result[tile1Index].push_back(tile2Index);
				result[tile2Index].push_back(tile1Index);
			}
		}
	}
	return result;
}

bool markMonsters(const std::vector<std::string>& monsterPattern, Tile& tile)
{
	bool foundMonster{ false };
	size_t patternLength{ monsterPattern[0].length() };
	for (size_t row = 0; row < tile.getNumRows() - monsterPattern.size(); ++row)
	{
		for (size_t col = 0; col < tile.getNumCols() - patternLength; ++col)
		{
			bool allMatch{ true };
			for (size_t m = 0; m < monsterPattern.size() && allMatch; ++m)
			{
				for (size_t n = 0; n < patternLength && allMatch; ++n)
				{
					if (monsterPattern[m][n] == '#' && tile(row + m, col + n) != '#')
						allMatch = false;
				}
			}

			if (allMatch)
			{
				for (size_t m = 0; m < monsterPattern.size(); ++m)
				{
					for (size_t n = 0; n < monsterPattern[m].length(); ++n)
					{
						if (monsterPattern[m][n] == '#')
							tile(row + m, col + n) = 'O';
					}
				}
			}
			foundMonster |= allMatch;
		}
	}
	return foundMonster;
}

void printImage(std::ostream& os, const std::vector<std::vector<Tile>>& img)
{
	for (auto& tileRow : img)
	{
		for (size_t rowIndex = 0; rowIndex < tileRow[0].getNumRows(); ++rowIndex)
		{
			for (auto& tile : tileRow)
			{
				tile.printRow(os, rowIndex, true);
				os << " ";
			}
			os << std::endl;
		}
		os << std::endl;
	}
}

int main()
{

	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	auto tiles = parseTiles(is);
	auto matchIndices = findMatchIndices(tiles);

	std::vector<size_t> corners{};
	for (size_t i = 0; i < matchIndices.size(); ++i)
	{
		if (matchIndices[i].size() == 2)
			corners.push_back(i);
	}

	const auto side = size_t(std::sqrt(tiles.size()));

	std::vector<std::vector<Tile>> img{side, std::vector<Tile>{side} };

	size_t currentTileIndex = corners[0];

	// align corner tile to top left corner
	auto match1 = tiles[currentTileIndex].match(tiles[matchIndices[currentTileIndex][0]]);
	auto match2 = tiles[currentTileIndex].match(tiles[matchIndices[currentTileIndex][1]]);
	if (match1.thisEdge == Tile::MatchResult::LEFT || match2.thisEdge == Tile::MatchResult::LEFT)
		tiles[currentTileIndex].flipH();
	if (match1.thisEdge == Tile::MatchResult::TOP || match2.thisEdge == Tile::MatchResult::TOP)
		tiles[currentTileIndex].flipV();
	img[0][0] = tiles[currentTileIndex];

	for (size_t row = 0; row < img.size(); ++row)
	{
		for (size_t col = 0; col < img[row].size(); ++col)
		{
			if (row == 0 && col == 0) ++col; // Already assigned

			auto previousTileIndex = (col > 0) ? img[row][col - 1].getIndex() : img[row - 1][col].getIndex();

			Tile::MatchResult match{};
			size_t nextTileIndex = *std::find_if(matchIndices[previousTileIndex].begin(), matchIndices[previousTileIndex].end(),
				[&](const size_t i)
				{
					match = tiles[previousTileIndex].match(tiles[i]);
					if ((col > 0))
						return match.thisEdge == Tile::MatchResult::RIGHT;
					else
						return match.thisEdge == Tile::MatchResult::BOTTOM;
				});

			// Flip next
			tiles[nextTileIndex].matchupEdges(match.thisEdge, match.otherEdge, match.reversed);
			img[row][col] = tiles[nextTileIndex];
		}
	}

	//printImage(std::cout, img);

	auto merged = Tile::merge(img, false);
	//std::cout << merged << std::endl;

	bool marked = markMonsters(MONSTER, merged);
	while (!marked)
	{
		merged.rotate();
		if (markMonsters(MONSTER, merged))
			break;

		merged.flipH();
		if (markMonsters(MONSTER, merged))
			break;

		merged.flipH();
		merged.flipV();
		if (markMonsters(MONSTER, merged))
			break;

		merged.flipV();
	}

	//std::cout << "----------" << std::endl;
	//std::cout << merged << std::endl;

	uint64_t part1{ 1 };
	for (auto i : corners)
		part1 *= tiles[i].getIndex();

	uint64_t part2 = merged.count('#');

	std::cout << "Day20 Part 1: " << part1 << std::endl;
	std::cout << "Day20 Part 2: " << part2 << std::endl;

}
