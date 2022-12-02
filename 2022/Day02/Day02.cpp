#include <fstream>
#include <iostream>
#include <array>
#include <vector>
#include <utility>
#include <unordered_map>
#include <tuple>
#include <exception>
#include <sstream>
#include <string_view>
#include <string>

using namespace std::string_literals;

enum class Hand: uint8_t
{
	Rock = 1,
	Paper,
	Scissors
};

enum class Outcome : uint8_t
{
	Lose = 1,
	Draw,
	Win
};

std::istream& operator>>(std::istream& s, Hand& out)
{
	unsigned char c;
	s >> std::skipws;
	s >> c;
	if (!s.good())
		return s;

	switch (c)
	{
		case 'A':
		case 'X':
			out = Hand::Rock;
			break;
		case 'B':
		case 'Y':
			out = Hand::Paper;
			break;
		case 'C':
		case 'Z':
			out = Hand::Scissors;
			break;
		default:
			throw std::exception("Unhandled case.");
	}
	return s;
}

auto score(Hand p1, Hand p2) -> std::tuple<int, int>
{
	std::tuple<int, int> r{};
	switch ((int)p1 - (int)p2)
	{
		// draw
		case 0:
			r = { 3 + (int)p1, 3 + (int)p2 };
			break;
		// p1 win
		case 1:
		case -2:
			r = { 6 + (int)p1, 0 + (int)p2 };
			break;
		// p2 win
		case -1:
		case 2:
			r = { 0 + (int)p1, 6 + (int)p2 };
			break;
		default:
			throw std::exception("Unhandled scoring case.");
	}
	return r;
}

auto hand_from_wanted_outcome(Hand otherHand, Outcome o) -> Hand
{
	switch (o)
	{
		case Outcome::Draw:
			return otherHand;
		case Outcome::Win:
			return static_cast<Hand>( ((int)otherHand +3) % 3 + 1);
		case Outcome::Lose:
			return static_cast<Hand>( ((int)otherHand +4) % 3 + 1);
		default:
			throw std::exception("Unhandled outcome.");
	}
}

int main()
{
	std::ifstream ifs("input.txt");

	int part1_score{}, part2_score{};

	while (ifs.good())
	{
		Hand elf_hand, part1_hand;
		Outcome outcome;

		ifs >> elf_hand >> part1_hand;
		if (!ifs.good())
			break;

		outcome = static_cast<Outcome>(part1_hand);
		auto part2_hand = hand_from_wanted_outcome(elf_hand, outcome);

		auto [_1, s1] = score(elf_hand, part1_hand);
		part1_score += s1;

		auto [_2, s2] = score(elf_hand, part2_hand);
		part2_score += s2;
	}

	std::cout << "Day02 Part 1: " << part1_score << '\n';
	std::cout << "Day02 Part 2: " << part2_score << '\n';
}
