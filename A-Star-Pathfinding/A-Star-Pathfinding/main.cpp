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

void RemoveItemFromVector(std::vector<Node>& vec, Node& node)
{
	auto it = std::find(vec.begin(), vec.end(), node);

	if (it != vec.end()) 
		vec.erase(it);
}

int CountItemInVector(std::vector<Node>& vec, Node& node)
{
	return count(vec.begin(), vec.end(), node);
}

void ResolveConflicts(Node& neighbour, Node& closedNode, Node& endNode, int& gCounter)
{
	if (neighbour.fCost != NULL)
	{
		double hCost = neighbour.CalculateHCost(
			neighbour.x, neighbour.y,
			endNode.x, endNode.y);

		int newFCost = gCounter + hCost;

		if (newFCost < neighbour.fCost)
		{
			neighbour.gCost = gCounter;
			neighbour.hCost = hCost;
			neighbour.fCost = newFCost;
			neighbour.SetParent(&closedNode);
		}
	}
	else
	{
		//zle bierze gCost
		neighbour.gCost = gCounter;
		neighbour.hCost = neighbour.CalculateHCost(
			neighbour.x, neighbour.y,
			endNode.x, endNode.y);

		neighbour.fCost = gCounter + neighbour.hCost;
		neighbour.SetParent(&closedNode);
	}
}

void CheckNeighbour(Node& neighbour, Node& closedNode, Node& endNode, std::vector<Node>& openList, int& gCounter)
{
	ResolveConflicts(neighbour, closedNode, endNode, gCounter);
	openList.push_back(neighbour);
}

void UpdateLists(std::vector<Node>& closedList, Node& lowestCostNode, std::vector<Node>& openList)
{
	if (lowestCostNode == NULL)
		return;

	lowestCostNode.cellType == CELL::ROUTE;
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
			//(*currentNode).cellType = CELL::ROUTE;
			currentNode = (*currentNode).parent;
		}
	}
}

int main()
{
	std::string gridText = TrimGrid(gridTemplate6x6);

	int width = 0;
	int height = 1;
	GetGridDimensions(gridText, width, height);

	std::vector<std::vector<Node>> grid = InitGrid(gridText, width, height);
	Node& startNode = grid[2][1];
	Node& endNode = grid[1][width - 1];
	startNode.PrintPosition();
	endNode.PrintPosition();

	std::vector<Node> openList;
	std::vector<Node> closedList;
	closedList.push_back(startNode);

	int gCounter = 1;

	do
	{
		Node closedNode = closedList.back();

		if (closedNode.y + 1 < height && 
			grid[closedNode.y + 1][closedNode.x].cellType == CELL::NO_WALL
			&& grid[closedNode.y + 1][closedNode.x].cellType == 0)
		{
			Node& neighbour = grid[closedNode.y + 1][closedNode.x];
			CheckNeighbour(neighbour, closedNode, endNode, openList, gCounter);
		}

		if (closedNode.x - 1 >= 0 &&
			grid[closedNode.y][closedNode.x - 1].cellType == CELL::NO_WALL
			 && CountItemInVector(closedList, grid[closedNode.y][closedNode.x - 1]) == 0)
		{
			Node& neighbour = grid[closedNode.y][closedNode.x - 1];
			CheckNeighbour(neighbour, closedNode, endNode, openList, gCounter);
		}

		if (closedNode.y - 1 >= 0 &&
			grid[closedNode.y - 1][closedNode.x].cellType == CELL::NO_WALL
			&& CountItemInVector(closedList, grid[closedNode.y - 1][closedNode.x]) == 0)
		{
			Node& neighbour = grid[closedNode.y - 1][closedNode.x];
			CheckNeighbour(neighbour, closedNode, endNode, openList, gCounter);
		}

		if (closedNode.x + 1 < width &&
			grid[closedNode.y][closedNode.x + 1].cellType == CELL::NO_WALL
			&& CountItemInVector(closedList, grid[closedNode.y][closedNode.x + 1]) == 0)
		{
			Node& neighbour = grid[closedNode.y][closedNode.x + 1];
			CheckNeighbour(neighbour, closedNode, endNode, openList, gCounter);
		}

		Node lowestCostNode = Node();
		FindLowestCostNode(openList, lowestCostNode);
		UpdateLists(closedList, lowestCostNode, openList);
		//TraverseBackToStart(closedList, endNode, startNode);
		std::cout << "oepn list size is: " << openList.size() << std::endl;

		gCounter++;
	} 
	while (openList.size() > 0);

	std::cout << "poza petla" << std::endl;
	//PrintGrid()

	for (int i = 0; i < closedList.size(); i++)
	{
		closedList[i].PrintPosition();
	}
	
	return 0;
}


