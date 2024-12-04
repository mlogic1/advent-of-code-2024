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
		std::regex mulRegex(R"(\bmul\(\d+,\d+\))");
		std::smatch regexMatch;
		if (std::regex_search(inputLines[0], regexMatch, mulRegex))
		{
			auto a = regexMatch[1].str();
			auto b = regexMatch[2].str();
		}


		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << 1335 << std::endl;
	}

	void PartB(const StringVector& inputLines)
	{


		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << 1336 << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/sample.txt", inputLines))
		{
			throw std::runtime_error("[Day 03] Unable to open input file.");
		}

		std::thread aThread(PartA, inputLines);
		PartB(inputLines);
		aThread.join();
	}
}
