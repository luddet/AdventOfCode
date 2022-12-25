#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <memory>
#include <ranges>
#include <regex>


#include "../../2021/Utilities/utilities.h"

static const std::string testInput(
R"($ cd /
$ ls
dir a
14848514 b.txt
8504156 c.dat
dir d
$ cd a
$ ls
dir e
29116 f
2557 g
62596 h.lst
$ cd e
$ ls
584 i
$ cd ..
$ cd ..
$ cd d
$ ls
4060174 j
8033020 d.log
5626152 d.ext
7214296 k
)");

class Entry
{
public:
	Entry(const std::string& name): m_name(name) {}
	std::string getName() const { return m_name; }
	virtual size_t getSize() const = 0;
	virtual ~Entry() = default;
private:
	const std::string m_name;
};


class File : public Entry
{
public:
	File(const std::string& name, int size): Entry(name), m_size(size){}
	virtual size_t getSize() const override { return m_size; }
private:
	const size_t m_size;
};

class Directory : public Entry
{
public:
	Directory(const std::string& name): Entry(name) {} 
	virtual size_t getSize() const override
	{
		return std::accumulate(m_children.cbegin(), m_children.cend(), 0ull,
								[](const auto acc, const auto& childEntry) { return acc + childEntry->getSize(); });
	}
	void addChild(std::shared_ptr<Entry> entry) { m_children.emplace_back(entry); }

	template<class Type>
	std::shared_ptr<Type> getChild(const std::string_view name) 
	{
		return std::dynamic_pointer_cast<Type>(*(std::find_if(m_children.begin(), m_children.end(), [&](auto& child) { return child->getName() == name; })));
	}

	const std::vector<std::shared_ptr<Entry>>& getChildren() const { return m_children; }
private:
	std::vector<std::shared_ptr<Entry>> m_children{};
};


template<class Type, class Callable>
void forEach(const Type& dir, Callable&& fun)
{
	std::for_each(cbegin(dir.getChildren()), cend(dir.getChildren()), [&](const auto& child)
	{
		if (auto dirPtr = std::dynamic_pointer_cast<Type>(child); dirPtr != nullptr)
		{
			forEach(*dirPtr, fun);
			fun(*dirPtr);
		}
	});
}

int main()
{
	std::string input{ getInput(2022, 7) };

	std::regex commandRe(R"(^\$ (\w+)(?: (.+))?$)");
	std::regex listingRe(R"(^(?:dir (\w+))|(?:(\d+) ([\w.]+))$)");

	// Build directory tree
	std::shared_ptr<Directory> cwd;
	std::vector<std::shared_ptr<Directory>> path;
	auto root = std::make_shared<Directory>("/");
	path.emplace_back(root);

	for (const auto& line :  split(input))
	{
		std::smatch match;
		if (std::regex_match(line, match, commandRe))
		{
			if (match[1] == "cd")
			{
				if (match[2] == "/")
					path.resize(1);
				else if (match[2] == "..")
					path.pop_back();
				else
					path.push_back(path.back()->getChild<Directory>(match[2].str()));

				cwd = path.back();
			}
		}
		else 
		{
			std::regex_match(line, match, listingRe);

			if (match[1].matched)
				cwd->addChild(std::make_shared<Directory>(match[1]));
			else
				cwd->addChild(std::make_shared<File>(match[3], std::stoi(match[2])));
		}
	}


	std::vector<size_t> sizes;
	size_t part1{ 0 };
	forEach(*root, [&](const auto& dir)
	{
		auto size = dir.getSize();
		sizes.push_back(size);
		if (size <= 100000)
			part1 += size;
	});

	std::sort(sizes.begin(), sizes.end());

	constexpr size_t totalDiskSpace{ 70000000 };
	constexpr size_t unusedNeeded{ 30000000 };
	const size_t totalUsed{ root->getSize() };
	const auto sizeToDelete{ totalUsed - (totalDiskSpace - unusedNeeded)};

	auto part2 = std::ranges::find_if(sizes, [sizeToDelete](const auto s) {return s > sizeToDelete; });


	std::cout << "Day07 Part 1: " << part1 << '\n';
	std::cout << "Day07 Part 2: " << *part2 << '\n';
}
