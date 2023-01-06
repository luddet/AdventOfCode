#include <algorithm>
#include <cassert>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <cpr/cpr.h>

#include "utilities.h"


std::string downloadInput(unsigned int year, unsigned int day)
{
	auto sessionToken = getSessionCookie();
	std::ostringstream url;
	url << "https://adventofcode.com/" << year << "/day/" << day << "/input";
	cpr::Cookie sessionCookie("session", sessionToken, ".adventofcode.com", false, "/", true);
	cpr::Response r = cpr::Get(cpr::Url(url.str()), sessionCookie);
	if (r.status_code != 200)
	{
		std::ostringstream msg;
		msg << "Failed to download input from: " << url.str() << std::endl;
		msg << "Response: " << r.text;
		throw std::runtime_error(msg.str());
	}

	return r.text;
}

DLLEXPORT std::string getInput(unsigned int year, unsigned int day, const std::string& cacheFile)
{
	assert(!cacheFile.empty() && "Can't use an empty string as cachefile name.");
	
	std::string input{};

	if (!std::filesystem::exists(cacheFile))
	{
		input = downloadInput(year, day);
		std::ofstream ofs(cacheFile);
		ofs << input;
		return input;
	}
	else
	{
		input = readAllText(cacheFile);
	}
	return input;
}


DLLEXPORT std::string getEnvVar(const char* var)
{
	assert(var && "var pointer can't be null!");

	char* buffer = nullptr;
	FINALLY(free(buffer));

	size_t len;
	getenv_s(&len, nullptr, 0, var);
	if (len == 0)
		throw env_error(std::string("Failed to find env var: ") + std::string(var));

	buffer = (char*)malloc(len);
	if (buffer == nullptr)
		throw std::bad_alloc();

	getenv_s(&len, buffer, len, var);
	if (len < 2) // null terminated, so size == 1 if empty
		throw env_error("Env var was empty, this is highly unusual!: " + std::string(var));

	std::string str(buffer);
	return str;
}


DLLEXPORT std::string getSessionCookie(const std::string& envVar)
{
	auto cookiePath = getEnvVar(envVar.c_str());
	std::ifstream ifs(cookiePath);
	if (!ifs)
	{
		std::ostringstream msg;
		msg << "Can't open cookie file:" << cookiePath;
		throw std::runtime_error(msg.str());
	}

	if (std::string cookie;	std::getline(ifs, cookie) && !cookie.empty())
		return cookie;
	else
	{
		std::ostringstream msg;
		msg << "Cookie file was empty: " << cookiePath;
		throw std::runtime_error(msg.str());
	}
}

std::vector<std::string> split(const std::string& str, const std::string& delims)
{
	std::vector<std::string> result{};
	size_t first{ 0 }, end;
	do
	{
		end = min(str.find_first_of(delims, first), str.length());
		if (end - first > 1)
			result.emplace_back(str.substr(first, end - first));
		
		first = end + 1;
	} while (first < str.length());

	return result;
}

DLLEXPORT std::string readAllText(std::istream& is)
{
	return std::string(std::istreambuf_iterator<char>{is}, {});
}

DLLEXPORT std::string readAllText(const std::string& fileName)
{
	std::ifstream ifs(fileName);
	return readAllText(ifs);
}

DLLEXPORT std::vector<int> readInts(std::istream& os)
{
	std::vector<int> result;
	while (!os.eof())
	{
		int i;
		os >> i;
		result.push_back(i);
	}
	return result;
}

DLLEXPORT std::vector<std::string> readLines(const std::string& filePath)
{
	return readLines(std::ifstream(filePath));
}

DLLEXPORT std::vector<std::string> readLines(std::istream& is)
{
	std::vector<std::string> result;
	for (std::string line; std::getline(is, line);)
		result.push_back(line);
	return result;

}

DLLEXPORT std::vector<std::string> readLines(std::istream&& is)
{
	return readLines(is);
}