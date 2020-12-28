#include <iostream>
#include <vector>
#include <string>
#include <array>
#include <cassert>
#include <chrono>
#include <numeric>
#include <stack>
#include <algorithm>
#include <random>
#include <iomanip>

#define DEBUGPRINT 0

const std::vector<uint32_t> INPUT = { 7,1,6,8,9,2,5,4,3 };
const std::vector<uint32_t> EXAMPLE = { 3,8,9,1,2,5,4,6,7 };

template<class Fun>
class ScopeGuard
{
public:
	ScopeGuard(Fun fun):m_fun(fun) {}
	~ScopeGuard() { m_fun(); }
private:
	Fun m_fun;
};

template<class InputIt, class T>
InputIt hintFind(InputIt start, InputIt end, const T& value, InputIt startHint)
{
	constexpr uint64_t interval{ 10000 };

	static uint32_t numCalls = 0;
	static uint32_t numBack = 0;
	static uint32_t numForward = 0;
	static uint32_t numBackward = 0;
	static uint32_t numFallback = 0;

	//ScopeGuard guard([&]()
	//	{
	//		++numCalls;
	//		if (numCalls % 10000 == 0)
	//		{
	//			std::cout << "numCalls: " << std::setw(6) << numCalls << " numBack: " << std::setw(6) << numBack << " , numForward: " << std::setw(6) << numForward;
	//			std::cout << " , numBackward: " << std::setw(6) << numBackward << " , numFallback: " << std::setw(6) << numFallback << std::endl;
	//		}
	//	});

	InputIt it;
	if ((it = std::find(end - 3, end, value)) != end)
	{
		++numBack;
		return it;
	}

	auto partEnd = startHint + std::min(uint64_t(std::distance(startHint, end)), interval);
	if ((it = std::find(startHint, partEnd, value)) != partEnd)
	{
		++numForward;
		return it;
	}

	auto rStart = std::make_reverse_iterator(startHint);
	auto rEnd = std::make_reverse_iterator(startHint - std::min(uint64_t(std::distance(start, startHint)), interval));
	auto rIt = std::find(rStart, rEnd , value);
	if (rIt != rEnd)
	{
		++numBackward;
		return std::prev(rIt.base());
	}
	
	++numFallback;
	return std::find(start, end, value);
}

template<class InputIt, class T>
InputIt bisectFind(InputIt start, InputIt end, const T& value)
{
	std::stack<std::pair<InputIt, InputIt>> intervals;
	InputIt s{ start }, e{ end };
	do
	{
		if (s == e)
		{
			s = intervals.top().first;
			e = intervals.top().second;
			intervals.pop();
		}
		else if (*s == value)
		{
			return s;
		}

		auto halfDistance = (std::distance(s, e) / 2);
		if (halfDistance == 0)
		{
			s = e;
			continue;
		}
		auto m = s + halfDistance;
		bool goLeft = value < *m;
		intervals.emplace((goLeft ? std::pair{ m, e } : std::pair{ s, m }));
		if (goLeft)
			e = m;
		else
			s = m;
	} while (!intervals.empty());

	return end;
}

