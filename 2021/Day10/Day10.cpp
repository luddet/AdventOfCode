#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>


int main()
{
	std::ifstream ifs("input.txt");

	const std::unordered_map<char,char> braces { {'(', ')'}, {'{', '}'}, {'[', ']'}, {'<', '>'} };
	const std::unordered_map<char, int> invalidScoresLookup{ {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };
	const std::unordered_map<char, int> completionScoreLookup{ {'(', 1}, {'[', 2}, {'{', 3}, {'<', 4} };

	int invalidScore{ 0 };
	std::vector<size_t> completionScores;
	{
		std::string line;
		while (std::getline(ifs, line))
		{
			std::stack<char> chunks;

			for (auto c : line)
			{
				// Opening brace?
				if (std::any_of(braces.begin(), braces.end(), [c](auto b) { return c == b.first; }))
					chunks.push(c);
				else if (braces.at(chunks.top()) == c) // matching close
					chunks.pop();
				else // invalid chunk
				{
					invalidScore += invalidScoresLookup.at(c);
					chunks = std::stack<char>();
					break;
				}
			}
			
			if (chunks.empty())
				continue;

			size_t score{ 0 };
			while (!chunks.empty())
			{
				score *= 5;
				score += completionScoreLookup.at(chunks.top());
				chunks.pop();
			}
			completionScores.push_back(score);
		}
	}

	auto median = completionScores.begin() + completionScores.size() / 2;
	std::ranges::nth_element(completionScores, median);


	std::cout << "Day10 Part 1: " << invalidScore << '\n';
	std::cout << "Day10 Part 2: " << *median << '\n';
}
