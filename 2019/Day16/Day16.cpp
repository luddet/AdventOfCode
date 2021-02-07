// https://adventofcode.com/2019/day/16

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <numeric>
#include <chrono>
#include <ppl.h>
#include <concrtrm.h>

class ScopeTimer
{
public:
	ScopeTimer(std::chrono::duration<double>& duration):m_duration(duration)
	{
		m_start = std::chrono::steady_clock::now();
	}

	~ScopeTimer()
	{
		m_duration = std::chrono::steady_clock::now() - m_start;
	}

private:
	std::chrono::duration<double>& m_duration;
	std::chrono::steady_clock::time_point m_start;
};

const char* EXAMPLE1 = "12345678";
const char* EXAMPLE2 = "80871224585914546619083218645595";
const char* EXAMPLE3 = "123456789";
const char* EXAMPLE4 = "03036732577212944063491565474664";

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v)
{
	size_t i = 0;
	for (auto c : v)
		os << c << ((++i % 10 == 0) ? " " : "");
	return os;
}


std::vector<int64_t> transform(const std::vector<uint8_t>& list, size_t iterations)
{
	const size_t listSize = list.size();
	std::vector<int64_t> partials(listSize);
	std::vector<int64_t> output(list.begin(), list.end());

	std::chrono::duration<double> time;
	{
		ScopeTimer timer(time);
		for (size_t iteration = 0; iteration < iterations; ++iteration)
		{
			std::partial_sum(output.begin(), output.end(), partials.begin());
			
			int64_t sum{ 0 }, factor{ 1 };
			for (size_t i = 0; i < listSize; i += 2, factor = -factor)
				sum += factor * output[i];
			output[0] = abs(sum) % 10;

			concurrency::parallel_for(size_t(1), listSize / 2, [listSize, &partials, &output](size_t i) 
				{
					const size_t stepSize{ 2 * (i + 1) };
					size_t startIndex{ i }, endIndex{ startIndex + i };
					int64_t factor = 1;
					int64_t sum = 0;
					for (; endIndex < listSize; startIndex += stepSize, endIndex += stepSize, factor = -factor)
						sum += factor * (partials[endIndex] - partials[startIndex - 1]);

					if (startIndex < listSize)
						sum += factor * (partials[listSize - 1] - partials[startIndex - 1]);

					output[i] = abs(sum) % 10;
				});
			
			int64_t last = partials.back();
			for (size_t i = listSize/2; i < listSize; ++i)
				output[i] = (last - partials[i-1]) % 10;
		}
	}
	std::cout << "total time (s): " << time.count() << std::endl;
	return output;
}

int main()
{
	//std::istringstream is(EXAMPLE4);
	std::ifstream is("input.txt");

	std::string input;
	std::getline(is, input);

	std::vector<uint8_t> list;
	list.reserve(input.size());
	for (auto c : input)
		list.push_back(c - '0');

	auto part1Transformed = transform(list, 100);

	std::vector<char> finalList{};
	std::transform(part1Transformed.begin(), part1Transformed.begin() + 8, std::back_inserter(finalList), [](auto e) -> char { return (char)e + '0'; });

	size_t offset = 0;
	for (size_t i = 0; i < 7; ++i)
		offset += size_t(std::pow(10, i)) * list[6 - i];

	const size_t repeats{ 10000 };
	std::vector<uint8_t> part2Input(list.size() * repeats);
	auto dest = part2Input.begin();
	for (size_t i = 0; i < repeats; ++i)
		dest = std::copy(list.begin(), list.end(), dest);

	auto part2Transformed = transform(part2Input, 100);

	std::vector<char> part2Final{};
	std::transform(part2Transformed.begin() + offset, part2Transformed.begin() +offset + 8, std::back_inserter(part2Final), [](auto e) -> char { return (char)e + '0'; });

	std::cout << "Day16 Part1: " << finalList << std::endl;
	std::cout << "Day16 Part2: " << part2Final << std::endl;
}