template<size_t NUM_CUPS>
std::array<uint32_t, NUM_CUPS> execute(const std::vector<uint32_t>& seed, const size_t iterations)
{
	constexpr size_t NUM_PICKUPS{ 3 };
	assert(NUM_CUPS >= seed.size());
	std::array<uint32_t, NUM_CUPS> cups1;
	std::array<uint32_t, NUM_CUPS> cups2;


//	// copy seed and fill rest incrementally
	auto next = std::copy(seed.begin(), seed.end(), cups1.begin());
	auto seedMax = *std::max_element(seed.begin(), seed.end());
	if (next != cups1.end())
		std::iota(next, cups1.end(), seedMax + 1);
	std::copy(cups1.begin(), cups1.end(), cups2.begin());

	size_t currentIndex{ 0 };
	size_t destinationIndex{ 0 };

	auto cups = &cups1;
	auto oldCups = &cups2;

	&oldCups;

	auto lastPrint = std::chrono::steady_clock::now();

	for (size_t iteration = 1; iteration <= iterations; ++iteration)
	{
		if (std::chrono::duration<double>(std::chrono::steady_clock::now() - lastPrint).count() > 5.0)
		{
			lastPrint = std::chrono::steady_clock::now();
			std::cout << "Iteration: " << iteration << std::endl;
		}

#if DEBUGPRINT
		std::cout << "-- move " << iteration << " --" << std::endl;
		for (size_t i = 0; i < NUM_CUPS; ++i)
			std::cout << ((i == currentIndex) ? "(" : "") << cups[i] << ((i == currentIndex) ? ") " : " ");
		std::cout << std::endl;
#endif
		std::array<uint32_t, NUM_PICKUPS> pickup{ (*cups)[(currentIndex + 1) % NUM_CUPS],
										(*cups)[(currentIndex + 2) % NUM_CUPS],
										(*cups)[(currentIndex + 3) % NUM_CUPS] };
#if DEBUGPRINT
		std::cout << "pick up: " << pickup[0] << ", " << pickup[1] << ", " << pickup[2] << std::endl;
#endif

#if _DEBUG
		(*cups)[(currentIndex + 1) % NUM_CUPS] = UINT32_MAX;
		(*cups)[(currentIndex + 2) % NUM_CUPS] = UINT32_MAX;
		(*cups)[(currentIndex + 3) % NUM_CUPS] = UINT32_MAX;
#endif
		auto destinationLabel = ((*cups)[currentIndex] > 1) ? ((*cups)[currentIndex] - 1) : NUM_CUPS;
		while (std::find(pickup.begin(), pickup.end(), destinationLabel) != pickup.end())
			destinationLabel = destinationLabel > 1 ? destinationLabel - 1 : NUM_CUPS;
		
		if ((*cups)[destinationIndex + NUM_PICKUPS] != destinationLabel)
		{
			auto destIt = std::find(cups->rbegin(), cups->rend(), destinationLabel).base() - 1;
			destinationIndex = std::distance(cups->begin(), destIt);
		}

		if (currentIndex > NUM_CUPS - NUM_PICKUPS - 1)
		{
			for (size_t i = currentIndex + 1; i < destinationIndex + NUM_CUPS - NUM_PICKUPS + 1; ++i)
				(*cups)[i % NUM_CUPS] = (*cups)[(i+NUM_PICKUPS) % NUM_CUPS];
			destinationIndex -= NUM_PICKUPS;
		}
		else if (destinationIndex > currentIndex)
		{
			size_t copyDest = currentIndex + 1;
			size_t copyStart = copyDest + NUM_PICKUPS;
			size_t copyEnd = destinationIndex + 1;
			std::copy_n(cups + copyStart, copyEnd-copyStart, cups + copyDest);
			destinationIndex -= NUM_PICKUPS;
		}
		else if (destinationIndex < currentIndex)
		{
			size_t copyStart = destinationIndex + 1;
			size_t copyEnd = currentIndex + 1;
			size_t copyDest = currentIndex + 1 + NUM_PICKUPS;
			std::copy_backward(cups->begin() + copyStart, cups->begin() + copyEnd, cups->begin() + copyDest);
			currentIndex += NUM_PICKUPS;
		}


#if DEBUGPRINT
		std::cout << "destination: " << destinationLabel << std::endl << std::endl;
#endif

		(*cups)[(destinationIndex + 1) % NUM_CUPS] = pickup[0];
		(*cups)[(destinationIndex + 2) % NUM_CUPS] = pickup[1];
		(*cups)[(destinationIndex + 3) % NUM_CUPS] = pickup[2];

		currentIndex = ++currentIndex % NUM_CUPS;
	}
	return *cups;
}

void findTests();
void findTests2();

int main()
{
	auto& in = INPUT;

	auto cups1 = execute<9>(in, 100);

#if DEBUGPRINT
	std::cout << "-- final --" << std::endl;
	for (size_t i = 0; i < cups1.size(); ++i)
		std::cout << cups1[i] << " ";
	std::cout << std::endl;
#endif

	auto cups2 = execute<1000000>(in, 10000000);
	auto oneIt = std::find(cups2.begin(), cups2.end(), 1ull);
	uint64_t part2 = *(++oneIt);
	part2 *= *(++oneIt);

	std::cout << "Day23 Part 1: ";
	auto start = std::distance(cups1.begin(), std::find(cups1.begin(), cups1.end(), 1ull)) + 1ull;
	for (size_t i = 0; i < cups1.size() - 1; ++i)
		std::cout << cups1[(start + i) % cups1.size()];
	std::cout << std::endl;

	//std::cout << "Day23 Part 2: " << part2 << std::endl;



}


void findTests()
{
	std::array<uint32_t, 200000> test;
	std::iota(test.begin(), test.end(), 0ul);

	for (size_t i = 0; i < test.size(); ++i)
	{
		auto rnd = ((50 * rand()) / RAND_MAX) - 25;
		std::swap(test[i], test[std::abs(int(i + rnd)) % test.size()]);
	}

	std::cout << "bisect... ";

	auto start = std::chrono::steady_clock::now();
	for (size_t i = 0; i < test.size(); ++i)
	{
		auto it = bisectFind(test.begin(), test.end(), i);
		//if (i % 1000 == 0)
		//	std::cout << i << " -> " << *it << ": " << (i == *it ? " !" : " FAIL")  << std::endl;
	}
	auto bisectTime = std::chrono::steady_clock::now() - start;
	std::cout << "done." << std::endl;

	std::cout << "std::find...." << std::endl;
	start = std::chrono::steady_clock::now();
	for (size_t i = 0; i < test.size(); ++i)
	{
		auto it = std::find(test.begin(), test.end(), i);
		//if (i % 1000 == 0)
		//	std::cout << i << " -> " << *it << ": " << (i == *it ? " !" : " FAIL") << std::endl;
	}
	auto findTime = std::chrono::steady_clock::now() - start;
	std::cout << "done." << std::endl;

	std::cout << "bisect: " << std::chrono::duration<double>(bisectTime).count() << std::endl;
	std::cout << "std::find: " << std::chrono::duration<double>(findTime).count() << std::endl;
}


void findTests2()
{
	std::array<uint32_t, 12> test;
	std::iota(test.begin(), test.end(), 0ul);

	for (size_t i = 0; i < test.size(); ++i)
	{
		auto it = hintFind(test.begin(), test.end(), i, test.begin() + 5);
		//if (i % 1000 == 0)
		std::cout << i << " -> " << *it << ": " << (i == *it ? " !" : " FAIL")  << std::endl;
	}
}
