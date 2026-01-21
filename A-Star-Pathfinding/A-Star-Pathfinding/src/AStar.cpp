#include <iostream>

#include "AStar.h"
#include "Grid.h"

AStar::AStar()
{
	grid = SetupWorld("", width, height);
	if (grid.empty())
	{
		std::cout << "A Star won't be able to run. Please fix the map file..." << std::endl;
		return;
	}
}

AStar::AStar(std::string gridFilepath)
{
	grid = SetupWorld(gridFilepath, width, height);
	if (grid.empty())
	{
		std::cout << "A Star won't be able to run. Please fix the map file..." << std::endl;
		return;
	}
}

AStar::AStar(std::string gridFilepath, int startX, int startY, int endX, int endY) 
	: startX(startX), startY(startY), endX(endX), endY(endY)
{
	grid = SetupWorld(gridFilepath, width, height);
	if (grid.empty())
	{
		std::cout << "A Star won't be able to run. Please fix the map file..." << std::endl;
		return;
	}
}

void AStar::FindPathBySteps(float deltaTime)
{
	if (!ValidateStartEndCoordinates())
		return;

	if (finishedDrawing)
		return;

	Node* startNode = &grid[startY][startX];
	Node* endNode = &grid[endY][endX];

	astarCounter += deltaTime;
	if (astarCounter >= astarInterval)
	{
		astarCounter = 0;
		if (!pathFound)
		{
			pathFound = RunAStarStep();
			if (pathFound)
			{
				drawingPath = true;
				std::cout << "\nYOU FOUND A PATH, STARTING DRAWING..." << std::endl;
			}
		}
		else if(drawingPath)
		{
			TraverseBackToStartSteps();
			std::cout << "\nCouldn't find a path :C" << "Change map template..." << std::endl;

		}
	}
}

void AStar::FindPathFull()
{
	if(!ValidateStartEndCoordinates())
		return;

	startNode = &grid[startY][startX];
	endNode	  = &grid[endY][endX];

	PrintCoordinates();

	pathFound = RunAStarFull();
	if (pathFound)
	{
		TraverseBackToStart();
		RenderMap(grid, startNode, endNode);
		std::cout << "\nYOU FOUND A PATH, LET'S GO!!!" << std::endl;
	}
	else
	{
		std::cout << "\nCouldn't find a path :C" << "Change map template..." << std::endl;
	}
}

bool AStar::RunAStarFull()
{
	closedList.push_back(startNode);

	do
	{
		Node* closedNode = closedList.back();
		if (closedNode == endNode)
			return true;

		if (closedNode->y + 1 < height)
		{
			CheckNeighbour(&grid[closedNode->y + 1][closedNode->x], closedNode);
		}

		if (closedNode->x - 1 >= 0)
		{
			CheckNeighbour(&grid[closedNode->y][closedNode->x - 1], closedNode);
		}

		if (closedNode->y - 1 >= 0)
		{
			CheckNeighbour(&grid[closedNode->y - 1][closedNode->x], closedNode);
		}

		if (closedNode->x + 1 < width)
		{
			CheckNeighbour(&grid[closedNode->y][closedNode->x + 1], closedNode);
		}

		Node* lowest = FindLowestCostNode();
		if (!lowest)
			return false;

		UpdateLists(lowest);
	} while (!openList.empty());

	return false;
}

bool AStar::RunAStarStep()
{
	if (pathFound)
		return true;

	if (closedList.empty())
		closedList.push_back(startNode);

	Node* closedNode = closedList.back();
	if (closedNode == endNode)
		return true;

	switch (neighbourStep)
	{
	case 0:
		if (closedNode->y + 1 < height)
			CheckNeighbour(&grid[closedNode->y + 1][closedNode->x], closedNode);
		break;

	case 1:
		if (closedNode->x - 1 >= 0)
			CheckNeighbour(&grid[closedNode->y][closedNode->x - 1], closedNode);
		break;

	case 2:
		if (closedNode->y - 1 >= 0)
			CheckNeighbour(&grid[closedNode->y - 1][closedNode->x], closedNode);
		break;

	case 3:
		if (closedNode->x + 1 < width)
			CheckNeighbour(&grid[closedNode->y][closedNode->x + 1], closedNode);
		break;
	}

	neighbourStep++;
	
	if (neighbourStep > 3)
	{
		neighbourStep = 0;

		Node* lowest = FindLowestCostNode();
		if (!lowest)
			return false;

		UpdateLists(lowest);
		if (lowest == endNode)
			return true;
	}

	return false;
}

