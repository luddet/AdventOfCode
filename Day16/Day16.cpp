#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <algorithm>

const char* EXAMPLE =
"class: 1-3 or 5-7\n"
"row: 6-11 or 33-44\n"
"seat: 13-40 or 45-50\n"
"\n"
"your ticket:\n"
"7,1,14\n"
"\n"
"nearby tickets:\n"
"7,3,47\n"
"40,4,50\n"
"55,2,20\n"
"38,6,12";

struct Range
{
	bool contains(uint64_t val) const { return val >= start && val <= end; }
	uint64_t start;
	uint64_t end;
};

struct Rule
{
	bool operator()(uint64_t val) const
	{
		return ranges[0].contains(val) || ranges[1].contains(val);
	}

	std::string fieldName;
	std::array<Range, 2> ranges;
};

Rule parseRule(const std::string& line)
{
	std::regex r(R"(^([^:]+): (\d+)-(\d+) or (\d+)-(\d+))");
	std::smatch match;
	if (!std::regex_match(line, match, r))
		throw std::exception("Failed to parse rule.");

	size_t i(1);
	std::string fieldName(match[i++]);
	Range range1{ std::stoull(match[i++]), std::stoull(match[i++]) };
	Range range2{ std::stoull(match[i++]), std::stoull(match[i++]) };
	return { fieldName, {range1, range2} };
}

int main()
{
	using std::getline;

	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	std::vector<Rule> rules;
	std::vector<uint64_t> myTicket;
	std::vector<std::vector<uint64_t>> otherTickets;

	// Parse rules
	std::string line;
	while (getline(is, line) && !std::empty(line))
		rules.push_back(parseRule(line));

	getline(is, line);
	getline(is, line);

	{ // Parse my ticket
		std::string numStr;
		std::istringstream ticketStream{ line };
		while (getline(ticketStream, numStr, ','))
			myTicket.push_back(std::stoull(numStr));
	}

	getline(is, line);
	getline(is, line);

	// Parse other tickets
	while (getline(is, line) && !std::empty(line))
	{
		std::string numStr;
		std::vector<uint64_t> currentTicket;
		std::istringstream ticketStream{ line };
		while (getline(ticketStream, numStr, ','))
			currentTicket.push_back(std::stoull(numStr));
		otherTickets.push_back(currentTicket);
	}

	int64_t invalidSum(0);
	for (auto& ticket : otherTickets)
	{
		for (auto val : ticket)
		{
			if (std::none_of(std::begin(rules), std::end(rules),
				[val](const auto& rule)
				{ 
					return rule(val); 
				}))
			{
				invalidSum += val;
			}
		}
	}
	std::cout << "Day16 Part 1: " << invalidSum << std::endl;
}
