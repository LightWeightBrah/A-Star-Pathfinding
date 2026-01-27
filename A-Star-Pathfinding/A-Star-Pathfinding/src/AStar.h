#pragma once
#include <vector>
#include "Node.h"



class AStar 
{
private:
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
	
	bool drawingPath	 = false;
	bool finishedDrawing = false;
	Node* pathTracker    = nullptr;
	std::vector<Node*> fullPath;

	bool pathFound		 = false;
	int neighbourStep	 = 0;
	float astarCounter	 = 0.0f;
	float astarInterval	 = 0.01f;

	float modelCounter  = 0.0f;
	float modelInterval = 0.4f;
	bool modelFinished  = false;

	int startX	= -1, startY = -1, 
		endX	= -1, endY	 = -1;

	Node* startNode;
	Node* endNode;


public:
	int width = 0, height = 0;
	std::vector<std::vector<Node>> grid;

public:
	AStar();
	AStar(std::string gridFilepath);
	AStar(std::string gridFilepath, int startX, int startY, int endX, int endY);

	void FindPathBySteps(float deltaTime);
	void TravelWithModel(int& x, int& z, float deltaTime);
	void FindPathFull();
	void Reset(int& modelX, int& modelZ);

	inline Node* GetStartNode() const { return startNode; }
	inline Node* GetEndNode()   const { return endNode; }


private:
	bool RunAStarStep();
	void TraverseBackToStartSteps();

	bool RunAStarFull();
	void TraverseBackToStartFull();
	
	void CheckNeighbour(Node* neighbour, Node* currentNode);
	void ResolveConflicts(Node& neighbour, Node& currentNode);

	Node* FindLowestCostNode();
	void UpdateLists(Node* lowestCostNode);
	
	bool ValidateStartEndCoordinates();
	void PrintCoordinates();

	void RemoveItemFromVector(std::vector<Node*>& vec, Node* node);
	bool IsNodeInVector(const std::vector<Node*>& vec, Node* node);


};
