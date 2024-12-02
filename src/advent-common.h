/************************  ADVENT OF CODE 2024 **********************************
**                         adventofcode.com                                     *       *
**                                                                              *      /.\
**                         mlogic1 (https://github.com/mlogic1)                 *     /..'\
**                                                                              *     /'.'\
**                                                                              *    /.''.'\
**                                                                              *    /.'.'.\
**                                                                              *   /'.''.'.\
**                                                                              *   ^^^[_]^^^
********************************************************************************/

#ifndef ADVENT_COMMON
#define ADVENT_COMMON

#include <fstream>
#include <set>
#include <map>
#include <string>
#include <sstream>
#include <stack>
#include <vector>
#include <type_traits>

using std::string;
using std::stringstream;
using std::vector;

using StringVector = vector<string>;
using IntVector = vector<int>;
using VecStack = std::vector<std::stack<char>>;

namespace common
{
	static bool ReadEntireFileStrings(const std::string& fileName, StringVector& vec)
	{
		std::ifstream file(fileName);
		if (file.good())
		{
			std::string line;
			while (std::getline(file, line))
			{
				vec.emplace_back(line);
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	static bool ReadEntireFileInts(const std::string& fileName, IntVector& vec)
	{
		std::ifstream file(fileName);
		if (file.good())
		{
			std::string line;
			while (std::getline(file, line))
			{
				vec.emplace_back(std::stoi(line));
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	static StringVector SplitStringWithDelimiter(const string& data, const char delimiter)
	{
		StringVector result;
		stringstream sstream(data);

		string temp;
		while (getline(sstream, temp, delimiter))
				result.push_back(temp);

		return result;
	}

	static std::ifstream OpenFile(const std::string& fileName)
	{
		std::ifstream fileStream(fileName, std::ios::in);

		if (!fileStream.is_open())
		{
			throw std::runtime_error("Unable to open file: " + fileName);
		}

		return fileStream;
	}

	static void CloseFile(std::ifstream& file)
	{
		file.close();
	}

	template<typename T>
	static T** CreateGrid(const int rows, const int columns)
	{
		T** result = new T* [rows];

		for (int i = 0; i < rows; ++i)
		{
			result[i] = new T[columns];
		}

		return result;
	}

	template<typename T>
	static void DeleteGrid(T** grid, int rows, int columns)
	{
		for (int i = 0; i < columns; ++i)
		{
			delete[] grid[i];
		}

		delete[] grid;
	}

	template<typename T>
	static std::vector<T> GetNumbersInString(const std::string& str)
	{
		std::stringstream ss(str);
		std::vector<T> result;

		while( !isdigit(ss.peek()) )
			ss.ignore();

		T num;
		while(ss >> num)
		{
			result.push_back(num);
			if ( ss.good() && !isdigit(ss.peek()) )
				ss.ignore();
		}

		return result;
	}
}

#endif
