#include <exception>
#include <fstream>
#include <iostream>
#include <unordered_map>

enum class Hand: uint8_t { Rock, Paper,	Scissors };
enum class Outcome : uint8_t { Draw, Win, Lose };

std::istream& operator>>(std::istream& s, Hand& out)
{
	const static std::unordered_map<char, Hand> map({ {'A', Hand::Rock}, {'X', Hand::Rock}, {'B', Hand::Paper}, {'Y', Hand::Paper},{'C', Hand::Scissors},{'Z', Hand::Scissors} });
	unsigned char c;

	s >> c;
	if (!s.good())
		return s;

	out = map.at(c);
	return s;
}

int score(Hand p1, Hand p2)
{
	auto diff = ((int)p2 - (int)p1 + 1 + 3)%3;
	return (int)p2 + 1 + diff*3;
}

Hand handFromWantedOutcome(Hand otherHand, Outcome o)
{
	return static_cast<Hand>(((int)otherHand + 3 + (int)o) % 3);
}

int main()
{
	std::ifstream ifs("input.txt");

	int part1Score{}, part2Score{};

	while (true)
	{
		Hand elfHand, part1Hand;
		Outcome wantedOutcome;

		ifs >> elfHand >> part1Hand;
		if (!ifs.good())
			break;

		auto s = score(elfHand, part1Hand);
		part1Score += s;

		wantedOutcome = static_cast<Outcome>( (int(part1Hand) + 2) % 3);
		auto part2_hand = handFromWantedOutcome(elfHand, wantedOutcome);

		s = score(elfHand, part2_hand);
		part2Score += s;
	}

	std::cout << "Day02 Part 1: " << part1Score << '\n';
	std::cout << "Day02 Part 2: " << part2Score << '\n';
}
