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

	uint64_t currentTime(maxId - maxOffset);
	uint64_t iteration(0);

	std::vector<uint64_t> flattenedIds;
	for (auto id : ids)
	{
		flattenedIds.push_back(id.second);
		flattenedIds.push_back(id.first);
	}

	auto startTime = std::chrono::steady_clock::now();
	while (true /* iteration < 1000000000 */)
	{
		if (iteration % 1000000000 == 0)
			std::cout << "Current time: " << currentTime << std::endl;
		uint64_t mod(0);
		for (size_t i = 0; mod == 0 && i < flattenedIds.size(); i += 2)
			mod = (currentTime + flattenedIds[i]) % flattenedIds[i+1];
		if (mod == 0)
			break;
		currentTime += maxId;
		iteration++;
	}
	auto endTime = std::chrono::steady_clock::now();
	auto time = std::chrono::duration<double>(endTime - startTime);
	std::cout << iteration/time.count() << " iteration/s" << std::endl;

	std::cout << "Day13 Part 1: " << part1 << std::endl;
	std::cout << "Day13 Part 2: " << currentTime << std::endl;



}

