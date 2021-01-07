// https://adventofcode.com/2019/day/1

#include <iostream>
#include <fstream>
#include <vector>

int calculateFuelReq(int mass)
{
	return (mass / 3) - 2;
}

int calculateFuelReqRecursive(int mass)
{
	int fuelReq = calculateFuelReq(mass);
	if (fuelReq <= 0)
		return 0;
	return fuelReq + calculateFuelReqRecursive(fuelReq);
}

void partOne(const std::vector<int>& input)
{
	int totalFuelReq = 0;
	for (int mass : input)
		totalFuelReq += calculateFuelReq(mass);
	std::cout << "Part 1: " << totalFuelReq << std::endl;
}

void partTwo(const std::vector<int>& input)
{
	int totalFuelReq = 0;
	for (int mass : input)
		totalFuelReq += calculateFuelReqRecursive(mass);
	std::cout << "Part 2: " << totalFuelReq << std::endl;
}

std::vector<int> readInput(const char* fileName)
{
	std::ifstream file(fileName);
	std::vector<int> input;
	int i;
	while (file >> i)
		input.push_back(i);
	return input;
}

int main()
{
	std::vector<int> input = readInput("input.txt");
	partOne(input);
	partTwo(input);
}


