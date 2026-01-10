#include <iostream>
#include <cmath>
#include "Node.h"

Node::Node()
{
	this->x = -1;
	this->y = -1;
	this->cellType = CELL::ERROR;
	this->fCost = 9999;
}

Node::Node(int x, int y, CELL cellType)
{
	this->x = x;
	this->y = y;
	this->cellType = cellType;
	this->gCost = 1;
}

void Node::PrintPosition()
{
	std::cout << "x is: " << x << " y is: " << y << " cellType = " << cellType << std::endl;
}

void Node::CalculateCost(Node endNode)
{
	this->hCost = sqrt(pow((this->x - endNode.x), 2) + 
		pow((this->y - endNode.y), 2));

	this->fCost = gCost + hCost;
}

bool Node::operator == (const Node& other) const
{
	return x == other.x && y == other.y && cellType == other.cellType;
}

bool Node::operator != (const Node& other) const
{
	return !(*this == other);
}