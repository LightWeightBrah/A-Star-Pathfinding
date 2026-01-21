#pragma once
#include <vector>
#include "Node.h"



class AStar 
{
private:
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	bool pathFound = false;

	Node* startNode;
	Node* endNode;

public:
	int width = 0, height = 0;
	std::vector<std::vector<Node>> grid;

public:
	AStar();
	AStar(std::string gridFilepath);

	void FindPathBySteps(int startX, int startY, int endX, int endY);
	void FindPathFull(int startX = -1, int startY = -1, int endX = -1, int endY = -1);

	inline Node* GetStartNode() const { return startNode; }
	inline Node* GetEndNode()   const { return startNode; }


private:
	bool RunAStarFull();
	bool RunAStarStep(int step);

	void CheckNeighbour(Node* neighbour, Node* closedNode);
	void ResolveConflicts(Node& neighbour, Node& closedNode);
	Node* FindLowestCostNode();
	void UpdateLists(Node* lowestCostNode);
	void TraverseBackToStart();
	
	bool ValidateStartEndCoordinates(int& startX, int& startY, int& endX, int& endY);
	void PrintCoordinates(int& startX, int& startY, int& endX, int& endY);

	void RemoveItemFromVector(std::vector<Node*>& vec, Node* node);
	bool IsNodeInVector(const std::vector<Node*>& vec, Node* node);


};
