/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 01 - Historian Hysteria                          *    /.'.'.\
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

namespace Day01
{
	void PartA(const StringVector& inputLines)
	{
		uint32_t result = 0;
		std::vector<int32_t> leftList;
		std::vector<int32_t> rightList;

		for(const std::string& line : inputLines)
		{
			std::istringstream iss(line);
			int a, b;
			iss >> a >> b;
			leftList.emplace_back(a);
			rightList.emplace_back(b);
		}

		std::sort(leftList.begin(), leftList.end());
		std::sort(rightList.begin(), rightList.end());

		const size_t numElems = leftList.size();
		for (size_t i = 0; i < numElems; ++i)
		{
			const uint32_t distance = std::abs(leftList[i] - rightList[i]);
			result += distance;
		}

		std::cout << "Part A: " << result << std::endl;
	}

	void PartB(const StringVector& inputLines)
	{
		uint32_t result = 0;
		std::vector<int32_t> leftList;
		std::vector<int32_t> rightList;

		for(const std::string& line : inputLines)
		{
			std::istringstream iss(line);
			int a, b;
			iss >> a >> b;
			leftList.emplace_back(a);
			rightList.emplace_back(b);
		}

		for (int32_t num : leftList)
		{
			result += (num * std::count(rightList.cbegin(), rightList.cend(), num));
		}

		std::cout << "Part B: " << result << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input01.txt", inputLines))
		{
			throw std::runtime_error("[Day 01] Unable to open input file.");
		}

		std::thread aThread(PartA, inputLines);
		PartB(inputLines);
		aThread.join();
	}
}
