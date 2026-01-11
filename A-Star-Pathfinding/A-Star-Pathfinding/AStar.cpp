#include "AStar.h"
#include <iostream>
#include "Grid.h"

void FindPath(std::vector<std::vector<Node>>& grid, int width, int height, int startX, int startY, int endX, int endY)
{
	int finalStartX = (startX == -1) ? 0 : startX;
	int finalStartY = (startY == -1) ? height - 1 : startY;

	int finalEndX = (endX == -1) ? width - 1 : endX;
	int finalEndY = (endY == -1) ? 0 : endY;

	if (finalStartX >= width || finalStartY >= height ||
		finalEndX >= width || finalEndY >= height ||
		finalStartX < 0 || finalStartY < 0 ||
		finalEndX < 0 || finalEndY < 0)
	{
		std::cout << "ERROR: Coordinates out of grid bounds!!!" << std::endl;
		return;
	}

	Node* startNode = &grid[finalStartY][finalStartX];
	Node* endNode = &grid[finalEndY][finalEndX];

	if (startNode->cellType == CELL::WALL || endNode->cellType == CELL::WALL)
	{
		std::cout << "ERROR: Start or End is a WALL (5)!!!" << std::endl;
		return;
	}

	std::cout << "Width is: (" << width << ") Height is: (" << height << ")" << std::endl;
	std::cout << "Start: [" << finalStartX << ", " << finalStartY << "] "
		<< "End: [" << finalEndX << ", " << finalEndY << "]" << std::endl;

	bool found = RunAStar(grid, startNode, endNode, width, height);
	if (found)
	{
		TraverseBackToStart(grid, startNode, endNode);
		RenderMap(grid, startNode, endNode);
		std::cout << "\nYOU FOUND A PATH, LET'S GO!!!" << std::endl;
	}
	else
	{
		std::cout << "\nCouldn't find a path :C" << "Change map template..." << std::endl;
	}
}

bool RunAStar(std::vector<std::vector<Node>>& grid, Node* startNode, Node* endNode, const int& width, const int& height)
{
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	closedList.push_back(startNode);

	do
	{
		Node* closedNode = closedList.back();
		if (closedNode == endNode)
			return true;

		if (closedNode->y + 1 < height)
		{
			CheckNeighbour(grid, &grid[closedNode->y + 1][closedNode->x], closedNode, endNode, openList, closedList);
		}

		if (closedNode->x - 1 >= 0)
		{
			CheckNeighbour(grid, &grid[closedNode->y][closedNode->x - 1], closedNode, endNode, openList, closedList);
		}

		if (closedNode->y - 1 >= 0)
		{
			CheckNeighbour(grid, &grid[closedNode->y - 1][closedNode->x], closedNode, endNode, openList, closedList);
		}

		if (closedNode->x + 1 < width)
		{
			CheckNeighbour(grid, &grid[closedNode->y][closedNode->x + 1], closedNode, endNode, openList, closedList);
		}

		Node* lowest = FindLowestCostNode(openList);
		if (!lowest)
			return false;

		UpdateLists(closedList, openList, lowest);
	} while (!openList.empty());

	return false;
}

void CheckNeighbour(std::vector<std::vector<Node>>& grid, Node* neighbour, Node* closedNode, Node* endNode, std::vector<Node*>& openList, std::vector<Node*>& closedList)
{
	if (neighbour->cellType == CELL::WALL)
		return;

	if (IsNodeInVector(closedList, neighbour))
		return;

	ResolveConflicts(grid, *neighbour, *closedNode, *endNode);

	if (!IsNodeInVector(openList, neighbour))
	{
		neighbour->cellType = CELL::OPEN;
		openList.push_back(neighbour);
	}
}

void ResolveConflicts(std::vector<std::vector<Node>>& grid, Node& neighbour, Node& closedNode, Node& endNode)
{
	double gCost = closedNode.gCost + 1;
	double hCost = neighbour.CalculateHCost(neighbour.x, neighbour.y,
		endNode.x, endNode.y);
	double newFCost = gCost + hCost;

	if (newFCost < neighbour.fCost)
	{
		neighbour.gCost = gCost;
		neighbour.hCost = hCost;
		neighbour.fCost = newFCost;
		neighbour.SetParent(&grid[closedNode.y][closedNode.x]);
	}
}

Node* FindLowestCostNode(std::vector<Node*>& openList)
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

void UpdateLists(std::vector<Node*>& closedList, std::vector<Node*>& openList, Node* lowestCostNode)
{
	if (lowestCostNode == nullptr)
		return;

	RemoveItemFromVector(openList, lowestCostNode);
	lowestCostNode->cellType = CELL::CLOSED;
	closedList.push_back(lowestCostNode);
}

void TraverseBackToStart(std::vector<std::vector<Node>>& grid, Node* startNode, Node* endNode)
{
	Node* currentNode = endNode;

	while (currentNode != nullptr && currentNode != startNode)
	{
		if (currentNode != endNode)
			currentNode->cellType = CELL::ROUTE;
	
		currentNode = currentNode->parent;
	}
}

void RemoveItemFromVector(std::vector<Node*>& vec, Node* node)
{
	vec.erase(std::remove(vec.begin(), vec.end(), node), vec.end());
}

bool IsNodeInVector(const std::vector<Node*>& vec, Node* node)
{
	for (Node* n : vec)
	{
		if (n == node)
			return true;
	}
	return false;
}