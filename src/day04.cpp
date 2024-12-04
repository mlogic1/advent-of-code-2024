/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 04 - Ceres Search                                *    /.'.'.\
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

namespace Day04
{
	std::mutex coutMutex;

	// Directions array to traverse all directions
	const std::vector<std::pair<int, int>> directions = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};


	static bool searchFromPos(const vector<vector<char>>& grid, const string& word, int row, int col, int dir) {
		int rows = grid.size();
		int cols = grid[0].size();
		int len = word.length();

		for (int k = 0; k < len; ++k) {
			int newRow = row + k * directions[dir].first;
			int newCol = col + k * directions[dir].second;

			if (newRow < 0 || newRow >= rows || newCol < 0 || newCol >= cols || grid[newRow][newCol] != word[k]) {
				return false;
			}
		}
		return true;
	}

	static int countOccurrences(const vector<vector<char>>& grid, const string& word) {
		int rows = grid.size();
		int cols = grid[0].size();
		int count = 0;

		for (int i = 0; i < rows; ++i) {
			for (int j = 0; j < cols; ++j) {
				if (grid[i][j] == word[0]) {
					for (int dir = 0; dir < 8; ++dir) {
						if (searchFromPos(grid, word, i, j, dir)) {
							count++;
						}
					}
				}
			}
		}
		return count;
	}

	void PartA(const std::vector<std::vector<char>>& grid)
	{
		const std::string lookupWord = "XMAS";
		int res = countOccurrences(grid, lookupWord);

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << res << std::endl;
	}

	void PartB(const std::vector<std::vector<char>>& grid)
	{
		int count = 0;
		for (int i = 1; i < grid.size() - 1; ++i)
		{
			for (int j = 1; j < grid[i].size() - 1; ++j)
			{
				if (grid[i][j] == 'A')
				{
					int x = 1;
					if (
					(
						( grid[i-1][j-1] == 'M' && grid[i+1][j+1] == 'S' ) ||  ( grid[i-1][j-1] == 'S' && grid[i+1][j+1] == 'M' )
					)
					&&
					(
						( grid[i-1][j+1] == 'M' && grid[i+1][j-1] == 'S' ) ||  ( grid[i-1][j+1] == 'S' && grid[i+1][j-1] == 'M' )
					)
					)
					{
						++count;
					}
				}
			}
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << count << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input04.txt", inputLines))
		{
			throw std::runtime_error("[Day 04] Unable to open input file.");
		}

		// convert to chars
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
