#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <deque>

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

const char* EXAMPLE2 =
"Player 1:\n"
"43\n"
"19\n"
"\n"
"Player 2:\n"
"2\n"
"29\n"
"14\n";

constexpr size_t P1 = 0;
constexpr size_t P2 = 1;
using card_t = uint32_t;
using deck_t = std::deque<card_t>;

void printDecks(const std::vector<deck_t>& decks)
{
	for (size_t i = 0; i < decks.size(); ++i)
	{
		std::cout << "Player " << (i + 1) << ": ";
		for (auto c : decks[i])
			std::cout << c << ", ";
		std::cout << std::endl;
	}
}


deck_t runRegularGame(const std::vector<deck_t>& startDecks)
{
	auto decks{ startDecks };
	size_t winner{0};
	while (decks[P1].size() > 0 && decks[P2].size() > 0)
	{
		auto p1 = decks[P1].front();
		auto p2 = decks[P2].front();
		decks[P1].pop_front();
		decks[P2].pop_front();

		winner = (p1 > p2) ? P1 : P2;
		decks[winner].push_back((winner == P1) ? p1 : p2);
		decks[winner].push_back((winner == P1) ? p2 : p1);
	}
	return decks[winner];
}

size_t runRecursiveGame(std::vector<deck_t>& decks, size_t& game)
{
	std::vector<std::pair<deck_t, deck_t>> states;
	size_t round{ 1 };
	auto thisGame{ game };
	while (decks[P1].size() > 0 && decks[P2].size() > 0)
	{
		// check previous states
		for (auto& state : states)
		{
			if (std::equal(state.first.begin(), state.first.end(), decks[P1].begin(), decks[P1].end())
				&& std::equal(state.second.begin(), state.second.end(), decks[P2].begin(), decks[P2].end()))
				return P1;
		}
		states.push_back({ decks[P1], decks[P2] });

		auto p1 = decks[P1].front();
		auto p2 = decks[P2].front();
		decks[P1].pop_front();
		decks[P2].pop_front();

		size_t winner;
		if (decks[P1].size() >= p1 && decks[P2].size() >= p2)
		{
			std::vector<deck_t> subDecks;
			subDecks.push_back({ decks[P1].begin(), decks[P1].begin() + p1 });
			subDecks.push_back({ decks[P2].begin(), decks[P2].begin() + p2 });
			winner = runRecursiveGame(subDecks, ++game);
		}
		else
			winner = (p1 > p2) ? P1 : P2;

		decks[winner].push_back((winner == P1) ? p1 : p2);
		decks[winner].push_back((winner == P1) ? p2 : p1);

		(&round);
		(&thisGame);
		//std::cout << "Round: " << std::setw(4) << round++ << ", Game: " << std::setw(4) << thisGame << ", Winner: " << ((winner == P1) ? "P1" : "P2") << std::endl;
		//printDecks(decks);
	}
	return (decks[P1].size() > 0) ? P1 : P2;
}

size_t runRecursiveGame(std::vector<deck_t>& decks)
{
	size_t game{ 1 };
	return runRecursiveGame(decks, game);
}

uint64_t calculateScore(const deck_t& deck)
{
	uint64_t score{ 0 };
	for (size_t multiplier = deck.size(), i = 0; i < deck.size(); --multiplier, ++i)
		score += multiplier * deck[i];
	return score;
}


int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");
	std::vector<deck_t> decks;

	std::string line;
	while (std::getline(is, line))
	{
		if (line.find("Player") == 0)
		{
			decks.push_back({});
			continue;
		}
		else if (!line.empty())
			decks.back().push_back(std::stoul(line));
	}

	auto winningDeck1 = runRegularGame(decks);
	auto winner2 = runRecursiveGame(decks);

	//printDecks(decks);

	auto score1 = calculateScore(winningDeck1);
	auto score2 = calculateScore(decks[winner2]);

	std::cout << "Day22 Part 1: " << score1 << std::endl;
	std::cout << "Day22 Part 2: " << score2 << std::endl;

}
