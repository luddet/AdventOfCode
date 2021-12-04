#include "utilities.h"

std::vector<int> readInts(std::istream& os)
{
	std::vector<int> result;
	while (!os.eof())
	{
		int i;
		os >> i;
		result.push_back(i);
	}
	return result;
}
