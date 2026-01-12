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
	auto grid = SetupWorld("grid.txt", width, height);
	FindPath(grid, width, height);

	return 0;
}