/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 10 - Hoof It                                     *    /.'.'.\
**                                                                              *   /'.''.'.\
**                                                                              *   ^^^[_]^^^
********************************************************************************/

#include "days.h"
#include "advent-common.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <thread>
#include <mutex>

namespace Day10
{
	std::mutex coutMutex;

	using IntPairSet = std::set<std::pair<int32_t, int32_t>>;
	using RatingMap = std::map<std::pair<int32_t, int32_t>, int>;

	void RatedPathFind(const std::vector<std::vector<int8_t>>& grid, RatingMap& ratingMap, int32_t i, int32_t j, const std::pair<int32_t, int32_t>& startPos)
	{
		const int32_t boundX = grid[0].size() - 1;
		const int32_t boundY = grid.size() - 1;
		int8_t current = grid[i][j];
		if (current == 9)
		{
			ratingMap[startPos]++;
			return;
		}
		int8_t target = current + 1;

		// Check left, right, up and below for the target

		// Left
		if (j > 0)
		{
			if (grid[i][j-1] == target)
			{
				RatedPathFind(grid, ratingMap, i, j-1, startPos);
			}
		}

		// Right
		if (j < boundX)
		{
			if (grid[i][j+1] == target)
			{
				RatedPathFind(grid, ratingMap, i, j+1, startPos);
			}
		}

		// Up
		if (i > 0)
		{
			if (grid[i-1][j] == target)
			{
				RatedPathFind(grid, ratingMap, i-1, j, startPos);
			}
		}

		// Down
		if (i < boundY)
		{
			if (grid[i+1][j] == target)
			{
				RatedPathFind(grid, ratingMap, i+1, j, startPos);
			}
		}
	}

	void PathFind(const std::vector<std::vector<int8_t>>& grid, IntPairSet& ninesReached, int32_t i, int32_t j)
	{
		const int32_t boundX = grid[0].size() - 1;
		const int32_t boundY = grid.size() - 1;
		int8_t current = grid[i][j];
		if (current == 9)
		{
			ninesReached.insert(std::make_pair(i, j));
			return;
		}
		int8_t target = current + 1;

		// Check left, right, up and below for the target

		// Left
		if (j > 0)
		{
			if (grid[i][j-1] == target)
			{
				PathFind(grid, ninesReached, i, j-1);
			}
		}

		// Right
		if (j < boundX)
		{
			if (grid[i][j+1] == target)
			{
				PathFind(grid, ninesReached, i, j+1);
			}
		}

		// Up
		if (i > 0)
		{
			if (grid[i-1][j] == target)
			{
				PathFind(grid, ninesReached, i-1, j);
			}
		}

		// Down
		if (i < boundY)
		{
			if (grid[i+1][j] == target)
			{
				PathFind(grid, ninesReached, i+1, j);
			}
		}
	}

	void PartA(const std::vector<std::vector<int8_t>>& grid)
	{
		IntPairSet startPositions;
		uint32_t totalScore = 0;
		for (int i = 0; i < grid.size(); ++i)
		{
			for (int j = 0; j < grid[0].size(); ++j)
			{
				if (grid[i][j] == 0)
				{
					startPositions.insert(std::make_pair(i, j));
				}
			}
		}

		for (auto kvp : startPositions)
		{
			IntPairSet ninesReached;
			PathFind(grid, ninesReached, kvp.first, kvp.second);
			totalScore += ninesReached.size();
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << totalScore << std::endl;
	}

	void PartB(const std::vector<std::vector<int8_t>>& grid)
	{
		IntPairSet startPositions;
		uint32_t totalScore = 0;
		for (int i = 0; i < grid.size(); ++i)
		{
			for (int j = 0; j < grid[0].size(); ++j)
			{
				if (grid[i][j] == 0)
				{
					startPositions.insert(std::make_pair(i, j));
				}
			}
		}

		for (auto kvp : startPositions)
		{
			RatingMap ratingMap;
			RatedPathFind(grid, ratingMap, kvp.first, kvp.second, kvp);
			for (auto mkvp : ratingMap)
			{
				auto k = mkvp.first;
				auto v = mkvp.second;
				int x = 1;
				totalScore += mkvp.second;
			}
		}
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << totalScore << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input10.txt", inputLines))
		{
			throw std::runtime_error("[Day 10] Unable to open input file.");
		}

		// convert to grid
		std::vector<std::vector<int8_t>> grid;

		for(size_t i = 0; i < inputLines.size(); ++i)
		{
			int col = 0;
			std::vector<int8_t> row;
			for(char c : inputLines[i])
			{
				row.push_back(c - '0');
			}
			grid.push_back(std::move(row));
		}

		std::thread aThread(PartA, grid);
		PartB(grid);
		aThread.join();
	}
}
