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

const std::string gridTemplate6x6 = R"(0 0 0 0 0 0
0 5 0 0 0 0
0 0 5 0 0 0
0 0 0 5 0 0
0 0 0 0 5 0
0 0 0 0 0 0)";

#pragma endregion

void PrintGrid(std::vector<std::vector<Node>> grid)
{
	for (const auto& row : grid)
	{
		for (const auto& node : row)
		{
			std::cout << (int)node.cellType << " ";
		}
		std::cout << std::endl;
	}
}

void RenderMap(std::vector<std::vector<Node>>& grid, Node* startNode, Node* endNode)
{
	for (const auto& row : grid)
	{
		for (const auto& node : row)
		{
			if (&node == startNode)
				std::cout << "BB";
			else if (&node == endNode)
				std::cout << "EE";
			else
			{
				switch (node.cellType)
				{
				case CELL::WALL:
					std::cout << "##";
					break;
				case CELL::ROUTE:
					std::cout << "++";
					break;
				default:
					std::cout << "..";
					break;
				}
			}
		}
		std::cout << std::endl;
	}
}

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


	PrintGrid(grid);



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

void RemoveItemFromVector(std::vector<Node*>& vec, Node* node)
{
	vec.erase(std::remove(vec.begin(), vec.end(), node), vec.end());
}


bool IsNodeInVector(std::vector<Node*>& vec, Node* node)
{
	for (Node* n : vec) 
	{
		if (n == node) 
			return true;
	}
	return false;
}

void ResolveConflicts(std::vector<std::vector<Node>>& grid, Node& neighbour, Node& closedNode, Node& endNode)
{
	double gCost = closedNode.gCost + 1;
	if (neighbour.fCost != NULL)
	{
		double hCost = neighbour.CalculateHCost(
			neighbour.x, neighbour.y,
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
	else
	{
		neighbour.gCost = gCost;
		neighbour.hCost = neighbour.CalculateHCost(
			neighbour.x, neighbour.y,
			endNode.x, endNode.y);

		neighbour.fCost = gCost + neighbour.hCost;
		neighbour.SetParent(&grid[closedNode.y][closedNode.x]);

	}
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

void UpdateLists(std::vector<Node*>& closedList, std::vector<Node*>& openList, Node* lowestCostNode)
{
	if (lowestCostNode == nullptr)
		return;

	RemoveItemFromVector(openList, lowestCostNode);
	lowestCostNode->cellType = CELL::CLOSED;
	closedList.push_back(lowestCostNode);
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

void TraverseBackToStart(std::vector<std::vector<Node>>& grid, std::vector<Node*>& closedList, std::vector<Node*>& openList, Node* startNode, Node* endNode)
{
	if (closedList.back() == endNode)
	{

		Node* currentNode = closedList.back();
		while (currentNode != nullptr && currentNode != startNode)
		{
			currentNode->cellType = CELL::ROUTE;
			currentNode = currentNode->parent;
		}
		startNode->cellType = CELL::ROUTE;
	}
	else
	{
		std::cout << "Nie mozna znalezc drogi od startu do konca. Zmien grida" << std::endl;
	}
}

int main()
{
	std::string gridText = TrimGrid(gridTemplate6x6);

	int width = 0;
	int height = 1;
	GetGridDimensions(gridText, width, height);

	std::vector<std::vector<Node>> grid = InitGrid(gridText, width, height);
	Node* startNode = &grid[2][1];
	Node* endNode = &grid[1][width - 1];
	(*startNode).PrintPosition();
	(*endNode).PrintPosition();

	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	closedList.push_back(startNode);


	do
	{
		Node* closedNode = closedList.back();

		if (closedNode == endNode)
			break;

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

		Node* lowestCostNode = FindLowestCostNode(openList);
		if (lowestCostNode == nullptr)
			break;

		UpdateLists(closedList, openList, lowestCostNode);
		std::cout << "oepn list size is: " << openList.size() << std::endl;
	} 
	while (!openList.empty());

	TraverseBackToStart(grid, closedList, openList, startNode, endNode);
	std::cout << "poza petla" << std::endl;
	RenderMap(grid, startNode, endNode);

	
	return 0;
}