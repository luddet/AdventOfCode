#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

//2 * 3 + (4 * 5) becomes 26.
//5 + (8 * 3 + 9 + 3 * 4 * 3) becomes 437.
//5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4)) becomes 12240.
//((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2 becomes 13632.

const std::vector<std::string> EXAMPLES
{
"2 * 3 + (4 * 5)",
"5 + (8 * 3 + 9 + 3 * 4 * 3)",
"5 * 9 * (7 * 3 * 3 + 9 * 3 + (8 + 6 * 4))",
"((2 + 4 * 9) * (6 + 9 * 8 + 6) + 6) + 2 + 4 * 2"
};


int64_t evaluate(const std::string& expression)
{
	size_t firstP = expression.find('(');
	std::stringstream ss;
	ss << expression.substr(0, firstP);
	while (firstP != std::string::npos)
	{
		// scan for matching end parenthesis
		size_t depth(1), lastIndex(firstP + 1);
		while (depth > 0)
		{
			if (expression[lastIndex] == '(')
				depth++;
			else if (expression[lastIndex] == ')')
				depth--;
			++lastIndex;
		}
		--lastIndex;
		ss << evaluate(expression.substr(firstP + 1, (lastIndex - firstP - 1)));
		firstP = expression.find('(', lastIndex);
		ss << expression.substr(lastIndex + 1, firstP - lastIndex - 1);
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

int main()
{
	std::ifstream is("input.txt");
	int64_t sum(0);
	std::string line;
	while (std::getline(is, line) && !line.empty())
	{
		sum += evaluate(line);
	}

	std::cout << "Day18 Part 1: " << sum << std::endl;

}

