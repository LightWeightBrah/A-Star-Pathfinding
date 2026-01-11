#include "AStar.h"
#include <iostream>

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