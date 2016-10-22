#pragma once
#include "Matrix.h"
#include "memory"
#include "iostream"
#include "fstream"
#include "sstream"
#include "string"
class ACOReader
{
public:

	ACOReader()
	{
	}

	std::shared_ptr<Matrix<double> > FromFile(const std::string& filename) 
	{
		std::ifstream graphFile;
		graphFile.open(filename);
		std::string line;

		if (!graphFile.is_open())
		{
			throw;
		}

		// Read comments.
		do
		{
			std::getline(graphFile, line);
		} while (line[0] == 'c' || line[0] == 'C');

		// Read problem size.
		std::istringstream iss(line, std::istringstream::in);
		std::string word;
		std::vector<std::string> words;
		unsigned int problemSize;
		while (iss >> word)
		{
			words.push_back(word);
		}
		if (words.size() == 2)
		{
			problemSize = std::stoul(words[1]);
		}

		// Read matrix.
		auto routes = std::shared_ptr<Matrix<double> >(new Matrix<double>(problemSize, problemSize));
		for (unsigned int row = 0; row < problemSize; row++)
		{
			std::getline(graphFile, line);
			std::istringstream iss(line, std::istringstream::in);
			std::string word;
			std::vector<std::string> words;

			while (iss >> word)
			{
				words.push_back(word);
			}
			if (words.size() == problemSize + 1)
			{
				for (unsigned int column = 0; column < problemSize; column++)
				{
					auto distance = std::stol(words[column + 1]);
					if (0 == distance) 
					{
						distance = -1;
					}
					routes->setAt(row, column, distance);
				}
			}
		}
		graphFile.close();
		return routes;
	}

	~ACOReader()
	{
	}
};

