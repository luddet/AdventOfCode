#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>
#include <set>

const char* EXAMPLE =
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

class Rule
{
public:
	virtual bool match(const std::string& str) const
	{
		return match(str, 0) == str.length();
	};

	virtual ~Rule() = default;


	virtual size_t match(const std::string& str, size_t startIndex) const = 0;
};


class StringRule : public Rule
{
public:
	StringRule(const std::string& str) : m_str(str) {}

protected:
	virtual size_t match(const std::string& str, size_t startIndex) const override
	{
		if (startIndex + m_str.length() > str.length())
			return 0;

		return (str.substr(startIndex, m_str.length()) == m_str) ? m_str.length() : 0;
	}

private:
	std::string m_str;
	friend std::ostream& operator<<(std::ostream& os, const StringRule& rule);
};
std::ostream& operator<<(std::ostream& os, const StringRule& rule) { return os << rule.m_str; }

class AndRule : public Rule
{
public:
	AndRule(std::vector<std::shared_ptr<Rule>>& rules) : m_rules(rules)	{}

protected:
	virtual size_t match(const std::string& str, size_t startIndex) const override
	{
		size_t currentMatchLength(0);
		size_t matchLength(0);
		for (auto& rule : m_rules)
		{
			if ((currentMatchLength = rule->match(str, startIndex + matchLength)) > 0)
				matchLength += currentMatchLength;
			else
				break;
		}

		return (currentMatchLength > 0) ? matchLength : 0;
	}
private:
	std::vector<std::shared_ptr<Rule>> m_rules;
};

class OrRule : public Rule
{
public:
	OrRule(std::vector<std::shared_ptr<Rule>>& rules) : m_rules(rules)	{ }

protected:
	virtual size_t match(const std::string& str, size_t startIndex) const override
	{
		size_t matchLength(0);
		for (auto& rule : m_rules)
		{
			if ((matchLength = rule->match(str, startIndex + matchLength)) > 0)
				break;
		}
		return matchLength;
	}
private:
	std::vector<std::shared_ptr<Rule>> m_rules;
};

#pragma warning(push)
#pragma warning(disable: 4100)
std::shared_ptr<Rule> buildRuleTree(size_t ruleIndex,
	const std::map<size_t, const std::string>& ruleStrings,
	std::map<size_t, std::shared_ptr<Rule>>& rules)
{
	auto ruleIt = rules.find(ruleIndex);
	if (ruleIt != rules.end())
		return ruleIt->second;

	const auto& currentStr = (*ruleStrings.find(ruleIndex)).second;
	auto charIndex = currentStr.find('"');
	if (charIndex != std::string::npos)
	{
		++charIndex;
		auto newRule = std::make_shared<StringRule>(currentStr.substr(charIndex, currentStr.find('"', charIndex) - charIndex));
		rules[ruleIndex] = newRule;
		return newRule;
	}

	auto parts = split(currentStr, '|');
	std::vector<std::shared_ptr<Rule>> orRules;
	for (const auto& part : parts)
	{
		std::vector<std::shared_ptr<Rule>> andRules;
		for (const auto& index : split(part))
			andRules.push_back(buildRuleTree(std::stoull(index), ruleStrings, rules));

		auto newRule = std::make_shared<AndRule>(andRules);
		orRules.push_back(newRule);
	}

	auto newRule = (orRules.size() == 1)? orRules[0]:  std::make_shared<OrRule>(orRules);
	rules[ruleIndex] = newRule;
	return newRule;
}

#pragma warning(pop)

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");
	std::string line;
	std::map<size_t, const std::string> ruleStrings;
	std::map<size_t, std::shared_ptr<Rule>> rules;
	while (std::getline(is, line) && !line.empty())
	{
		auto parts = split(line, ':');
		auto ruleIndex = std::stoull(parts[0]);
		auto rule = parts[1];
		ruleStrings.insert({ ruleIndex, rule });
	}


	std::shared_ptr<Rule> rule = buildRuleTree(0, ruleStrings, rules);

	size_t numMatching(0);
	while (std::getline(is, line) && !line.empty())
	{
		if (rule->match(line))
			++numMatching;
	}

	std::cout << "Day19 Part 1: " << numMatching << std::endl;

}

