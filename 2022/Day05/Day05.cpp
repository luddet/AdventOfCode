#include <fstream>
#include <iostream>
#include <array>
#include <stack>
#include <string>
#include <regex>
#include <algorithm>
#include <tuple>

auto parseStartingState(std::istream& s)
{
	std::regex re(R"((?:\[(\w)\]|\s\s\s)\s?)");
	std::stack<std::string> lines;
	for (std::string line; std::getline(s, line) && line.length() > 0; )
		lines.push(line);

	lines.pop(); // discard last with only indices

	std::array<std::stack<unsigned char>, 9> state{};
	while (!lines.empty())
	{
		auto& line = lines.top();
		std::sregex_iterator it(begin(line), end(line), re);
		for (auto& stack : state)
		{
			if ((*it)[1].matched)
				stack.push((*it)[1].str().at(0));

			if (++it == std::sregex_iterator())
				break;
		}
		lines.pop();
	}

	return state;
}


auto parseOperations(std::istream& s)
{
	using std::get;
	std::string _;
	std::tuple<int, int, int> in;
	std::vector<decltype(in)> result;
	while (true)
	{
		s >> _ >> get<0>(in) >> _ >> get<1>(in) >> _ >> get<2>(in);
		if (s.bad() || s.fail())
			break;
		result.push_back(in);
	}
	
	return result;
}

int main()
{
	using std::get;
	std::ifstream ifs("input.txt");
	auto stacks1 = parseStartingState(ifs);
	auto stacks2{ stacks1 };

	auto ops = parseOperations(ifs);

	for (const auto& op : ops)
	{
		const int num = get<0>(op);
		const int from{ get<1>(op) - 1 }, to{ get<2>(op) - 1 };

		std::stack<unsigned char> temp;
		for (size_t i{ 0 }; i < num; ++i)
		{
			// part 1
			stacks1[to].push(stacks1[from].top());
			stacks1[from].pop();

			// part 2 take items
			temp.push(stacks2[from].top());
			stacks2[from].pop();
		}

		while (!temp.empty())
		{
			stacks2[to].push(temp.top());
			temp.pop();
		}
	}

	std::string part1, part2;
	auto top = [](auto& stack) { return stack.top();};
	std::transform(cbegin(stacks1), cend(stacks1), std::back_inserter(part1), top);
	std::transform(cbegin(stacks2), cend(stacks2), std::back_inserter(part2), top);

	std::cout << "Day05 Part 1: " << part1 << '\n';
	std::cout << "Day05 Part 2: " << part2 << '\n';
}
