#include <fstream>
#include <iostream>
#include <array>
#include <string>
#include <algorithm>
#include <tuple>
#include <vector>
#include <ranges>

auto parseStartingState2(std::istream& s)
{
	using namespace std::views;
	std::vector<std::string> lines;
	for (std::string line; std::getline(s, line) && line.length() > 0; )
		lines.push_back(line);

	std::array<std::vector<unsigned char>, 9> state{};
	for(auto& line : lines | reverse | drop(1))
		for (size_t i = 0; i < state.size(); ++i)
			if (char c{ line[i * 4 + 1] }; c != ' ')
				state[i].push_back(c);
	return state;
}

auto parseOperations(std::istream& s)
{
	using std::get;
	std::string _;
	int n, f, t;
	std::vector<std::tuple<int, int, int>> result;
	while (true)
	{
		s >> _ >> n >> _ >> f >> _ >> t;
		if (s.bad() || s.fail())
			break;
		result.emplace_back(std::make_tuple(n, f-1, t-1));
	}
	return result;
}

int main()
{
	using std::get;
	std::ifstream ifs("input.txt");
	auto stacks1{ parseStartingState2(ifs) }, stacks2{ stacks1 };

	for (const auto [n, f, t] : parseOperations(ifs))
	{
		std::copy_n(rbegin(stacks1[f]), n, std::back_inserter(stacks1[t]));
		stacks1[f].resize(stacks1[f].size() - n);

		std::copy_n(end(stacks2[f]) - n, n, std::back_inserter(stacks2[t]));
		stacks2[f].resize(stacks2[f].size() - n);
	}

	std::string part1, part2;
	auto top = [](auto& stack) { return stack.back();};
	std::transform(cbegin(stacks1), cend(stacks1), std::back_inserter(part1), top);
	std::transform(cbegin(stacks2), cend(stacks2), std::back_inserter(part2), top);

	std::cout << "Day05 Part 1: " << part1 << '\n';
	std::cout << "Day05 Part 2: " << part2 << '\n';
}
