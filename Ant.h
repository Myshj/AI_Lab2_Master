#pragma once

#include "vector"
#include "memory"
#include "algorithm"

#include "math.h"

#include "Matrix.h"

class Ant
{
public:

	Ant(
		const double alpha,
		const double beta,
		const double q
	)
	{
		this->alpha = alpha;
		this->beta = beta;
		this->q = q;
	}

	void initialize(
		const std::shared_ptr<Matrix<double> >& routeCosts,
		const std::shared_ptr<Matrix<double> >& pheromoneLevels
	)
	{
		this->routeCosts = routeCosts;
		this->pheromoneLevels = pheromoneLevels;
	}

	void findPath(
		const unsigned int startVertex,
		const unsigned int endVertex
	)
	{
		// Reset current route.
		currentRoute = std::vector<unsigned int>();
		currentRoute.push_back(startVertex);

		this->endVertex = endVertex;

		// Find path.
		while (currentRoute.back() != endVertex)
		{
			auto nextVertex = chooseNextVertex();
			currentRoute.push_back(nextVertex);
		}

		// Update pheromones.
		caculateCostForCurrentRoute();

		updatePheromones();
	}

	std::vector<unsigned int> getCurrentRoute() const
	{
		return currentRoute;
	}

	double getRouteCost() const
	{
		return routeCost;
	}

private:

	void updatePheromones()
	{
		double deltaPheromone = q / routeCost;
		for (unsigned int i = 0; i < currentRoute.size() - 1; i++)
		{
			pheromoneLevels->setAt(i, i + 1, pheromoneLevels->getAt(i, i + 1) + deltaPheromone);
		}
	}

	void caculateCostForCurrentRoute()
	{
		routeCost = 0.0;
		for (unsigned int i = 0; i < currentRoute.size() - 1; i++)
		{
			routeCost += routeCosts->getAt(currentRoute[i], currentRoute[i + 1]);
		}
	}

	unsigned int chooseNextVertex()
	{
		currentVertex = currentRoute.back();

		vertexCount = routeCosts->getColumnCount();

		calculateVertexMultipliersForCurrentVertex();
		calculateVertexProbabilitiesForCurrentVertex();

		double r = (double)rand() / (RAND_MAX + 1) * sumOfProbabilities;
		double localSum = 0.0;
		for (unsigned int vertexNumber = 0; vertexNumber < vertexCount; vertexNumber++)
		{
			localSum += vertexProbabilities[vertexNumber];

			if (localSum >= r && verticesAreAdjacent(currentVertex, vertexNumber))
			{
				return vertexNumber;
			}
		}
	}

	bool vertexIsInCurrentRoute(
		const unsigned int vertexNumber
	) const
	{
		return currentRoute.end() != find(currentRoute.begin(), currentRoute.end(), vertexNumber);
	}

	bool verticesAreAdjacent(
		const unsigned int startVertex,
		const unsigned int endVertex
	) const
	{
		return 0.0 <= routeCosts->getAt(startVertex, endVertex);
	}

	void calculateVertexMultipliersForCurrentVertex()
	{
		vertexMultipliers = std::vector<double>(vertexCount);
		sumOfMultipliers = 0.0;
		// Calculate multipliers and sum of multipliers for adjacent vertices.
		for (unsigned int vertexNumber = 0; vertexNumber < vertexCount; vertexNumber++)
		{
			if (
				!verticesAreAdjacent(currentVertex, vertexNumber) ||
				vertexIsInCurrentRoute(vertexNumber)
				)
			{
				continue;
			}
			double pheromoneLevel = pheromoneLevels->getAt(currentVertex, vertexNumber);
			double fitness = (double)1 / routeCosts->getAt(currentVertex, vertexNumber);

			double multiplier = pow(pheromoneLevel, alpha) * pow(fitness, beta);

			vertexMultipliers[vertexNumber] = multiplier;

			sumOfMultipliers += multiplier;
		}
	}

	void calculateVertexProbabilitiesForCurrentVertex()
	{
		vertexProbabilities = std::vector<double>(vertexCount);
		sumOfProbabilities = 0.0;
		// Calculate probabilities and sum of probabilities for adjacent vertices.
		for (unsigned int vertexNumber = 0; vertexNumber < vertexCount; vertexNumber++)
		{
			if (
				!verticesAreAdjacent(currentVertex, vertexNumber) ||
				vertexIsInCurrentRoute(vertexNumber)
				)
			{
				continue;
			}

			double probability = vertexMultipliers[vertexNumber] / sumOfMultipliers;

			vertexProbabilities[vertexNumber] = probability;

			sumOfProbabilities += probability;
		}
	}

	double alpha;
	double beta;
	double q;

	unsigned int vertexCount;
	unsigned int currentVertex;

	unsigned int endVertex;

	std::vector<unsigned int> currentRoute;
	double routeCost;

	std::vector<double> vertexMultipliers;
	double sumOfMultipliers;

	std::vector<double> vertexProbabilities;
	double sumOfProbabilities;

	std::shared_ptr<Matrix<double> > routeCosts;
	std::shared_ptr<Matrix<double> > pheromoneLevels;
};

