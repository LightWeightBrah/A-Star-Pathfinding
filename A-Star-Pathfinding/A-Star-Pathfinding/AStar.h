#pragma once
#include <vector>
#include "Node.h"

void FindPath(std::vector<std::vector<Node>>& grid, int width, int height, int startX = -1, int startY = -1, int endX = -1, int endY = -1);
bool RunAStar(std::vector<std::vector<Node>>& grid, Node* startNode, Node* endNode, const int& width, const int& height);
void CheckNeighbour(std::vector<std::vector<Node>>& grid, Node* neighbour, Node* closedNode, Node* endNode, std::vector<Node*>& openList, std::vector<Node*>& closedList);
void ResolveConflicts(std::vector<std::vector<Node>>& grid, Node& neighbour, Node& closedNode, Node& endNode);
Node* FindLowestCostNode(std::vector<Node*>& openList);
void UpdateLists(std::vector<Node*>& closedList, std::vector<Node*>& openList, Node* lowestCostNode);
void TraverseBackToStart(std::vector<std::vector<Node>>& grid, Node* startNode, Node* endNode);

void RemoveItemFromVector(std::vector<Node*>& vec, Node* node);
bool IsNodeInVector(const std::vector<Node*>& vec, Node* node);