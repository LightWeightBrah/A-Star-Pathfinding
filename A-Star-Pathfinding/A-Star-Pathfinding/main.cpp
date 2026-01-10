#include <iostream>
#include <tuple>
#include <string>
#include <vector>
#include "main.h"
#include "Node.h"

#pragma region gridTemplates

const std::string gridTextTemplate20x20 =
R"(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 5 5 5
5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0
0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 5 0 0 5 5 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0
0 0 0 5 5 5 5 5 5 5 5 5 0 5 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 5 5
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0
5 0 0 0 5 5 5 5 0 0 5 0 0 0 0 0 5 0 0 0
5 5 5 5 5 5 5 5 5 5 5 0 0 0 0 0 5 0 0 0
5 0 0 0 0 0 5 0 0 0 5 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0)";

const std::string gridTextTemplate30x20 =
R"(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 5
0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5
5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 5
0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5
0 0 0 0 0 0 0 5 0 0 5 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 5 5 5 5 5 5 5 5 5 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 0 5 0 0 5 5 0 5 0 0 0 0 5 5 5 5 5 5 5 5 5 5 5 5
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 5 5 0 0 5 5 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 5 5 5 5 0 0 5 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 5 5 5 5 5 5 5 5 5 5 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0
5 0 0 0 0 0 5 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0)";

const std::string gridTemplate3x3 = R"(0 0 5
0 5 0
5 0 0)";

#pragma endregion

std::vector < std::vector<Node>> InitGrid(std::string& gridText, int& width, int& height)
{
	std::vector < std::vector<Node>> grid;
	int charIndex = 0;

	for (int y = 0; y < height; y++)
	{
		std::vector<Node> singleRow;
		for (int x = 0; x < width; x++)
		{
			while (charIndex < gridText.length() && (gridText[charIndex] == '\n'))
				charIndex++;

			if (charIndex > gridText.length())
				continue;

			char currentCell = gridText[charIndex];
			singleRow.push_back(Node(x, y, (CELL)(currentCell - '0')));
			charIndex++;
		}

		if(!singleRow.empty())
			grid.push_back(singleRow);
	}


	for (const auto& row : grid) 
	{
		for (const auto& node : row)
		{
			std::cout << (int)node.cellType << " ";
		}
		std::cout << std::endl;
	}


	return grid;
}


void GetGridDimensions(std::string& gridText, int& width, int& height)
{
	int i = 0;
	while (gridText[i] != '\0')
	{
		if (gridText[i] == '\n')
		{
			height++;
			width = 0;
		}
		else
		{
			width++;
		}

		i++;
	}

	std::cout << "width is: " << width << " height is: " << height << std::endl;
}

std::string TrimGrid(std::string girdTemplate)
{
	std::string output  = girdTemplate;
	output.erase(std::remove(output.begin(), output.end(), ' '), output.end());
	std::cout << output << std::endl;
	return output;
}

void RemoveItemFromVector(std::vector<Node>& vec, Node node)
{
	vec.erase(std::remove(vec.begin(), vec.end(),
		node), vec.end());
}

void ResolveConflicts(Node& neighbour, Node& closedNode, Node& endNode)
{
	if (neighbour.fCost != NULL)
	{
		double gCost = closedNode.gCost + 1;
		double hCost = neighbour.CalculateHCost(
			neighbour.x, neighbour.y,
			endNode.x, endNode.y);

		int newFCost = gCost + hCost;

		if (newFCost < neighbour.fCost)
		{
			neighbour.gCost = gCost;
			neighbour.hCost = hCost;
			neighbour.fCost = newFCost;
			neighbour.SetParent(&closedNode);
		}
	}
	else
	{
		neighbour.hCost = neighbour.CalculateHCost(
			neighbour.x, neighbour.y,
			endNode.x, endNode.y);
		neighbour.SetParent(&closedNode);
	}
}

void CheckNeighbour(Node& neighbour, Node& closedNode, Node& endNode, std::vector<Node>& openList)
{
	neighbour.IncreaseG();
	ResolveConflicts(neighbour, closedNode, endNode);
	openList.push_back(neighbour);
}

void UpdateLists(std::vector<Node>& closedList, Node& lowestCostNode, std::vector<Node>& openList)
{
	closedList.push_back(lowestCostNode);
	RemoveItemFromVector(openList, lowestCostNode);
}

void FindLowestCostNode(std::vector<Node>& openList, Node& lowestCostNode)
{
	for (int i = 0; i < openList.size(); i++)
	{
		if (openList[i].fCost <= lowestCostNode.fCost)
		{
			lowestCostNode = openList[i];
		}
	}
}

void TraverseBackToStart(std::vector<Node>& closedList, Node& endNode, Node& startNode)
{
	if (closedList.back() == endNode)
	{
		Node* currentNode = &closedList.back();
		while (currentNode != &startNode)
		{
			(*currentNode).cellType = CELL::ROUTE;
			currentNode = (*currentNode).parent;
		}
	}
}

int main()
{
	std::string gridText = TrimGrid(gridTextTemplate30x20);

	int width = 0;
	int height = 1;
	GetGridDimensions(gridText, width, height);

	std::vector<std::vector<Node>> grid = InitGrid(gridText, width, height);
	Node& startNode = grid[height - 1][0];
	Node& endNode = grid[0][width - 1];

	std::vector<Node> openList;
	std::vector<Node> closedList;
	closedList.push_back(startNode);

	while (closedList.back() != endNode)
	{
		Node closedNode = closedList.back();

		if (closedNode.y + 1 < height)
		{
			Node& neighbour = grid[closedNode.y + 1][closedNode.x];
			CheckNeighbour(neighbour, closedNode, endNode, openList);
		}

		if (closedNode.x - 1 >= 0)
		{
			Node& neighbour = grid[closedNode.y][closedNode.x - 1];
			CheckNeighbour(neighbour, closedNode, endNode, openList);
		}

		if (closedNode.y - 1 >= 0)
		{
			Node& neighbour = grid[closedNode.y - 1][closedNode.x];
			CheckNeighbour(neighbour, closedNode, endNode, openList);
		}

		if (closedNode.x + 1 < width)
		{
			Node& neighbour = grid[closedNode.y][closedNode.x + 1];
			CheckNeighbour(neighbour, closedNode, endNode, openList);
		}

		Node lowestCostNode = Node();
		FindLowestCostNode(openList, lowestCostNode);
		UpdateLists(closedList, lowestCostNode, openList);

		TraverseBackToStart(closedList, endNode, startNode);
	}

	std::cout << "poza petla" << std::endl;
	std::cout << gridText << std::endl;
	
	return 0;
}


