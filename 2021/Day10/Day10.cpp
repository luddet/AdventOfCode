#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>



int main()
{
	std::ifstream ifs("input.txt");

	const std::unordered_map<char,char> braces { {'(', ')'}, {'{', '}'}, {'[', ']'}, {'<', '>'} };
	const std::unordered_map<char, int> scores{ {')', 3}, {']', 57}, {'}', 1197}, {'>', 25137} };

	//[[maybe_unused]]
	//std::vector<std::string> invalidLines;
	int score{ 0 };

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
					//invalidLines.push_back(line);
					score += scores.at(c);
					break;
				}
			}
		}
	}


	std::cout << "Day10 Part 1: " << score << '\n';
	std::cout << "Day10 Part 2: " << '\n';
}
