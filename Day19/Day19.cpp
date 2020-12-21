#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

const char* EXAMPLE1 =
"0: 4 1 5\n"
"1: 2 3 | 3 2\n"
"2: 4 4 | 5 5\n"
"3: 4 5 | 5 4\n"
"4: \"a\"\n"
"5: \"b\"\n"
"\n"
"ababbb\n"
"bababa\n"
"abbbab\n"
"aaabbb\n"
"aaaabbb\n";

const char* EXAMPLE2 =
"42: 9 14 | 10 1\n"
"9: 14 27 | 1 26\n"
"10: 23 14 | 28 1\n"
"1: \"a\"\n"
"11: 42 31\n"
"5: 1 14 | 15 1\n"
"19: 14 1 | 14 14\n"
"12: 24 14 | 19 1\n"
"16: 15 1 | 14 14\n"
"31: 14 17 | 1 13\n"
"6: 14 14 | 1 14\n"
"2: 1 24 | 14 4\n"
"0: 8 11\n"
"13: 14 3 | 1 12\n"
"15: 1 | 14\n"
"17: 14 2 | 1 7\n"
"23: 25 1 | 22 14\n"
"28: 16 1\n"
"4: 1 1\n"
"20: 14 14 | 1 15\n"
"3: 5 14 | 16 1\n"
"27: 1 6 | 14 18\n"
"14: \"b\"\n"
"21: 14 1 | 1 14\n"
"25: 1 1 | 1 14\n"
"22: 14 14\n"
"8: 42\n"
"26: 14 22 | 1 20\n"
"18: 15 15\n"
"7: 14 5 | 1 21\n"
"24: 14 1\n"
"\n"
"abbbbbabbbaaaababbaabbbbabababbbabbbbbbabaaaa\n"
"bbabbbbaabaabba\n"
"babbbbaabbbbbabbbbbbaabaaabaaa\n"
"aaabbbbbbaaaabaababaabababbabaaabbababababaaa\n"
"bbbbbbbaaaabbbbaaabbabaaa\n"
"bbbababbbbaaaaaaaabbababaaababaabab\n"
"ababaaaaaabaaab\n"
"ababaaaaabbbaba\n"
"baabbaaaabbaaaababbaababb\n"
"abbbbabbbbaaaababbbbbbaaaababb\n"
"aaaaabbaabaaaaababaa\n"
"aaaabbaaaabbaaa\n"
"aaaabbaabbaaaaaaabbbabbbaaabbaabaaa\n"
"babaaabbbaaabaababbaabababaaab\n"
"aabbbbbaabbbaaaaaabbbbbababaaaaabbaaabba\n";

template<class Container_T>
Container_T split(const std::string& str, char delim = ' ', bool trimWhitespace = true)
{
	std::istringstream ss{ str };
	std::string token;
	Container_T result{};
	
	while (std::getline(ss, token, delim))
	{
		const char* WS = " \t\n";

		if (trimWhitespace)
		{
			auto firstNonWS = token.find_first_not_of(WS);
			token = (firstNonWS != std::string::npos)
				? token.substr(firstNonWS, token.find_last_not_of(WS) - firstNonWS + 1)
				: "";
		}
		
		if (!token.empty())
			result.insert(result.end(), token);
	}
	return result;
}

std::vector<std::string> split(const std::string& str, char delim = ' ', bool trimWhitespace = true)
{
	return split<std::vector<std::string>>(str, delim, trimWhitespace);
}

class Rule;
using rules_t = std::map<size_t, std::shared_ptr<Rule>>;

class Rule
{
public:
	Rule(size_t index) : m_index(index) {}
	size_t getIndex() const { return m_index; }
	virtual bool match(const std::string& str, const rules_t& rules) const
	{
		auto matchLengths = match(str, 0, rules);
		return std::any_of(matchLengths.begin(), matchLengths.end(), [&](auto length)
			{
				return length == str.length();
			});
	};

	virtual ~Rule() = default;


	virtual std::vector<size_t> match(const std::string& str, size_t startIndex, const rules_t& rules) const = 0;
	virtual std::string toString() const = 0;
private:
	size_t m_index;
};

std::ostream& operator<<(std::ostream& os, const Rule& rule)
{
	return os << rule.toString();
}

