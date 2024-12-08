/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 03 - Mull It Over                                *    /.'.'.\
**                                                                              *   /'.''.'.\
**                                                                              *   ^^^[_]^^^
********************************************************************************/

#include "days.h"
#include "advent-common.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>
#include <regex>

namespace Day03
{
	std::mutex coutMutex;

	void PartA(const StringVector& inputLines)
	{
		uint32_t total = 0;

		for (const std::string& line : inputLines)
		{
			std::regex mulRegex(R"(mul\(\d+,\d+\))");
			std::smatch regexMatch;

			std::string::const_iterator searchStart(line.cbegin());

			while (std::regex_search(searchStart, line.cend(), regexMatch, mulRegex))
			{
				string matched = regexMatch[0];
				std::regex numRegex(R"(\d+)");
				std::sregex_iterator numBegin(matched.begin(), matched.end(), numRegex);

				std::sregex_iterator numEnd;
				int num1 = stoi(numBegin->str());
				++numBegin;
				int num2 = stoi(numBegin->str());
				int z = 1;
				searchStart = regexMatch.suffix().first;
				total += (num1 * num2);
			}
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << total << std::endl;
	}

	void PartB(const StringVector& inputLines)
	{
		uint32_t total = 0;

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << total << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/sample_day3.txt", inputLines))
		{
			throw std::runtime_error("[Day 03] Unable to open input file.");
		}

		std::thread aThread(PartA, inputLines);
		PartB(inputLines);
		aThread.join();
	}
}
