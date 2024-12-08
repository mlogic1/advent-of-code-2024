/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 06 - Guard Gallivant                             *    /.'.'.\
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

namespace Day06
{
	std::mutex coutMutex;

	static std::pair<int32_t, int32_t> GetGuardPos(const std::vector<std::vector<char>>& grid)
	{
		for (int i = 0; i < grid.size(); ++i)
		{
			for (int j = 0; j < grid[i].size(); ++j)
			{
				if (grid[i][j] == '^')
				{
					return std::make_pair(i, j);
				}
			}
		}

		return std::make_pair(-1, -1);
	}

	static void RotateRight(std::pair<int32_t, int32_t>& dir)
	{
		// up to right
		if (dir.first == -1 && dir.second == 0)
		{
			dir.first = 0;
			dir.second = 1;
		}

		// right to down
		else if (dir.first == 0 && dir.second == 1)
		{
			dir.first = 1;
			dir.second = 0;
		}

		// down to left
		else if (dir.first == 1 && dir.second == 0)
		{
			dir.first = 0;
			dir.second = -1;
		}

		// left to up
		else if (dir.first == 0 && dir.second == -1)
		{
			dir.first = -1;
			dir.second = 0;
		}
	}

	static char GetNextBlock(const std::vector<std::vector<char>>& grid,
				const std::pair<int32_t, int32_t>& pos,
				const std::pair<int32_t, int32_t>& dir)
	{
		int32_t tx = pos.first + dir.first;
		int32_t ty = pos.second + dir.second;
		return grid[tx][ty];
	}

	static void StepForward(std::pair<int32_t, int32_t>& pos,
				const std::pair<int32_t, int32_t>& dir)
	{
		pos.first += dir.first;
		pos.second += dir.second;
	}

	void PartA(const std::vector<std::vector<char>>& grid)
	{
		std::pair<int32_t, int32_t> guardPos = GetGuardPos(grid);
		std::pair<int32_t, int32_t> guardDir{-1, 0};

		int32_t boundX = grid[0].size() - 1, boundY = grid.size() -1;

		std::set<std::pair<int32_t, int32_t>> uniqueSpots;

		while(1)
		{
			if (GetNextBlock(grid, guardPos, guardDir) == '#')
			{
				RotateRight(guardDir);
			}
			else
			{
 				StepForward(guardPos, guardDir);
				uniqueSpots.insert(guardPos);
			}

			if (guardPos.first <= 0 || guardPos.first >= boundX)
			{
				break;
			}

			if (guardPos.second <= 0 || guardPos.second >= boundY)
			{
				break;
			}
		}
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << uniqueSpots.size() << std::endl;
	}

	void PartB(const std::vector<std::vector<char>>& grid)
	{
		uint32_t possibleBlocks = 0;
		uint32_t maxSteps = grid[0].size() * grid.size();

		for (int i = 0; i < grid.size(); ++i)
		{
			for (int j = 0; j < grid[0].size(); ++j)
			{
				if (grid[i][j] == '.')
				{
					std::vector<std::vector<char>> customGrid = grid;
					// try placing an object on the spot
					customGrid[i][j] = '#';

					// if the guard takes more steps than there's cells, consider it locked
					std::pair<int32_t, int32_t> guardPos = GetGuardPos(customGrid);
					std::pair<int32_t, int32_t> guardDir{ -1, 0 };

					int32_t boundX = customGrid[0].size() - 1, boundY = customGrid.size() - 1;					

					uint32_t stepCounter = 0;

					while (1)
					{
						if (GetNextBlock(customGrid, guardPos, guardDir) == '#')
						{
							RotateRight(guardDir);
						}
						else
						{
							StepForward(guardPos, guardDir);
							++stepCounter;
						}

						if (guardPos.first <= 0 || guardPos.first >= boundX)
						{
							break;
						}

						if (guardPos.second <= 0 || guardPos.second >= boundY)
						{
							break;
						}

						if (stepCounter >= maxSteps)
						{
							++possibleBlocks;
							break;
						}
					}
				}
			}
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << possibleBlocks << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input06.txt", inputLines))
		{
			throw std::runtime_error("[Day 06] Unable to open input file.");
		}

		// convert to grid
		std::vector<std::vector<char>> grid;

		for(size_t i = 0; i < inputLines.size(); ++i)
		{
			int col = 0;
			std::vector<char> row;
			for(char c : inputLines[i])
			{
				row.push_back(c);
			}
			grid.push_back(std::move(row));
		}

		std::thread aThread(PartA, grid);
		PartB(grid);
		aThread.join();
	}
}
