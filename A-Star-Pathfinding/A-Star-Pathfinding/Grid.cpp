#include "Grid.h"
#include <iostream>

void RenderMap(const std::vector<std::vector<Node>>& grid, Node* startNode, Node* endNode)
{
	for (const auto& row : grid)
	{
		for (const auto& node : row)
		{
			if (&node == startNode)
				std::cout << "OO";
			else if (&node == endNode)
				std::cout << "XX";
			else
			{
				switch (node.cellType)
				{
				case CELL::WALL:
					std::cout << "||";
					break;
				case CELL::ROUTE:
					std::cout << "->";
					break;
				default:
					std::cout << "  ";
					break;
				}
			}
		}
		std::cout << std::endl;
	}
}

void PrintGrid(const std::vector<std::vector<Node>>& grid)
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

std::vector<std::vector<Node>> SetupWorld(std::string templateString, int& width, int& height)
{
	std::string cleanText = TrimGrid(templateString);
	width = 0;
	height = 1;
	GetGridDimensions(cleanText, width, height);
	return InitGrid(cleanText, width, height);
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

		if (!singleRow.empty())
			grid.push_back(singleRow);
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

}

std::string TrimGrid(std::string girdTemplate)
{
	std::string output = girdTemplate;
	output.erase(std::remove(output.begin(), output.end(), ' '), output.end());
	return output;
}