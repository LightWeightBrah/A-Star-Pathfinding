#pragma once

enum CELL {
	WALL = 5,
	NO_WALL = 0,
	ROUTE = 1,
	ERROR = -1
};

struct Node
{
	Node* parent;
	CELL cellType;

	int x, y;
	double fCost, gCost, hCost;

	Node();
	Node(int x, int y, CELL cellType);


	void PrintPosition();
	void CalculateCost(Node endNode);

	bool operator == (const Node& other) const;
	bool operator != (const Node& other) const;
};