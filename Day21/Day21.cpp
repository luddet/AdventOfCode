#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <map>
#include <string>
#include <algorithm>
#include <stack>
#include <exception>

const char* EXAMPLE =
"mxmxvkd kfcds sqjhc nhms (contains dairy, fish)\n"
"trh fvjkl sbzzf mxmxvkd (contains dairy)\n"
"sqjhc fvjkl (contains soy)\n"
"sqjhc mxmxvkd sbzzf (contains fish)\n";


int main()
{
	//std::istringstream is(EXAMPLE);
	std::ifstream is("input.txt");

	std::vector<std::set<std::string>> foods;
	std::map<std::string, std::set<std::string>> allergens;

	std::string line;
	while (std::getline(is, line) && !line.empty())
	{
		const std::string divider = " (contains ";
		auto i = line.find(divider);
		std::istringstream ingredients(line.substr(0, i));
		std::istringstream allergenStream(line.substr(i + divider.length(), line.length() - i - divider.length() - 1));
		
		foods.push_back(std::set<std::string>(std::istream_iterator<std::string>(ingredients), std::istream_iterator<std::string>()) );
		
		while (std::getline(allergenStream, line, ','))
		{
			auto start = line.find_first_not_of(' ');
			line = line.substr(start, line.find_last_not_of(' ') - start + 1);
			auto aIt = allergens.find(line);
			if (aIt == allergens.end())
				allergens[line] = std::set<std::string>(foods.back().begin(), foods.back().end());
			else
			{
				std::set<std::string> temp;
				std::set_intersection(foods.back().begin(), foods.back().end(), aIt->second.begin(), aIt->second.end(),
					std::inserter(temp, temp.begin()));
				aIt->second = temp;
			}
		}
	}

	std::stack<std::pair<std::string, std::string>> unambiguous;
	for (auto& [a, ingredients] : allergens)
		if (ingredients.size() == 1)
			unambiguous.push({ a, *ingredients.begin() });

	while (!unambiguous.empty())
	{
		auto [currentAllergen, currentIngredient] = unambiguous.top();
		unambiguous.pop();
		for (auto& [allergen, ingredients] : allergens)
		{
			if (allergen == currentAllergen)
				continue;
			if (ingredients.size() > 1)
			{
				ingredients.erase(currentIngredient);
				if (ingredients.size() == 1)
					unambiguous.push({ allergen, *ingredients.begin() });
			}
		}
	}

	std::ostringstream ss;
	for (auto& [a, i] : allergens)
	{
		if (i.size() != 1)
			throw std::exception("Fail!");

		std::cout << "Allergen: " << a << ", Ingredient: " << *i.begin() << std::endl;
		ss << *i.begin() << ",";
	}
	auto sortedIngredients = ss.str().substr(0, ss.str().length() - 1);

	std::set<std::string> knownIngredients;
	for (auto& [a, i] : allergens)
		knownIngredients.insert(*i.begin());

	// count unkown ingredients
	uint64_t part1{ 0 };
	for (auto& food : foods)
		for (auto& i : food)
			if (knownIngredients.find(i) == knownIngredients.end())
				++part1;


	std::cout << "Day21 Part 1: " << part1 << std::endl;
	std::cout << "Day21 Part 2: " << sortedIngredients << std::endl;
}

