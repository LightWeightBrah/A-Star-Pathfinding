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
	int width = 0, height = 0;
	auto grid = SetupWorld(MapTemplates::grid30x20, width, height);
	FindPath(grid, width, height);

	return 0;
}