void AStar::TraverseBackToStart()
{
	Node* currentNode = endNode;

	while (currentNode != nullptr && currentNode != startNode)
	{
		if (currentNode != endNode)
			currentNode->cellType = CELL::ROUTE;

		currentNode = currentNode->parent;
	}
}

void AStar::TraverseBackToStartSteps()
{
	if (pathTracker == nullptr)
		pathTracker = endNode;

	if (pathTracker == startNode)
	{
		drawingPath = false;
		finishedDrawing = true;
		return;
	}

	pathTracker->cellType = CELL::ROUTE;
	pathTracker = pathTracker->parent;
}

void AStar::CheckNeighbour(Node* neighbour, Node* closedNode)
{
	if (neighbour->cellType == CELL::WALL)
		return;

	if (IsNodeInVector(closedList, neighbour))
		return;

	ResolveConflicts(*neighbour, *closedNode);

	if (!IsNodeInVector(openList, neighbour))
	{
		neighbour->cellType = CELL::OPEN;
		openList.push_back(neighbour);
	}
}

void AStar::ResolveConflicts(Node& neighbour, Node& closedNode)
{
	double gCost = closedNode.gCost + 1;
	double hCost = neighbour.CalculateHCost(neighbour.x, neighbour.y,
		endNode->x, endNode->y);
	double newFCost = gCost + hCost;

	if (newFCost < neighbour.fCost)
	{
		neighbour.gCost = gCost;
		neighbour.hCost = hCost;
		neighbour.fCost = newFCost;
		neighbour.SetParent(&grid[closedNode.y][closedNode.x]);
	}
}

Node* AStar::FindLowestCostNode()
{
	if (openList.empty())
		return nullptr;

	Node* lowest = openList[0];
	for (int i = 1; i < openList.size(); i++)
	{
		if (openList[i]->fCost <= lowest->fCost)
		{
			lowest = openList[i];
		}
	}

	return lowest;
}

void AStar::UpdateLists(Node* lowestCostNode)
{
	if (lowestCostNode == nullptr)
		return;

	RemoveItemFromVector(openList, lowestCostNode);
	lowestCostNode->cellType = CELL::CLOSED;
	closedList.push_back(lowestCostNode);
}



bool AStar::ValidateStartEndCoordinates()
{
	startX = (startX ==  -1) ?     0		: startX;
	startY = (startY ==  -1) ?  height - 1  : startY;
						 
	endX   = (endX   ==  -1) ?  width  - 1	: endX;
	endY   = (endY   ==  -1) ?	   0		: endY;

	if (startX >= width || startY >= height ||
		endX   >= width || endY   >= height ||
		startX <    0   || startY <    0    ||
		endX   <    0   || endY   <    0)
	{
		std::cout << "ERROR: COORDINATES OUT OF GRID BOUNDS!!!" << std::endl;
		return false;
	}

	startNode = &grid[startY][startX];
	endNode	  = &grid[endY][endX];

	if (startNode->cellType == CELL::WALL || endNode->cellType == CELL::WALL)
	{
		std::cout << "ERROR: Start or End is a WALL (5)!!!" << std::endl;
		return false;
	}

	return true;
}

void AStar::PrintCoordinates()
{
	std::cout << "Width is: (" << endX << ") Height is: (" << endY << ")" << std::endl;
	std::cout << "Start: [" << endX << ", " << endY << "] "
		<< "End: [" << endX << ", " << endY << "]" << std::endl;
}

void AStar::RemoveItemFromVector(std::vector<Node*>& vec, Node* node)
{
	vec.erase(std::remove(vec.begin(), vec.end(), node), vec.end());
}

bool AStar::IsNodeInVector(const std::vector<Node*>& vec, Node* node)
{
	for (Node* n : vec)
	{
		if (n == node)
			return true;
	}
	return false;
}