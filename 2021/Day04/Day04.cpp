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


#include "..\Utilities\utilities.h"

using std::string_literals::operator""s;
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


std::optional<int> part1(const ranges::input_range auto& numbers, ranges::random_access_range auto& boards)
{
	for (auto n : numbers)
	{
		for (size_t boardIndex = 0; boardIndex < boards.size(); ++boardIndex)
		{
			if (boards[boardIndex].mark(n) && boards[boardIndex].isComplete())
				return n * boards[boardIndex].getUnmarkedSum();
		}
	}

	return std::optional<int>{};
}

int main()
{
	using std::getline;

	std::ifstream ifs("input.txt");
	std::string line;
	getline(ifs, line);
	auto numbers = readItems<int>(line, ',');
	auto boards = readItems<Board<5>>(ifs);
	
	
	auto part1Answer = part1(numbers, boards);
	std::cout << "Day04 Part1: " << (part1Answer.has_value() ? std::to_string(part1Answer.value()) : "Failed"s) << '\n';

}
