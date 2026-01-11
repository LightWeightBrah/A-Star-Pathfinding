#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include "main.h"
#include "Node.h"
#include "Grid.h"
#include "AStar.h"

int main()
{
	int width = 0;
	int height = 0;
	auto grid = SetupWorld(MapTemplates::grid30x20, width, height);

	Node* startNode = &grid[height - 1][0];
	Node* endNode = &grid[0][width - 1];

	bool found = RunAStar(grid, startNode, endNode, width, height);
	if (found)
	{
		TraverseBackToStart(grid, startNode, endNode);
		RenderMap(grid, startNode, endNode);
	}
	else
	{
		std::cout << "Nie znaleziono drogi, zmien mape";
	}
	
	return 0;
}