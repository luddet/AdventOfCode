// https://adventofcode.com/2019/day/6

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

using orbit_map = std::unordered_map<std::string, std::string>;

orbit_map buildTree(std::vector<std::string>& input)
{
    std::unordered_map<std::string, std::string> objects;
    for (auto line : input)
    {   
        size_t delimIndex = line.find(')');
        objects.insert(std::make_pair(line.substr(delimIndex + 1), line.substr(0, delimIndex)));
    }

    return objects;
}

uint32_t orbitCount(const orbit_map& map)
{
    uint32_t count = 0;
    for (auto pair : map)
    {
        count++;
        auto currentPair = map.find(pair.second);
        while (currentPair != map.end())
        {
            count++;
            currentPair = map.find(currentPair->second);
        }
    }
    return count;
}

std::vector<std::string> getRootedPath(const std::string& startNode, const orbit_map& map)
{
	std::vector<std::string> path;
	auto currentNode = map.find(startNode);
	while (currentNode != map.end())
	{
		path.push_back(currentNode->first);
		auto nextNode = map.find(currentNode->second);
		
		if (nextNode == map.end()) // Found root node
			path.push_back(currentNode->second);

		currentNode = nextNode;
	}

	std::reverse(std::begin(path), std::end(path));

	return path;
}

int main()
{
    std::fstream fs("input.txt");
    std::string line;
    std::vector<std::string> input;
    while (std::getline(fs, line))
        input.push_back(line);

    orbit_map map = buildTree(input);
    uint32_t count = orbitCount(map);

	auto youPath = getRootedPath("YOU", map);
	auto sanPath = getRootedPath("SAN", map);

	// Find index of first common ancestor. We assume that there is always at least a common root node.
	uint32_t cai = 0;
	while (youPath[cai] == sanPath[cai])
		++cai;
	--cai;

	// subtract common path length and 1 for the common node and 1 for the node is, times 2 and for both paths
	uint32_t jumps = (youPath.size() + sanPath.size() - cai - cai - 4);

	std::cout << "Part 1: " << count << std::endl;
	std::cout << "Part 2: " << jumps << std::endl;

}
