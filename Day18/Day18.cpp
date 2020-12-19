#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const char* EXAMPLES =
"2 * 3 + (4 * 5)\n"
"5 + (8 * 3 + 9 + 3 * 4 * 3)\n"
"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))\n"
"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"
;

// <subexpr, nextIndex>
std::pair<std::string, size_t> extractSubExpr(const std::string& expression, size_t startIndex)
{
	// scan for matching end parenthesis
	size_t depth(1), index(startIndex + 1);
	while (depth > 0)
	{
		if (expression[index] == '(')
			depth++;
		else if (expression[index] == ')')
			depth--;
		++index;
	}

	return {expression.substr(startIndex + 1, index - startIndex - 2), index };
}

int64_t evaluate1(const std::string& expression)
{
	size_t nextSubExpr = expression.find('(');
	std::stringstream ss;
	ss << expression.substr(0, nextSubExpr);
	while (nextSubExpr != std::string::npos)
	{
		auto [subExpr, nextIndex] = extractSubExpr(expression, nextSubExpr);
		ss << evaluate1(subExpr);

		nextSubExpr = expression.find('(', nextIndex);
		auto start = nextIndex;
		auto length = (nextSubExpr != std::string::npos) ? nextSubExpr - nextIndex : expression.length() - nextIndex;
		ss << expression.substr(start, length);
	}

	char op;
	int64_t result, operand;

	ss >> result;
	while (ss >> op)
	{
		ss >> operand;
		switch (op)
		{
			case '+':
				result += operand;
				break;
			case '*':
				result *= operand;
				break;
		}
	}
	
	return result;
}

int64_t evaluate2(const std::string& expression)
{
	size_t nextSubExpr = expression.find('(');
	std::stringstream ss;
	ss << expression.substr(0, nextSubExpr);
	while (nextSubExpr != std::string::npos)
	{
		auto [subExpr, nextIndex] = extractSubExpr(expression, nextSubExpr);
		ss << evaluate2(subExpr);

		nextSubExpr = expression.find('(', nextIndex);
		auto start = nextIndex;
		auto length = (nextSubExpr != std::string::npos) ? nextSubExpr - nextIndex : expression.length() - nextIndex;
		ss << expression.substr(start, length);
	}

	char op;
	int64_t result, operand;

	size_t plusIndex;
	while ((plusIndex = ss.str().find('+')) != std::string::npos)
	{
		std::string temp(ss.str());
		ss = {};
		auto start = std::max(0ull, temp.rfind('*', plusIndex)) + 1;
		auto end = std::min(temp.length(), temp.find('*', plusIndex));
		if (end - start == temp.length())
		{
			ss << temp;
			break;
		}
		else
		{
			ss << temp.substr(0, start);
			ss << evaluate2(temp.substr(start, end - start));
			ss << temp.substr(end);
		}
	}

	ss >> result;
	while (ss >> op)
	{
		ss >> operand;
		switch (op)
		{
			case '+':
				result += operand;
				break;
			case '*':
				result *= operand;
				break;
		}
	}

	return result;
}


int main()
{
	//std::istringstream is(EXAMPLES);
	std::ifstream is("input.txt");

	int64_t sum1(0), sum2(0);

	std::string line;
	while (std::getline(is, line) && !line.empty())
	{
		sum1 += evaluate1(line);
		sum2 += evaluate2(line);
	}
	std::cout << "Day18 Part 1: " << sum1 << std::endl;
	std::cout << "Day18 Part 2: " << sum2 << std::endl;

}

