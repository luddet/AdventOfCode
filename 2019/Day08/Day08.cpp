// https://adventofcode.com/2019/day/8
#include <iostream>
#include <fstream>
#include <vector>
#include <array>
#include <string>
#include <sstream>
//#include <cctype>

constexpr size_t WIDTH = 25;
constexpr size_t HEIGHT = 6;
constexpr size_t NUMPIXELS = WIDTH * HEIGHT;

struct Layer
{
	std::array<uint8_t, NUMPIXELS> pixels{ };
	std::array<uint32_t, 10> histogram{ };
};

std::string convertLayerToString(const Layer& layer, size_t width)
{
	std::stringstream out;
	for (size_t i = 0; i < layer.pixels.size(); )
	{
		out << (layer.pixels[i] == 0 ? " " : "#");
		++i;
		if (i % width == 0)
			out << std::endl;
	}
	return out.str();
}

int main()
{
	std::fstream fs("input.txt");
	std::vector<Layer> image;

	unsigned char pixelValue;
	size_t pixelIndex = 0;
	Layer* currentLayer = nullptr;
	while (fs >> pixelValue)
	{
		if (pixelIndex % NUMPIXELS == 0)
		{
			pixelIndex = 0;
			image.push_back(Layer());
			currentLayer = &image.back();
		}
		pixelValue -= '0'; // convert to actual value from ascii code value
		currentLayer->pixels[pixelIndex] = pixelValue;
		currentLayer->histogram[pixelValue]++;
		++pixelIndex;
	}

	// Find layer with fewest zeroes
	currentLayer = &image[0];
	for (size_t i=1; i < image.size(); ++i)
	{
		if (image[i].histogram[0] < currentLayer->histogram[0])
			currentLayer = &image[i];
	}

	uint32_t part1Result = currentLayer->histogram[1] * currentLayer->histogram[2];

	Layer compositeImage{};
	for (size_t pixelIndex = 0; pixelIndex < NUMPIXELS; ++pixelIndex)
	{
		for (auto& layer : image)
		{
			if (layer.pixels[pixelIndex] != 2) // Not transparent
			{
				compositeImage.pixels[pixelIndex] = layer.pixels[pixelIndex];
				break;
			}
		}
	}

	std::cout << "Part 1: " << part1Result << std::endl;
	std::cout << convertLayerToString(compositeImage, WIDTH);
	// AGUEB

}