/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 12 - Garden Groups                               *    /.'.'.\
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

namespace Day12
{
	std::mutex coutMutex;

	struct Region
	{
		const char RegionId;
		std::vector<std::pair<uint32_t, uint32_t>> Plots;
		std::vector<std::pair<uint32_t, uint32_t>> BoundryPlots;

		uint64_t Area() const
		{
			return Plots.size();
		}

		uint64_t Perimiter() const
		{
			uint64_t perimiter = 0;
			for (auto kvp : Plots)
			{
				uint32_t i = kvp.first, j = kvp.second;
				std::pair<uint32_t, uint32_t> left{i, j - 1}, right{i, j + 1}, up{i - 1, j}, down{i + 1, j};

				if (std::count(Plots.begin(), Plots.end(), left) == 0)
				{
					++perimiter;
				}
				if (std::count(Plots.begin(), Plots.end(), right) == 0)
				{
					++perimiter;
				}
				if (std::count(Plots.begin(), Plots.end(), up) == 0)
				{
					++perimiter;
				}
				if (std::count(Plots.begin(), Plots.end(), down) == 0)
				{
					++perimiter;
				}
			}

			return perimiter;
		}

		uint64_t Sides() const
		{
			// TODO: this function is incomplete
			uint64_t sides = 0;

			std::set<std::pair<uint32_t, uint32_t>> perimiterPlots;

			// Get all perimiter plots
			for (auto kvp : Plots)
			{
				uint32_t i = kvp.first, j = kvp.second;
				std::pair<uint32_t, uint32_t> left{i, j - 1}, right{i, j + 1}, up{i - 1, j}, down{i + 1, j};

				if (std::count(Plots.begin(), Plots.end(), left) == 0)
				{
					perimiterPlots.insert(kvp);
				}
				if (std::count(Plots.begin(), Plots.end(), right) == 0)
				{
					perimiterPlots.insert(kvp);
				}
				if (std::count(Plots.begin(), Plots.end(), up) == 0)
				{
					perimiterPlots.insert(kvp);
				}
				if (std::count(Plots.begin(), Plots.end(), down) == 0)
				{
					perimiterPlots.insert(kvp);
				}
			}

			const uint64_t totalWalls = perimiterPlots.size() * 4;
			uint64_t sharedWalls = 0;

			for(auto it = perimiterPlots.begin(); it != std::prev(perimiterPlots.end()); it++)
			{
				std::pair<uint32_t, uint32_t> kvp = *it;
				std::pair<uint32_t, uint32_t> next = *std::next(it);

				// do they share a wall?
				if (kvp.first == next.first)
				{
					if (std::abs((int)kvp.second - (int)next.second) == 1)
					{
						// shared wall
						++sharedWalls;
						continue;
					}
				}

				if (kvp.second == next.second)
				{
					if (std::abs((int)kvp.first - (int)next.first) == 1)
					{
						// shared wall
						++sharedWalls;
						continue;
					}
				}
			}

			return sides;
		}
	};

	static void Step(Region& region, const std::vector<std::vector<char>>& grid, uint32_t i, uint32_t j, std::vector<std::pair<uint32_t, uint32_t>>& processedPlots, const uint32_t irows, const uint32_t icols)
	{
		char current = grid[i][j];

		if (current == region.RegionId)
		{
			region.Plots.emplace_back(i, j);
			processedPlots.emplace_back(i, j);
		}
		else
		{
			return;
		}

		if (i <= 0)	// Don't check above
		{

		}
		else
		{
			if (grid[i - 1][j] == current)
			{
				if (std::count(processedPlots.begin(), processedPlots.end(), std::make_pair(i - 1, j)) == 0)
				{
					Step(region, grid, i - 1, j, processedPlots, irows, icols);
				}
			}
		}

		if (j <= 0) // Don't check left
		{

		}
		else
		{
			if (grid[i][j - 1] == current)
			{
				if (std::count(processedPlots.begin(), processedPlots.end(), std::make_pair(i, j - 1)) == 0)
				{
					Step(region, grid, i, j - 1, processedPlots, irows, icols);
				}
			}
		}

		if (j >= icols)	// Don't check right
		{

		}
		else
		{
			if (grid[i][j + 1] == current)
			{
				if (std::count(processedPlots.begin(), processedPlots.end(), std::make_pair(i, j + 1)) == 0)
				{
					Step(region, grid, i, j + 1, processedPlots, irows, icols);
				}
			}
		}

		if (i >= irows)	// Don't check below
		{

		}
		else
		{
			if (grid[i + 1][j] == current)
			{
				if (std::count(processedPlots.begin(), processedPlots.end(), std::make_pair(i + 1, j)) == 0)
				{
					Step(region, grid, i + 1, j, processedPlots, irows, icols);
				}
			}
		}
	}

	static std::vector<Region> DetectRegions(const std::vector<std::vector<char>>& grid)
	{
		std::vector<std::pair<uint32_t, uint32_t>> ProcessedPlots;
		std::vector<Region> regions;

		int32_t rows = grid.size();
		int32_t columns = grid[0].size();

		for (uint32_t i = 0; i < rows; ++i)
		{
			for (uint32_t j = 0; j < rows; ++j)
			{
				if (std::count(ProcessedPlots.begin(), ProcessedPlots.end(), std::make_pair(i, j)) == 0)
				{
					// Plot does not yet belong to a region
					char regionId = grid[i][j];
					Region reg{ .RegionId = regionId };
					Step(reg, grid, i, j, ProcessedPlots, rows - 1, columns - 1);
					regions.emplace_back(std::move(reg));
				}
				// else Skip
			}
		}

		return regions;
	}

	void PartA(const std::vector<Region>& regs)
	{
		uint64_t totalPrice = 0;
		for (const Region& region : regs)
		{
			uint64_t price = region.Area() * region.Perimiter();
			totalPrice += price;
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << totalPrice << std::endl;
	}

	void PartB(const std::vector<Region>& regs)
	{
		uint64_t totalPrice = 0;
		for (const Region& region : regs)
		{
			uint64_t sides = region.Sides();
			uint64_t price = region.Area() * region.Perimiter();
			totalPrice += price;
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << 1335 << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/sample.txt", inputLines))
		{
			throw std::runtime_error("[Day 12] Unable to open input file.");
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

		std::vector<Region> regs = DetectRegions(grid);

		std::thread aThread(PartA, regs);
		// PartB(regs); // Part B is not implemented correctly
		aThread.join();
	}
}
