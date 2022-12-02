#include <exception>
#include <fstream>
#include <iostream>
#include <unordered_map>

enum class Hand: uint8_t { Rock, Paper,	Scissors };
enum class Outcome : uint8_t { Lose, Draw, Win };

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
	auto diff = (int)p1 - (int)p2;
	if (diff == 0) // draw
		return 3 + (int)p2 + 1;
	else if (diff == 1 || diff == -2) // p1 win
		return (int)p2 + 1;
	else if (diff == -1 || diff == 2) // p2 win
		return 6 + (int)p2 + 1;
	else
		throw std::exception("Unhandled scoring case");
}

Hand handFromWantedOutcome(Hand otherHand, Outcome o)
{
	if (o == Outcome::Draw)
		return otherHand;
	else if (o == Outcome::Win)
		return static_cast<Hand>(((int)otherHand + 4) % 3);
	else if (o == Outcome::Lose)
		return static_cast<Hand>(((int)otherHand + 5) % 3);
	else
		throw std::exception("Unhandled outcome case");
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

		wantedOutcome = static_cast<Outcome>(part1Hand);
		auto part2_hand = handFromWantedOutcome(elfHand, wantedOutcome);

		s = score(elfHand, part2_hand);
		part2Score += s;
	}

	std::cout << "Day02 Part 1: " << part1Score << '\n';
	std::cout << "Day02 Part 2: " << part2Score << '\n';
}
