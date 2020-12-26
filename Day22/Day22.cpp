#include <iostream>
#include <sstream>
#include <fstream>
#include <queue>
#include <string>
#include <vector>

const char* EXAMPLE =
"Player 1:\n"
"9\n"
"2\n"
"6\n"
"3\n"
"1\n"
"\n"
"Player 2:\n"
"5\n"
"8\n"
"4\n"
"7\n"
"10\n";

constexpr size_t P1 = 0;
constexpr size_t P2 = 1;

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");
	std::vector<std::queue<uint32_t>> decks;

	std::string line;
	while (std::getline(is, line))
	{
		if (line.find("Player") == 0)
		{
			decks.push_back({});
			continue;
		}
		else if (!line.empty())
			decks.back().push(std::stoul(line));
	}

	// Run game
	while (decks[P1].size() > 0 && decks[P2].size() > 0)
	{
		auto p1 = decks[P1].front();
		auto p2 = decks[P2].front();
		decks[P1].pop();
		decks[P2].pop();

		if (p1 > p2)
		{
			decks[P1].push(p1);
			decks[P1].push(p2);
		}
		else
		{
			decks[P2].push(p2);
			decks[P2].push(p1);
		}
	}

	// score
	auto winner = decks[P1].size() > 0 ? P1 : P2;

	uint64_t score{ 0 };
	size_t multiplier = decks[winner].size();
	while (!decks[winner].empty())
	{
		score += multiplier-- * decks[winner].front();
		decks[winner].pop();
	}

	std::cout << "Day22 Part 1: " << score << std::endl;

}
