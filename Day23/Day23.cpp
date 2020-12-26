#include <iostream>
#include <vector>
#include <string>
#include <array>

const std::string INPUT = "716892543";
const std::string EXAMPLE = "389125467";

int main()
{
	const size_t RINGSIZE = 9;;
	const size_t PICKUPSIZE = 3;

	auto& in = INPUT;
	std::array<uint32_t, RINGSIZE> ring{};
	for (size_t i = 0; i < in.length(); ++i)
		ring[i] = std::stoul(in.substr(i,1));

	size_t currentIndex{ 0 };

	for (size_t iteration = 1; iteration <= 100; ++iteration)
	{
		std::cout << "-- move " << iteration << " --" << std::endl;
		for (size_t i = 0; i < RINGSIZE; ++i)
			std::cout << ((i == currentIndex) ? "(" : "") << ring[i] << ((i == currentIndex) ? ") " : " ");
		std::cout << std::endl;

		//auto currentLabel{ ring[currentIndex] };
		std::array<uint32_t, PICKUPSIZE> pickup{ ring[(currentIndex + 1) % RINGSIZE], 
										ring[(currentIndex + 2) % RINGSIZE],
										ring[(currentIndex + 3) % RINGSIZE] };
		std::cout << "pick up: " << pickup[0] << ", " << pickup[1] << ", " << pickup[2] << std::endl;

#if _DEBUG
		ring[(currentIndex + 1) % RINGSIZE] = UINT32_MAX;
		ring[(currentIndex + 2) % RINGSIZE] = UINT32_MAX;
		ring[(currentIndex + 3) % RINGSIZE] = UINT32_MAX;
#endif
		for (size_t i = 0; i < RINGSIZE - PICKUPSIZE - 1; ++i)
		{
			ring[(currentIndex + 1 + i) % RINGSIZE] = ring[(currentIndex + PICKUPSIZE + 1 + i) % RINGSIZE];
#if _DEBUG
			ring[(currentIndex + PICKUPSIZE + 1 + i) % RINGSIZE] = UINT32_MAX;
#endif
		}

		auto destinationLabel = (ring[currentIndex] > 1) ? (ring[currentIndex] - 1) : RINGSIZE;
		while (std::find(pickup.begin(), pickup.end(), destinationLabel) != pickup.end())
			destinationLabel = destinationLabel > 1 ? destinationLabel  -1 : RINGSIZE;
		std::cout << "destination: " << destinationLabel << std::endl << std::endl;

		size_t destinationIndex = std::distance(ring.begin(), std::find(ring.begin(), ring.end(), destinationLabel));
		size_t numberToMove = (currentIndex + RINGSIZE - destinationIndex - PICKUPSIZE - 1) % RINGSIZE;
		size_t shiftDest = (currentIndex - 1 + RINGSIZE) % RINGSIZE;
		size_t shiftSrc = (shiftDest - PICKUPSIZE + RINGSIZE) % RINGSIZE;
		for (size_t i = 0; i < numberToMove; ++i)
		{
			ring[shiftDest] = ring[shiftSrc];
#if _DEBUG
			ring[shiftSrc] = UINT32_MAX;
#endif
			shiftDest = (--shiftDest + RINGSIZE) % RINGSIZE;
			shiftSrc = (--shiftSrc + RINGSIZE) % RINGSIZE;
		}

		ring[(destinationIndex + 1) % RINGSIZE] = pickup[0];
		ring[(destinationIndex + 2) % RINGSIZE] = pickup[1];
		ring[(destinationIndex + 3) % RINGSIZE] = pickup[2];

		currentIndex = ++currentIndex % RINGSIZE;
	}

	std::cout << "-- final --" << std::endl;
	for (size_t i = 0; i < RINGSIZE; ++i)
		std::cout << ((i == currentIndex) ? "(" : "") << ring[i] << ((i == currentIndex) ? ") " : " ");
	std::cout << std::endl;

	auto start = std::distance(ring.begin(), std::find(ring.begin(), ring.end(), 1ull)) + 1ull;
	std::cout << "Day23 Part 1: ";
	for (size_t i = 0; i < RINGSIZE - 1; ++i)
		std::cout << ring[(start + i) % RINGSIZE];
	std::cout << std::endl;
}

