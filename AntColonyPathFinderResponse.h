#pragma once
#include "vector"
struct AntColonyPathFinderResponse
{

	AntColonyPathFinderResponse(
		const std::vector<unsigned int>& route,
		const double cost
	)
	{
		this->route = route;
		this->cost = cost;
	}

	std::vector<unsigned int> route;
	double cost;
};

