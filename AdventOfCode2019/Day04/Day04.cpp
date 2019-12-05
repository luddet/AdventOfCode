// https://adventofcode.com/2019/day/4
// Input range 178416-676461

#include <iostream>
#include <array>

template<uint8_t size>
uint32_t convertToInt(std::array<uint8_t, size>& intArray)
{
	uint32_t result = 0;
	for (size_t i = 0; i < size; ++i)
	{
		result *= 10;
		result += intArray[i];
	}
	return result;
}

template<uint8_t size>
void convertToArray(uint32_t num, std::array<uint8_t, size>& array)
{
	for (size_t i = 0; i < size; ++i)
	{
		array[size - i - 1] = num % 10;
		num /= 10;
	}
}

template<uint8_t size>
void increment(std::array<uint8_t, size>& intArray)
{
	for (size_t i = size; i > 0; --i)
	{
		intArray[i - 1] = (intArray[i - 1] + 1) % 10;
		if ( intArray[i - 1] != 0)
			return;
	}
}


template<uint8_t size>
void makeNondecreasing(std::array<uint8_t, size>& intArray)
{
	uint8_t currentMax = 0;
	for (size_t i = 0; i < size; ++i)
	{
		if (intArray[i] < currentMax)
			intArray[i] = currentMax;
		else
			currentMax = intArray[i];
	}
}

template<uint8_t size>
std::ostream& operator<< (std::ostream& os, const std::array<uint8_t, size>& array)
{
	for (size_t i = 0; i < size; ++i)
		os << (int)array[i];
	return os;
}

template<uint8_t size>
bool atLeastDoubleDigits(std::array<uint8_t, size>& intArray)
{
	for (size_t i = 0; i < size - 1; ++i)
	{
		if (intArray[i] == intArray[i + 1])
		{
			return true;
		}
	}
	return false;
}

template<uint8_t size>
bool exactlyDoubleDigits(std::array<uint8_t, size>& intArray)
{
	uint8_t currentGroup = intArray[0];
	uint8_t currentGroupCount = 1;
	for (size_t i = 1; i < size; ++i)
	{
		if (intArray[i] == currentGroup)
			++currentGroupCount;
		else
		{
			if (currentGroupCount == 2)
				break;
			currentGroup = intArray[i];
			currentGroupCount = 1;
		}
	}
	return currentGroupCount == 2;
}


int main()
{
	constexpr uint32_t startNum = 178416;
	constexpr uint32_t endNum = 676461;
	
	std::array<uint8_t, 6> password;
	convertToArray(startNum, password);
	makeNondecreasing(password);

	uint32_t numMatchingPart1 = 0;
	uint32_t numMatchingPart2 = 0;
	uint32_t numChecked = 0;
	while (uint32_t currentPass = convertToInt(password) <= endNum)
	{
		// Part 1 criteria
		if (atLeastDoubleDigits(password))
			++numMatchingPart1;

		// Part 2 criteria
		if (exactlyDoubleDigits(password))
			++numMatchingPart2;

		numChecked++;
		increment(password);
		makeNondecreasing(password);
	}

	std::cout << "numChecked: " << numChecked << std::endl;
	std::cout << "Part 1: " << numMatchingPart1 << std::endl;
	std::cout << "Part 2: " << numMatchingPart2 << std::endl;

}
