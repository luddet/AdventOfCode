#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <chrono>

std::vector<const char*> EXAMPLE {
	"939\n7, 13, x, x, 59, x, 31, 19",
	"939\n17,x,13,19", // part2: 3417
	"939\n67,7,59,61", // 754018
	"939\n67,x,7,59,61", // 779210
	"939\n67,7,x,59,61", // 1261476
	"939\n1789,37,47,1889", // 1202161486
	"939\n4,x,7,x,13,x,9,x,x,5"
};

int main()
{
	//std::istringstream is(EXAMPLE[0]);
	std::ifstream is("input.txt");

	std::vector<std::pair<uint32_t, uint32_t>> ids;
	uint32_t earliestDeparture;
	is >> earliestDeparture;

	{
		uint32_t offset(0);
		std::string id;
		while (std::getline(is, id, ','))
		{
			if (id != " x" && id != "x")
				ids.push_back({ std::stoi(id), offset });
			++offset;
		}
	}

	uint32_t departureTime(earliestDeparture);
	uint32_t busId(0);
	while (true)
	{
		for (auto id : ids)
		{
			if (departureTime % id.first == 0)
			{
				busId = id.first;
				break;
			}
		}
		if (busId != 0)
			break;

		departureTime++;
	}

	auto part1 = (departureTime - earliestDeparture) * busId;

	auto [maxId, maxOffset] = *std::max_element(std::begin(ids), std::end(ids));

	uint64_t iteration(1);
	uint64_t currentTime(0);
	uint64_t currentIdsInPeriod(1);
	uint64_t currentPeriod(ids[0].first);

	auto startTime = std::chrono::steady_clock::now();
	while (true)
	{
		while (currentIdsInPeriod < ids.size())
		{
			if ((currentTime + ids[currentIdsInPeriod].second) % ids[currentIdsInPeriod].first == 0)
			{
				currentPeriod *= ids[currentIdsInPeriod].first;
				++currentIdsInPeriod;
			}
			else
				break;
		}

		if (currentIdsInPeriod == ids.size()) // Found all
			break;

		currentTime += currentPeriod;
		iteration++;
	}
	auto endTime = std::chrono::steady_clock::now();
	auto time = std::chrono::duration<double>(endTime - startTime);
	std::cout << "part 2 iterations: " << iteration << std::endl;
	std::cout << iteration/time.count() << " iteration/s" << std::endl;

	std::cout << "Day13 Part 1: " << part1 << std::endl;
	std::cout << "Day13 Part 2: " << currentTime << std::endl;



}

