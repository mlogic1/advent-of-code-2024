/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                         Day 13 - Claw Contraption                            *    /.'.'.\
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
#include <limits.h>

namespace Day13
{
	std::mutex coutMutex;

	struct ClawMachine
	{
		std::pair<int64_t, int64_t> ButtonA;
		std::pair<int64_t, int64_t> ButtonB;

		std::pair<int64_t, int64_t> Prize;
	};

	std::vector<ClawMachine> LoadMachines(const StringVector& inputLines)
	{
		std::vector<ClawMachine> machines;

		for (int i = 0; i < inputLines.size(); i+=4)
		{
			const std::string lineBtnA = inputLines[i];
			const std::string lineBtnB = inputLines[i + 1];
			const std::string linePrize = inputLines[i + 2];
			ClawMachine claw;
			std::sscanf(lineBtnA.c_str(), "Button A: X+%lld, Y+%lld", &claw.ButtonA.first, &claw.ButtonA.second);
			std::sscanf(lineBtnB.c_str(), "Button B: X+%lld, Y+%lld", &claw.ButtonB.first, &claw.ButtonB.second);
			std::sscanf(linePrize.c_str(), "Prize: X=%lld, Y=%lld", &claw.Prize.first, &claw.Prize.second);

			machines.emplace_back(std::move(claw));
		}

		return machines;
	}

	uint64_t CramerCost(const ClawMachine& machine, const int costA, const int costB)
	{
		uint64_t aPress = (machine.Prize.first * machine.ButtonB.second - machine.Prize.second * machine.ButtonB.first) / (machine.ButtonA.first * machine.ButtonB.second - machine.ButtonA.second * machine.ButtonB.first);
		uint64_t bPress = (machine.ButtonA.first * machine.Prize.second - machine.ButtonA.second * machine.Prize.first) / (machine.ButtonA.first * machine.ButtonB.second - machine.ButtonA.second * machine.ButtonB.first);

		if ( (aPress * machine.ButtonA.first + bPress * machine.ButtonB.first) == machine.Prize.first)
		{
			if ( (aPress * machine.ButtonA.second + bPress * machine.ButtonB.second) == machine.Prize.second)
			{
				return aPress * costA + bPress * costB;
			}
		}

		return 0;
	}

	uint64_t CalculateCost(const ClawMachine& machine, const int costA, const int costB)
	{
		int64_t cost = INT_MAX;

		const uint64_t maxPressB = machine.Prize.first / machine.ButtonB.first;
		const uint64_t maxPressA = machine.Prize.first / machine.ButtonA.first;

		for (uint64_t i = maxPressB; i > 0; --i)
		{
			for (uint64_t j = 0; j < maxPressA; ++j)
			{
				const uint64_t currentPosX = i * machine.ButtonB.first;
				const uint64_t currentPosY = i * machine.ButtonB.second;
				if (currentPosX > machine.Prize.first || currentPosY > machine.Prize.second)
				{
					continue;
				}

				const uint64_t btnAoffsetX = j * machine.ButtonA.first;
				const uint64_t btnAoffsetY = j * machine.ButtonA.second;

				const uint64_t resultX = currentPosX + btnAoffsetX;
				const uint64_t resultY = currentPosY + btnAoffsetY;

				if (resultX == machine.Prize.first && resultY == machine.Prize.second)
				{
					// found
					int64_t currentCost = costA * j + costB * i;
					if (currentCost < cost)
					{
						cost = currentCost;
					}
				}
			}
		}

		if (cost < INT_MAX)
		{
			return cost;
		}
		return 0;
	}

	void PartA(const std::vector<ClawMachine>& clawMachines)
	{
		uint64_t totalCost = 0;

		for (const ClawMachine& clawMachine : clawMachines)
		{
			totalCost += CalculateCost(clawMachine, 3, 1);
		}
		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part A: " << totalCost << std::endl;
	}

	void PartB(const std::vector<ClawMachine>& clawMachines)
	{
		std::vector<ClawMachine> offsetMachines = clawMachines;

		std::for_each(offsetMachines.begin(), offsetMachines.end(), [](ClawMachine& machine)
		{
			machine.Prize.first += 10000000000000;
			machine.Prize.second += 10000000000000;
		});

		uint64_t totalCost = 0;
		for (const ClawMachine& clawMachine : offsetMachines)
		{
			totalCost += CramerCost(clawMachine, 3, 1);
		}

		std::lock_guard<std::mutex> lock(coutMutex);
		std::cout << "Part B: " << totalCost << std::endl;
	}

	void RunDay()
	{
		StringVector inputLines;
		if (!common::ReadEntireFileStrings("inputs/input13.txt", inputLines))
		{
			throw std::runtime_error("[Day 13] Unable to open input file.");
		}

		std::vector<ClawMachine> machines = LoadMachines(inputLines);

		std::thread aThread(PartA, machines);
		PartB(machines);
		aThread.join();
	}
}
