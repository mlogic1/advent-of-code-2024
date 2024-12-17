/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 11 - Plutonian Pebbles                           *    /.'.'.\
**                                                                              *   /'.''.'.\
**                                                                              *   ^^^[_]^^^
********************************************************************************/

#include "days.h"
#include "advent-common.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <list>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>

namespace Day11
{
	std::mutex coutMutex;

	int8_t NumDigits(uint64_t number)
	{
		return std::to_string(number).size();
	}

	std::pair<uint64_t, uint64_t> SplitNumber(uint64_t number)
	{
		const std::string numStr = std::to_string(number);
		size_t midIndex = numStr.size() / 2;
		std::string a = numStr.substr(0, midIndex);
		std::string b =  numStr.substr(midIndex, numStr.size() - 1);

		return std::make_pair(std::stoi(a), std::stoi(b));
	}

	void Blink(std::vector<uint64_t>& numbers)
	{
		for (std::vector<uint64_t>::iterator it = numbers.begin(); it != numbers.end(); ++it)
		{
			if (*it == 0)
			{
				*it = 1;
			}
			else if(NumDigits(*it) % 2 == 0)
			{
				auto pair = SplitNumber(*it);
				*it = pair.first;
				it = numbers.insert(it + 1, pair.second);
			}
			else
			{
				*it *= 2024;
			}
		}
	}

	void PartA(const std::vector<uint64_t>& numbers)
	{
		std::vector<uint64_t> nums = numbers;
		for(int i = 0; i < 25; ++i)
		{
			Blink(nums);
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << nums.size() << std::endl;
	}

	void PartB(const std::vector<uint64_t>& numbers)
	{
		std::unordered_map<uint64_t, uint64_t> occurances;

		for (uint64_t num : numbers)
		{
			++occurances[num];
		}

		uint64_t totalStones = 0;
		for (int i = 0; i < 75; ++i)
		{
			std::unordered_map<uint64_t, uint64_t> newOcuurances;
			for (auto kvp : occurances)
			{
				if (kvp.first == 0)
				{
					newOcuurances[1] += kvp.second;
				}
				else if(NumDigits(kvp.first) % 2 == 0)
				{
					auto pair = SplitNumber(kvp.first);
					newOcuurances[pair.first] += kvp.second;
					newOcuurances[pair.second] += kvp.second;
				}
				else
				{
					newOcuurances[kvp.first * 2024] += kvp.second;
				}
			}

			occurances = newOcuurances;
		}

		for (auto kvp : occurances)
		{
			totalStones += kvp.second;
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << totalStones << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input11.txt", inputLines))
		{
			throw std::runtime_error("[Day 11] Unable to open input file.");
		}

		std::vector<uint64_t> numbers = common::GetNumbersInString<uint64_t>(inputLines[0]);

		std::thread aThread(PartA, numbers);
		PartB(numbers);
		aThread.join();
	}
}
