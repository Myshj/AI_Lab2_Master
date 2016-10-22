#pragma once
#include "iostream"

#include "AntColonyPathFinder.h"
#include "ACOReader.h"

using namespace std;

/*
Results:
shp55: cost=3
shp94: cost=12
shp155: cost=5
*/

int main(int argc, char* argv[])
{
	auto reader = ACOReader();
	std::shared_ptr<Matrix<double> > routes;
	unsigned int colonySize = 10;
	double alpha = 1.0;
	double beta = 1.0;
	double q = 1.0;
	double evaporationRate = 0.5;
	unsigned int countOfIterations = 100;

	unsigned int startVertex = 0;
	unsigned int endVertex = 1;

	for (int i = 0; i < argc - 1; i++)
	{
		if (0 == _stricmp(argv[i], "--file"))
		{
			routes = reader.FromFile(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--colony"))
		{
			colonySize = atol(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--alpha"))
		{
			alpha = atof(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--beta"))
		{
			beta = atof(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "-q"))
		{
			q = atof(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--evaporation_rate"))
		{
			evaporationRate = atof(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--iterations"))
		{
			countOfIterations = atol(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--start_vertex"))
		{
			startVertex = atol(argv[i + 1]);
		}
		else if (0 == _stricmp(argv[i], "--end_vertex"))
		{
			endVertex = atol(argv[i + 1]);
		}
	}

	auto aco = AntColonyPathFinder(
		colonySize, // colony size
		alpha, // alpha
		beta, // beta
		q, // q
		evaporationRate, // evaporation rate
		countOfIterations // count of iterations
	);

	auto response = aco.findPath(startVertex, endVertex, routes);

	cout << "Result:" << endl;
	cout << "Route: ";
	for (auto vertex : response.route)
	{
		cout << vertex << " ";
	}
	cout << endl;
	cout << "Cost: " << response.cost << endl;
	getchar();
	return 0;
}