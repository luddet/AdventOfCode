#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <set>
#include <regex>
#include <sstream>
#include <exception>
#include <stack>
#include <algorithm>

const std::string EXAMPLE_INPUT("light red bags contain 1 bright white bag, 2 muted yellow bags.\n"
	"dark orange bags contain 3 bright white bags, 4 muted yellow bags.\n"
	"bright white bags contain 1 shiny gold bag.\n"
	"muted yellow bags contain 2 shiny gold bags, 9 faded blue bags.\n"
	"shiny gold bags contain 1 dark olive bag, 2 vibrant plum bags.\n"
	"dark olive bags contain 3 faded blue bags, 4 dotted black bags.\n"
	"vibrant plum bags contain 5 faded blue bags, 6 dotted black bags.\n"
	"faded blue bags contain no other bags.\n"
	"dotted black bags contain no other bags.");

const std::regex LINE_REGEX("^((?:\\w+)(?:\\s\\w+)*)(?=\\sbags contain\\s)|(\\d+\\s\\w+(?:\\s\\w+)*(?=\\sbags?))|(no other bags)");

void parseLine(const std::string& line, std::map<const std::string, std::map<const std::string, uint32_t>>& canContain, std::map<const std::string, std::map<const std::string, uint32_t>>& canBeContainedIn)
{
	std::string container;
	std::map<const std::string, uint32_t> contained;
	uint32_t currentContainedNumber;

	std::string::const_iterator searchStart(line.cbegin());
	std::smatch matches;
	
	while (std::regex_search(searchStart, line.cend(), matches, LINE_REGEX))
	{
		if (matches[1].matched) // container
		{
			container = matches[1];
		}
		else if (matches[2].matched) // contained
		{
			std::string currentContained = matches[2];
			auto spaceIndex = currentContained.find_first_of(' ');
			currentContainedNumber = std::stoi(currentContained.substr(0, spaceIndex));
			currentContained = currentContained.substr(spaceIndex + 1);
			contained[currentContained] = currentContainedNumber;

			auto it = canBeContainedIn.find(currentContained);
			if (it == canBeContainedIn.end())
			{
				canBeContainedIn[currentContained] = {};
				it = canBeContainedIn.find(currentContained);
			}

			(it->second)[container] = currentContainedNumber;

		}
		else if (matches[3].matched) // no content
		{
		}
		else // Error!?
			throw std::exception("Match error");

		searchStart = matches.suffix().first;
	}
	canContain[container] = contained;
}

uint32_t sumBagContents(const std::string& target, const std::map<const std::string, std::map<const std::string, uint32_t>>& canContain)
{
	uint32_t sum(1);

	auto it = canContain.find(target);
	for (auto& m : it->second)
		sum += m.second * sumBagContents(m.first, canContain);
	
	return sum;
}

int main()
{
	using std::string;
	using std::map;
	using std::set;

	map<const string, map<const string, uint32_t >> canContain;
	map<const string, map<const string, uint32_t>> canBeContainedIn;

	std::ifstream ifs("input.txt");
	std::istringstream testInput(EXAMPLE_INPUT);
	std::string line;
	while (std::getline(ifs, line))
		parseLine(line, canContain, canBeContainedIn);

	set<string> possibleContainers;
	std::stack<std::string> targets;
	targets.push("shiny gold");

	while (!targets.empty())
	{
		string currentTarget(targets.top());
		targets.pop();
		possibleContainers.insert(currentTarget);

		for (auto& p : canBeContainedIn[currentTarget])
			targets.push(p.first);
	}

	auto part1BagCount = possibleContainers.size() - 1;
	auto part2BagCount = sumBagContents("shiny gold", canContain) - 1;

	std::cout << "Day07 Part 1: " << part1BagCount << std::endl;
	std::cout << "Day07 Part 2: " << part2BagCount << std::endl;

}
