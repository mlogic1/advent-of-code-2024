/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 05 - Print Queue                                 *    /.'.'.\
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

namespace Day05
{
	std::mutex coutMutex;

	static bool containsBothPages(const std::pair<int32_t, int32_t>& order, const std::vector<int32_t>& update)
	{
		return (std::count(update.cbegin(), update.cend(), order.first) > 0) && (std::count(update.cbegin(), update.cend(), order.second) > 0);
	}

	static int indexOfValue(const std::vector<int32_t>& vec, int32_t val)
	{
		auto it = std::find(vec.cbegin(), vec.cend(), val);

		if (it != vec.cend())
		{
			return std::distance(vec.begin(), it);
		}

		return -1;
	}

	void PartA(const std::vector<std::pair<int32_t, int32_t>>& orders, const std::vector<std::vector<int32_t>>& updates)
	{
		int32_t result = 0;
		for (const std::vector<int32_t>& update : updates)
		{
			bool isCorrect = true;
			// check if the updatelist belongs to any order. If it does, then validate if the update is correct
			for (const std::pair<int32_t, int32_t>& order : orders)
			{
				if (containsBothPages(order, update))
				{
					// update contains both pages, it must be validated
					int indexFirst = indexOfValue(update, order.first);
					int indexSecond = indexOfValue(update, order.second);

					isCorrect = indexSecond > indexFirst;
				}

				if (!isCorrect)
				{
					break; // skip this entirely, this is not a valid update list
				}
			}

			if (isCorrect)
			{
				result += update.at(update.size() / 2);
			}
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << result << std::endl;
	}

	void PartB(const std::vector<std::pair<int32_t, int32_t>>& orders, const std::vector<std::vector<int32_t>>& updates)
	{
		std::vector<std::vector<int32_t>> ucopy = updates;
		int32_t result = 0;
		for (std::vector<int32_t>& update : ucopy)
		{
			bool isCorrect = false;
			bool isCorrected = false;

			while(!isCorrect)	// keep swapping elements until the list is fully corrected
			{
				// check if the updatelist belongs to any order. If it does, then validate if the update is correct
				for (const std::pair<int32_t, int32_t>& order : orders)
				{
					if (containsBothPages(order, update))
					{
						// update contains both pages, it must be validated
						int indexFirst = indexOfValue(update, order.first);
						int indexSecond = indexOfValue(update, order.second);

						isCorrect = indexSecond > indexFirst;

						if (!isCorrect)
						{
							// swap 1 and 2
							std::swap(update[indexFirst], update[indexSecond]);
							isCorrected = true;
							break; // start from the top
						}
					}
				}
			}

			if (isCorrected)
			{
				result += update.at(update.size() / 2);
			}
		}
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << result << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input05.txt", inputLines))
		{
			throw std::runtime_error("[Day 05] Unable to open input file.");
		}

		std::vector<std::pair<int32_t, int32_t>> pageOrders;
		std::vector<std::vector<int32_t>> pageUpdates;

		// load the page ordering rules
		size_t index = 0;
		for(;;)
		{
			const std::string& line = inputLines.at(index);

			auto nums = common::SplitStringWithDelimiter(line, '|');
			++index;

			if (line.empty())
			{
				break;
			}
			else
			{
				pageOrders.emplace_back(std::make_pair(std::stoi(nums[0]), std::stoi(nums[1])));
			}
		}

		// load the page updates
		for (; index < inputLines.size(); ++index)
		{
			const std::string& line = inputLines.at(index);

			StringVector strNums = common::SplitStringWithDelimiter(line, ',');

			std::vector<int32_t> nums;
			for (const std::string& strNum : strNums)
			{
				nums.emplace_back(std::stoi(strNum));
			}
			pageUpdates.push_back(std::move(nums));
		}

		std::thread aThread(PartA, pageOrders, pageUpdates);
		PartB(pageOrders, pageUpdates);
		aThread.join();
	}
}
