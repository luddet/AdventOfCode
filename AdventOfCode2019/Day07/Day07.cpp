// https://adventofcode.com/2019/day/7

#include <iostream>
#include <vector>
#include <array>

template<size_t SIZE>
std::ostream& operator<<(std::ostream& os, const std::array<uint8_t, SIZE>& arr)
{
	for (size_t i = 0; i < SIZE - 1; ++i)
		os << (int)arr[i] << ", ";
	os << (int)arr[SIZE - 1];
	return os;
}

// Simple naive permutation generation, good enough for five elements
template<class T, size_t SIZE>
std::vector<std::array<T, SIZE>> getPermutations(const std::array<T, SIZE>& sequence)
{
	std::vector<std::array<T, SIZE>> permutations;
	for (size_t i = 0; i < SIZE; ++i)
	{
		std::array<T, SIZE - 1> subSequence;
		for (size_t subIndex = 0, orgIndex = 0; subIndex < SIZE-1; ++subIndex, ++orgIndex)
		{
			if (i == orgIndex)
				++orgIndex;
			subSequence[subIndex] = sequence[orgIndex];
		}

		auto subPerms = getPermutations(subSequence);

		for (auto sub : subPerms)
		{
			std::array<T, SIZE> currentPerm;
			currentPerm[0] = sequence[i];
			for (size_t subIndex = 1; subIndex < SIZE; ++subIndex)
				currentPerm[subIndex] = sub[subIndex - 1];
			permutations.push_back(currentPerm);
		}
	}
	return permutations;
}

template<class T>
std::vector<std::array<T, 2>> getPermutations(const std::array<T, 2>& sequence)
{
	std::vector<std::array<T, 2>> permutations;
	permutations.push_back(std::array<T, 2>{sequence});
	permutations.push_back(std::array<T, 2>{sequence[1], sequence[0]});
	return permutations;
}

int main()
{
	constexpr uint8_t PHASE_COUNT = 5;
	std::array<uint8_t, PHASE_COUNT> phases{ 0, 1, 2, 3, 4 };

	auto permutations = getPermutations(phases);

	for (auto permutation : permutations)
	{
		std::cout << permutation << std::endl;
	}
}

