#pragma once

#include "float.h"
#include "memory"

#include "Matrix.h"
#include "Ant.h"
#include "AntColonyPathFinderResponse.h"

/*
Implements ACO for path finding.
*/
class AntColonyPathFinder
{
public:

	/*
	Parametrized constructor.
	*/
	AntColonyPathFinder(
		const unsigned int population,
		const double alpha,
		const double beta,
		const double q,
		const double evaporationRate, //[0, 1]
		const unsigned int countOfIterations
	)
	{
		this->population = population;
		this->alpha = alpha;
		this->beta = beta;
		this->q = q;
		this->evaporationRate = evaporationRate;
		this->countOfIterations = countOfIterations;

		for (unsigned int antNumber = 0; antNumber < population; antNumber++)
		{
			ants.push_back(Ant(alpha, beta, q));
		}
	}

	AntColonyPathFinderResponse findPath(
		const unsigned int startVertex,
		const unsigned int endVertex,
		const std::shared_ptr<Matrix<double> > routeCosts
	)
	{
		this->routeCosts = routeCosts;
		initializePheromoneLevels(
			routeCosts->getRowCount(),
			routeCosts->getColumnCount()
			);

		initializeAnts();

		for (unsigned int iteration = 0; iteration < countOfIterations; iteration++)
		{
			for (auto& ant : ants)
			{
				ant.findPath(startVertex, endVertex);
			}
			evaporate();

			updateBestEver();
		}
		return AntColonyPathFinderResponse(bestEverRoute, bestEverCost);
	}

private:

	void updateBestEver()
	{
		unsigned int localBestNumber = 0;

		for (unsigned int antNumber = 1; antNumber < ants.size(); antNumber++)
		{
			if (ants[antNumber].getRouteCost() < ants[localBestNumber].getRouteCost())
			{
				localBestNumber = antNumber;
			}
		}

		if (ants[localBestNumber].getRouteCost() < bestEverCost)
		{
			bestEverCost = ants[localBestNumber].getRouteCost();
			bestEverRoute = ants[localBestNumber].getCurrentRoute();
		}
	}

	void evaporate()
	{
		auto rowCount = pheromoneLevels->getRowCount();
		auto columnCount = pheromoneLevels->getColumnCount();
		for (unsigned int row = 0; row < rowCount; row++)
		{
			for (unsigned int column = 0; column < columnCount; column++)
			{
				pheromoneLevels->setAt(row, column, pheromoneLevels->getAt(row, column) * (1 - evaporationRate));
			}
		}
	}

	void initializePheromoneLevels(
		const unsigned int rowCount,
		const unsigned int columnCount
	)
	{
		pheromoneLevels = std::shared_ptr<Matrix<double> >(new Matrix<double>(
			rowCount,
			columnCount
			)
			);

		for (unsigned int row = 0; row < rowCount; row++)
		{
			for (unsigned int column = 0; column < columnCount; column++)
			{
				pheromoneLevels->setAt(row, column, 0.1);
			}
		}
	}

	void initializeAnts()
	{
		for (auto& ant : ants)
		{
			ant.initialize(routeCosts, pheromoneLevels);
		}
	}

	unsigned int population;
	double alpha;
	double beta;
	double q;
	double evaporationRate;
	unsigned int countOfIterations;

	std::vector<unsigned int> bestEverRoute;
	double bestEverCost = DBL_MAX;

	std::vector<Ant> ants;

	std::shared_ptr<Matrix<double> > pheromoneLevels;

	std::shared_ptr<Matrix<double> > routeCosts;
};

