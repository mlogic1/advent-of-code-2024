/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 02 - Red-Nosed Reports                           *    /.'.'.\
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

namespace Day02
{
	std::mutex coutMutex;

	void PartA(const StringVector& inputLines)
	{
		uint32_t result = 0;

		for(const std::string& line : inputLines)
		{
			bool safe = true;
			std::istringstream iss(line);
			std::vector<int32_t> numbers;
			int32_t num;
			while(iss >> num)
			{
				numbers.emplace_back(num);
			}

			bool increasing = true;
			if (numbers[1] < numbers[0])
			{
				increasing = false;
			}

			if (increasing)
			{
				for (int i = 0; i < numbers.size() - 1; ++i)
				{
					int current = numbers[i], next = numbers[i + 1];
					if (current >= next || std::abs(current - next) > 3)
					{
						safe = false;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < numbers.size() - 1; ++i)
				{
					int current = numbers[i], next = numbers[i + 1];
					if (current <= next || std::abs(current - next) > 3)
					{
						safe = false;
						break;
					}
				}
			}

			if (safe)
			{
				++result;
			}
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << result << std::endl;
	}

	void PartB(const StringVector& inputLines)
	{
		uint32_t result = 0;

		for(const std::string& line : inputLines)
		{
			bool safe = true;
			std::istringstream iss(line);
			std::vector<int32_t> numbers;
			int32_t num;
			while(iss >> num)
			{
				numbers.emplace_back(num);
			}

			bool increasing = true;
			bool error = false;
			if (numbers[1] < numbers[0])
			{
				increasing = false;
			}

			if (increasing)
			{
				for (int i = 0; i < numbers.size() - 1; ++i)
				{
					int current = numbers[i], next = numbers[i + 1];
					if (current >= next || std::abs(current - next) > 3)
					{
						if (!error)
						{
							error = true;
							continue;
						}
						safe = false;
						break;
					}
				}
			}
			else
			{
				for (int i = 0; i < numbers.size() - 1; ++i)
				{
					int current = numbers[i], next = numbers[i + 1];
					if (current <= next || std::abs(current - next) > 3)
					{
						if (!error)
						{
							error = true;
							continue;
						}
						safe = false;
						break;
					}
				}
			}

			if (safe)
			{
				++result;
			}
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << result << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input02.txt", inputLines))
		{
			throw std::runtime_error("[Day 02] Unable to open input file.");
		}

		std::thread aThread(PartA, inputLines);
		PartB(inputLines);
		aThread.join();
	}
}
