#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>
#include <array>
#include <algorithm>
#include <stack>
#include <set>

const char* EXAMPLE1 =
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

const char* EXAMPLE2 =
"class: 0-1 or 4-19\n"
"row: 0-5 or 8-19\n"
"seat: 0-13 or 16-19\n"
"\n"
"your ticket:\n"
"11,12,13\n"
"\n"
"nearby tickets:\n"
"3,9,18\n"
"15,1,5\n"
"5,14,9";

struct Range
{
	uint64_t start;
	uint64_t end;
};

struct Rule
{
	bool operator()(uint64_t val) const
	{
		return ranges[0].start <= val && val <= ranges[0].end || ranges[1].start <= val && val <= ranges[1].end;
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

using ticket_t = std::vector<uint64_t>;
ticket_t parseTicket(const std::string& line)
{
	ticket_t ticket;
	std::string numStr;
	std::istringstream ticketStream{ line };
	while (getline(ticketStream, numStr, ','))
		ticket.push_back(std::stoull(numStr));
	return ticket;
}

int main()
{
	using std::getline;

	//std::istringstream is(EXAMPLE2);
	std::ifstream is("input.txt");

	

	// Parse rules
	std::vector<Rule> rules;
	std::string line;
	while (getline(is, line) && !std::empty(line))
		rules.push_back(parseRule(line));

	getline(is, line);
	getline(is, line);

	ticket_t myTicket{ parseTicket(line) };

	getline(is, line);
	getline(is, line);

	// Parse other tickets
	std::vector<ticket_t> otherTickets;
	while (getline(is, line) && !std::empty(line))
		otherTickets.push_back(parseTicket(line));

	int64_t invalidSum(0);
	std::vector<ticket_t> validTickets;
	for (auto& ticket : otherTickets)
	{
		bool ticketIsValid(true);
		for (auto val : ticket)
		{
			if (std::none_of(std::begin(rules), std::end(rules),
				[val](const auto& rule)
				{ 
					return rule(val); 
				}))
			{
				invalidSum += val;
				ticketIsValid = false;
				break;
			}
		}
		if (ticketIsValid)
			validTickets.push_back(ticket);
	}

	std::vector<std::pair<Rule, std::vector<size_t>>> potentialIndicies;
	potentialIndicies.resize(rules.size());

	for (size_t rIndex = 0; rIndex < rules.size(); ++rIndex)
	{
		potentialIndicies[rIndex].first = rules[rIndex];
		for (size_t fieldIndex = 0; fieldIndex < validTickets[0].size(); ++fieldIndex)
		{
			bool allValid(true);
			for (size_t i = 0; i < validTickets.size(); ++i)
			{
				if (!rules[rIndex](validTickets[i][fieldIndex]))
				{
					allValid = false;
					break;
				}
			}
			if (allValid)
				potentialIndicies[rIndex].second.push_back(fieldIndex);
		}
	}

	std::vector<Rule> orderedRules;
	orderedRules.resize(rules.size());

	std::sort(potentialIndicies.begin(), potentialIndicies.end(), [](auto& v1, auto& v2) { return v1.second.size() < v2.second.size(); });
	for (size_t i = potentialIndicies.size() - 1; i > 0; --i)
	{
		std::vector<uint64_t> temp;
		std::set_difference(begin(potentialIndicies[i].second), end(potentialIndicies[i].second)
			, begin(potentialIndicies[i - 1].second), end(potentialIndicies[i - 1].second)
			, std::inserter(temp, temp.begin()));
		orderedRules[temp[0]] = potentialIndicies[i].first;
	}
	orderedRules[potentialIndicies[0].second[0]] = potentialIndicies[0].first;

	uint64_t product(1);
	for (size_t i = 0; i < orderedRules.size(); ++i)
	{
		if (orderedRules[i].fieldName.find("departure") == 0)
			product *= myTicket[i];
	}

	std::cout << "Day16 Part 1: " << invalidSum << std::endl;
	std::cout << "Day16 Part 2: " << product << std::endl;
}
