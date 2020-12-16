#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

const std::string EXAMPLE("939\n7, 13, x, x, 59, x, 31, 19");

int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	int32_t earliestDeparture;
	std::vector<int32_t> ids;

	{
		is >> earliestDeparture;
		std::string id;
		while (std::getline(is, id, ','))
		{
			if (id == " x" || id == "x")
				continue;

			ids.push_back(std::stoi(id));
		}
	}

	int32_t departureTime(earliestDeparture);
	int32_t busId(0);
	while (true)
	{
		for (auto id : ids)
		{
			if (departureTime % id == 0)
			{
				busId = id;
				break;
			}
		}
		if (busId != 0)
			break;

		departureTime++;
	}

	auto part1 = (departureTime - earliestDeparture) * busId;

	std::cout << "Day13 Part 1: " << part1 << std::endl;




}

