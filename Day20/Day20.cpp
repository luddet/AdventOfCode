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

constexpr size_t DIM{ 10 };
constexpr size_t DATA_NUMROWS{ DIM }, DATA_NUMCOLS{ DIM };
constexpr size_t DATA_SIZE{ DATA_NUMROWS * DATA_NUMCOLS };



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

	Tile() {}
	Tile(size_t index, const std::array<char, DATA_SIZE>& data);

	MatchResult match(const Tile& other) const;
	size_t		getIndex() const { return m_index; }
	void		flipH();
	void		flipV();
	void		rotate(size_t n = 1);
	void		matchupEdges(size_t fixedEdge, size_t thisEdge, bool reversed);

	void		printRow(std::ostream& os, size_t row, bool excludeEdges = false) const;

private:
	void		updateEdges();

	size_t m_index { SIZE_MAX };
	std::array<char, DATA_SIZE> m_data{};
	std::array < std::array<char, DIM>, 4> m_edges{};

	friend std::ostream& operator<<(std::ostream& os, const Tile& tile);
};

Tile::Tile(size_t index, const std::array<char, DATA_SIZE>& data)
	: m_index(index), m_data(data), m_edges{}
{
	updateEdges();
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

		if (!reversed)
			if (thisEdge == Tile::MatchResult::TOP || thisEdge == Tile::MatchResult::BOTTOM)
				flipH();
			else
				flipV();
	}
	else if (edgeDiff == 3 || edgeDiff == -1)
	{
		flipH();
		flipV();
		rotate();
		if (reversed)
			if (thisEdge == Tile::MatchResult::TOP || thisEdge == Tile::MatchResult::BOTTOM)
				flipV();
			else
				flipH();
	}

}

void Tile::updateEdges()
{
	std::copy(m_data.begin(), m_data.begin() + DIM, m_edges[0].begin());
	std::copy(m_data.begin() + (DIM - 1) * DIM, m_data.end(), m_edges[2].begin());

	for (size_t i = 0; i < DIM; ++i)
	{
		m_edges[1][i] = m_data[i * DIM + DIM - 1];
		m_edges[3][i] = m_data[i * DIM];
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
	for (size_t row = 0; row < DATA_NUMROWS; ++row)
		for (size_t col = 0; col < DATA_NUMCOLS / 2; ++col)
			std::swap(m_data[row * DATA_NUMCOLS + col], m_data[row * DATA_NUMCOLS + DATA_NUMCOLS - col - 1]);
	updateEdges();
}

void Tile::flipV()
{
	for (size_t row = 0; row < DATA_NUMROWS / 2; ++row)
		std::swap_ranges(&m_data[row * DATA_NUMCOLS], &m_data[row * DATA_NUMCOLS] + DATA_NUMCOLS, &m_data[(DATA_NUMROWS - 1) * DATA_NUMCOLS - row * DATA_NUMCOLS]);
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
		size_t sourceStartIndex = (DATA_NUMROWS - 1) * DATA_NUMCOLS;
		size_t currentSourceIndex{ sourceStartIndex };
		for (size_t i = 0; i < m_data.size(); ++i)
		{
			m_data[i] = oldData[currentSourceIndex];
			if (i % DATA_NUMCOLS == DATA_NUMCOLS - 1)
				currentSourceIndex = ++sourceStartIndex;
			else
				currentSourceIndex -= DATA_NUMCOLS;
		}
	}
	updateEdges();
}

void Tile::printRow(std::ostream& os, size_t row, bool excludeEdges) const
{
	size_t edge{ excludeEdges ? 1ull : 0ull };
	for (size_t i = row * DATA_NUMCOLS + edge; i < (row + 1) * DATA_NUMCOLS - edge; ++i)
		os << m_data[i];
}

std::ostream& operator<<(std::ostream& os, const Tile& tile)
{
	os << "tile " << tile.m_index << ":" << std::endl;
	for (size_t i = 0; i < tile.m_data.size(); ++i)
	{
		os << tile.m_data[i];
		if (i % DATA_NUMCOLS == DATA_NUMCOLS - 1)
			os << std::endl;
	}
	os << "edges:" << std::endl;
	for (auto& edge : tile.m_edges)
	{
		for (auto c : edge)
			os << c;
		os << std::endl;
	}
	return os;
}

std::map<size_t, Tile> parseTiles(std::istream& is)
{
	std::map<size_t, Tile> tiles;
	std::string line;
	
	size_t tileIndex{ 0 };
	std::array<char, DATA_SIZE> data{};
	size_t bitIndex{ 0 };
	while (std::getline(is, line))
	{
		if (line.empty())
		{
			tiles[tileIndex] = { tileIndex, data };
			continue;
		}

		std::string indexPrefix = "Tile ";
		if (line.find(indexPrefix) == 0)
		{
			tileIndex = std::stoull(line.substr(indexPrefix.length(), line.find_last_of(':') - indexPrefix.length()));
			data = {};
			bitIndex = 0;
			continue;
		}

		for (auto ch : line)
			data[bitIndex++] = ch;
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

void printImage(std::ostream& os, const std::vector<std::vector<Tile>>& img, bool excludeEdges = false)
{
	const size_t edge{ excludeEdges ? 1ull : 0ull };
	for (auto& imgRow : img)
	{
		for (size_t tileRowIndex = edge; tileRowIndex < DATA_NUMROWS - edge; ++tileRowIndex)
		{
			for (auto& tile : imgRow)
			{
				tile.printRow(os, tileRowIndex, excludeEdges);
				if (!excludeEdges)
					os << ' ';
			}
			os << std::endl;
		}
		if (!excludeEdges)
			os << std::endl;
	}
}

std::vector<std::string> createImage(const std::vector<std::vector<Tile>>& img)
{

}


int main()
{

	std::istringstream is(EXAMPLE);
	//std::ifstream is("input.txt");

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

	//std::set<size_t> placedTiles;
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

	printImage(std::cout, img, true);

	uint64_t part1{ 1 };
	for (auto i : corners)
		part1 *= tiles[i].getIndex();

	std::cout << "Day20 Part 1: " << part1 << std::endl;

}
