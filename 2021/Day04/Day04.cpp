#include <concepts>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <numeric>
#include <ranges>
#include <array>
#include <bitset>
#include <optional>
#include <string>


#include "../../Utilities/Utilities/utilities.h"

using namespace std::string_literals;
namespace ranges = std::ranges;

template<int SIZE = 5>
class Board
{
	std::array<int, SIZE * SIZE> m_numbers{0};
	std::bitset<SIZE * SIZE> m_marks {0};

	static constexpr std::array<unsigned long, 10> COMPLETE_PATTERNS
	{
		0b1111100000000000000000000,
		0b0000011111000000000000000,
		0b0000000000111110000000000,
		0b0000000000000001111100000,
		0b0000000000000000000011111,
		0b1000010000100001000010000,
		0b0100001000010000100001000,
		0b0010000100001000010000100,
		0b0001000010000100001000010,
		0b0000100001000010000100001
	};


public:
	bool mark(int num)
	{
		for (size_t i = 0; i < m_numbers.size(); ++i)
		{
			if (m_numbers[i] == num)
			{
				m_marks.set(i, true);
				return true;
			}
		}
		return false;
	}

	bool isComplete() const
	{
		auto bits{ m_marks.to_ulong() };
		return ranges::any_of(COMPLETE_PATTERNS, [bits](auto pattern) 
		{
			return (pattern & bits) == pattern; 
		});
	}

	auto getUnmarkedSum() const
	{
		int sum{ 0 };
		for (size_t i = 0; i < m_numbers.size(); ++i)
		{
			if (!m_marks.test(i))
				sum += m_numbers[i];
		}
		return sum;
	}

	bool operator==(const Board& other) const
	{
		return m_marks == other.m_marks && m_numbers == other.m_numbers;
	}

	friend std::istream& operator>>(std::istream& is, Board& board)
	{
		for (size_t i = 0; i < board.m_numbers.size(); ++i)
			is >> board.m_numbers[i];
		return is;
	}

	friend std::ostream& operator<<(std::ostream& os, const Board& board)
	{
		for (size_t i = 0; i < board.m_numbers.size(); ++i)
			os << board.m_numbers[i] << (((i + 1) % SIZE) > 0 ? " " : "\n");
		return os;
	}
};

int main()
{
	using std::getline;

	std::ifstream ifs("input.txt");
	std::string line;
	getline(ifs, line);
	auto numbers = readItems<int>(line, ',');
	auto boards = readItems<Board<5>>(ifs);
	
	int part1Answer{}, part2Answer{};

	bool foundWinner{ false };
	size_t numIdx{ 0 };
	size_t boardIndex{ boards.size()-1};

	for (; numIdx < numbers.size(); ++numIdx)
	{
		auto n = numbers[numIdx];
		while (!foundWinner)
		{
			auto& board = boards[boardIndex];
			if (board.mark(n) && board.isComplete())
			{
				foundWinner = true;
				part1Answer = n * board.getUnmarkedSum();
				std::erase(boards, board);
			}

			if (boardIndex == 0)
				break;
			else
				--boardIndex;
		}

		if (foundWinner)
			break;
		else
			boardIndex = boards.size() - 1;
	}

	bool foundLooser{ false };
	for (; !foundLooser && numIdx < numbers.size(); ++numIdx)
	{
		auto n = numbers[numIdx];
		while (!foundLooser)
		{
			auto& board = boards[boardIndex];
			if (board.mark(n) && board.isComplete())
			{
				if (boards.size() > 1)
					std::erase(boards, board);
				else
				{
					part2Answer = n * board.getUnmarkedSum();
					foundLooser = true;
				}
			}

			if (boardIndex == 0)
				break;
			else
				--boardIndex;

		}
		boardIndex = boards.size() - 1;
	}
	
	std::cout << "Day04 Part1: " << part1Answer << '\n';
	std::cout << "Day04 Part2: " << part2Answer << '\n';
}
