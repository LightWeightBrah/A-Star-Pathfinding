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
R"(0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0 0 0 0 0 0 0 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 1
5 5 5 5 5 0 0 0 0 0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1
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
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
0 0 0 0 0 0 5 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
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

std::string TrimGrid(std::string& gridText)
{
	std::string output  = gridTextTemplate30x20;
	output.erase(std::remove(output.begin(), output.end(), ' '), output.end());
	std::cout << output << std::endl;
	return output;
}

void RemoveItemFromVector(std::vector<Node>& vec, Node node)
{
	vec.erase(std::remove(vec.begin(), vec.end(),
		node), vec.end());
}

int main()
{
	std::string gridText = TrimGrid(gridText);

	int width = 0;
	int height = 1;
	GetGridDimensions(gridText, width, height);

	std::vector<std::vector<Node>> grid = InitGrid(gridText, width, height);
	Node& startNode = grid[0][0];
	Node& endNode = grid[height - 1][width - 1];

	std::vector<Node> openList;
	std::vector<Node> closedList;
	closedList.push_back(startNode);

	//TESTING: delete line later
	closedList.push_back(grid[3][28]);

	while (closedList.back() != endNode)
	{
		Node closedNode = closedList.back();
		if (closedNode.y + 1 < height)
		{
			Node& bottomNeighbour = grid[closedNode.y + 1][closedNode.x];
			openList.push_back(bottomNeighbour);
			
			std::cout << "bottom neighbour " << std::endl;
			bottomNeighbour.PrintPosition();
		}

		if (closedNode.x - 1 >= 0)
		{
			Node& leftNeighbour = grid[closedNode.y][closedNode.x - 1];
			openList.push_back(leftNeighbour);

			std::cout << "left neighbour" << std::endl;
			leftNeighbour.PrintPosition();
		}

		if (closedNode.y - 0 >= 0)
		{
			Node& topNeighbour = grid[closedNode.y - 1][closedNode.x];
			openList.push_back(topNeighbour);

			std::cout << "top neighbour" << std::endl;
			topNeighbour.PrintPosition();
		}

		if (closedNode.x + 1 < width)
		{
			Node& rightNeighbour = grid[closedNode.y][closedNode.x + 1];
			openList.push_back(rightNeighbour);

			std::cout << "right neighbour" << std::endl;
			rightNeighbour.PrintPosition();
		}

		Node lowestCostNode = Node();
		for (int i = 0; i < openList.size(); i++)
		{
			if (openList[i].fCost < lowestCostNode.fCost)
			{
				lowestCostNode = openList[i];
			}
		}

		closedList.push_back(lowestCostNode);
		RemoveItemFromVector(openList, lowestCostNode);






		//TESTING: get out of loop 
		closedList.push_back(grid[height - 1][width - 1]);
	}

	std::cout << "poza petla" << std::endl;


	return 0;
}