class StringRule : public Rule
{
public:
	StringRule(size_t index, const std::string& str) : Rule{ index }, m_str(str) {}
	virtual std::string toString() const override
	{
		return getIndex() + ": \"" + m_str + "\"";
	};
protected:
#pragma warning(push)
#pragma warning(disable: 4100)
	virtual std::vector<size_t> match(const std::string& str, size_t startIndex, const rules_t& rules) const override
	{
		if (startIndex + m_str.length() > str.length())
			return { };

		return (str.substr(startIndex, m_str.length()) == m_str) ? std::vector<size_t>{ m_str.length() } : std::vector<size_t>{};
	}
#pragma warning(pop)
private:
	std::string m_str;
};

class CompoundRule : public Rule
{
public:
	CompoundRule(size_t index, const std::vector<std::vector<size_t>>& rules) : Rule{ index }, m_rules(rules) {}
	
	virtual std::string toString() const override
	{
		std::ostringstream os;
		os << getIndex() << ": ";
		for (size_t i = 0; i < m_rules.size(); ++i)
		{
			for (size_t j = 0; j < m_rules[i].size(); ++j)
			{
				os << m_rules[i][j];
				if (j < m_rules[i].size() - 1)
					os << " ";
			}
			if (i < m_rules.size() - 1)
				os << " | ";
		}
		return os.str();
	}

protected:
	virtual std::vector<size_t> match(const std::string& str, size_t startIndex, const rules_t& rules) const override
	{
		std::vector<size_t> potentialMatchLengths;
		for (auto andIndices : m_rules)
		{
			auto t = matchAll(str, startIndex, andIndices.begin(), andIndices.end(), rules);
			std::copy(t.begin(), t.end(), std::back_inserter(potentialMatchLengths));
		}

		return potentialMatchLengths;
	}
private:
	std::vector<size_t> matchAll(const std::string& str, size_t startIndex,
		std::vector<size_t>::const_iterator ruleStart, std::vector<size_t>::const_iterator ruleEnd, const rules_t& rules) const
	{
		if (ruleStart == ruleEnd)
			return {};

		std::vector<size_t> results;
		
		auto& rule = rules.find(*ruleStart)->second;
		auto matches = rule->match(str, startIndex, rules);
		if (matches.empty())
			return {};

		if (ruleStart + 1 == ruleEnd)
		{
			return matches;
		}
		else
		{
			for (auto matchLength : matches)
			{
				auto temp = matchAll(str, startIndex + matchLength, ruleStart + 1, ruleEnd, rules);
				for (auto t : temp)
					results.push_back(matchLength + t);
			}
			return results;
		}
	}

	const std::vector<std::vector<size_t>> m_rules;
};


rules_t parseRules(const std::map<size_t, std::string>& ruleStrings)
{
	std::map<size_t, std::shared_ptr<Rule>> rules;
	
	for (auto& [ruleIndex, ruleString] : ruleStrings)
	{
		auto charIndex = ruleString.find('"');
		if (charIndex != std::string::npos)
		{
			++charIndex;
			rules[ruleIndex] = std::make_shared<StringRule>(ruleIndex, ruleString.substr(charIndex, ruleString.find('"', charIndex) - charIndex));
			continue;
		}

		auto parts = split(ruleString, '|');
		std::vector<std::vector<size_t>> indices;
		for (const auto& part : parts)
		{
			std::vector<size_t> andIndices;
			for (const auto& index : split(part))
				andIndices.push_back(std::stoull(index));

			indices.push_back(andIndices);
		}

		rules[ruleIndex] = std::make_shared<CompoundRule>(ruleIndex, indices);
	}
	return rules;
}

int main()
{
	//std::istringstream is(EXAMPLE2);
	std::ifstream is("input.txt");
	std::string line;
	std::map<size_t, std::string> ruleStrings;
	while (std::getline(is, line) && !line.empty())
	{
		auto parts = split(line, ':');
		auto ruleIndex = std::stoull(parts[0]);
		auto rule = parts[1];
		ruleStrings.insert({ ruleIndex, rule });
	}

	auto part1Rules = parseRules(ruleStrings);
	auto& part1Rule = part1Rules[0];

	// modify for part 2
	ruleStrings[8] = "42 | 42 8" ;
	ruleStrings[11] ="42 31 | 42 11 31";

	auto part2Rules = parseRules(ruleStrings);
	auto& part2Rule = part2Rules[0];

	size_t numMatching1(0);
	size_t numMatching2(0);
	while (std::getline(is, line) && !line.empty())
	{
		if (part1Rule->match(line, part1Rules))
			++numMatching1;
		if (part2Rule->match(line, part2Rules))
			++numMatching2;
	}

	std::cout << "Day19 Part 1: " << numMatching1 << std::endl;
	std::cout << "Day19 Part 2: " << numMatching2 << std::endl;

}

