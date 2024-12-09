/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 07 - Bridge Repair                               *    /.'.'.\
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

namespace Day07
{
	std::mutex coutMutex;

	static void ComputeCombinationA(const std::vector<uint64_t>& numbers, int index, uint64_t current, std::set<uint64_t>& results)
	{
		if (index == numbers.size())
		{
			results.insert(current);
			return;
		}

		// Add
		ComputeCombinationA(numbers, index + 1, current + numbers[index], results);

		// Mul
		ComputeCombinationA(numbers, index + 1, current * numbers[index], results);
	}

	static bool CheckMatchA(const std::vector<uint64_t>& numbers, uint64_t target)
	{
		std::set<uint64_t> results;
		ComputeCombinationA(numbers, 1, numbers[0], results);

		return results.find(target) != results.end();
	}

	static void ComputeCombinationB(const std::vector<uint64_t>& numbers, int index, uint64_t current, std::set<uint64_t>& results)
	{
		if (index == numbers.size())
		{
			results.insert(current);
			return;
		}

		// Add
		ComputeCombinationB(numbers, index + 1, current + numbers[index], results);

		// Mul
		ComputeCombinationB(numbers, index + 1, current * numbers[index], results);

		// Concat
		std::string strResult = std::to_string(current) + std::to_string(numbers[index]);
		ComputeCombinationB(numbers, index + 1, std::stoull(strResult), results);
	}

	static bool CheckMatchB(const std::vector<uint64_t>& numbers, uint64_t target)
	{
		std::set<uint64_t> results;
		ComputeCombinationB(numbers, 1, numbers[0], results);

		return results.find(target) != results.end();
	}

	void PartA(const StringVector& inputLines)
	{
		uint64_t totalSum = 0;
		for (const std::string& line : inputLines)
		{
			const StringVector split = common::SplitStringWithDelimiter(line, ':');

			uint64_t target = std::stoull(split[0]);
			std::vector<uint64_t> numbers;
			std::stringstream ss(split[1]);
			
			uint64_t temp;
			while(ss >> temp)
			{
				numbers.emplace_back(temp);
			}

			if (CheckMatchA(numbers, target))
			{
				totalSum += target;
			}
		}
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << totalSum << std::endl;
	}

	void PartB(const StringVector& inputLines)
	{
		uint64_t totalSum = 0;
		for (const std::string& line : inputLines)
		{
			const StringVector split = common::SplitStringWithDelimiter(line, ':');

			uint64_t target = std::stoull(split[0]);
			std::vector<uint64_t> numbers;
			std::stringstream ss(split[1]);
			
			uint64_t temp;
			while(ss >> temp)
			{
				numbers.emplace_back(temp);
			}

			if (CheckMatchB(numbers, target))
			{
				totalSum += target;
			}
		}
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << totalSum << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input07.txt", inputLines))
		{
			throw std::runtime_error("[Day 07] Unable to open input file.");
		}

		std::thread aThread(PartA, inputLines);
		PartB(inputLines);
		aThread.join();
	}
}
