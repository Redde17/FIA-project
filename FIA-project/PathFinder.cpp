#include "PathFinder.h"
#include <iostream>
#include <queue>
#include <list>

void PathFinder::debug_PrintMessage(std::string msg) {
	std::cout << "=== AI debug message start ===" << std::endl;
	std::cout << msg << std::endl;
	std::cout << "===  AI debug message end  ===" << std::endl;
}

PathFinder::PathFinder() {
	actionBuffer = new std::stack<Action>();
}

PathFinder::~PathFinder() {
	//not sure if it works this way
	//actionBuffer->empty();
	delete[] actionBuffer;
}

float PathFinder::calculateDistance(Node start, Node target) {
	//best path possibile is the path with less steps
	//not a diagonal but a sum of the distance on the x axis and y axis between the nodes
	return abs(abs(start.x - target.x) + abs(start.y - target.y));
}

bool PathFinder::findPath(int** &mapInstance, int xStart, int yStart, int xTarget, int yTarget) {
	//this->mapInstace = mapInstace;
	Node startNode(xStart, yStart);
	Node targetNode(xTarget, yTarget);

	std::priority_queue<Node, std::vector<Node>, NodeComparator> unexploredNodes;
	std::list<Node> exploredNodes;
	
	startNode.h = calculateDistance(startNode, targetNode);
	unexploredNodes.push(startNode);


	while (!unexploredNodes.empty()){
		Node head = unexploredNodes.top();
		unexploredNodes.pop();

		if (head.x == targetNode.x && head.y == targetNode.y) {
			return true;
		}

		//generate successors
		//up
		if (mapInstace[head.x][head.y - 1] == 0 || mapInstace[head.x][head.y - 1] == 1) {
			Node successorUp(&head, head.x, head.y - 1);
			successorUp.g = calculateDistance(startNode, successorUp);
			successorUp.h = calculateDistance(successorUp, targetNode);
			successorUp.f = successorUp.g + successorUp.h;
			unexploredNodes.push(successorUp);
		}

		//down
		if (mapInstace[head.x][head.y + 1] == 0 || mapInstace[head.x][head.y - 1] == 1) {
			Node successorDown(&head, head.x, head.y + 1);
			successorDown.g = calculateDistance(startNode, successorDown);
			successorDown.h = calculateDistance(successorDown, targetNode);
			successorDown.f = successorDown.g + successorDown.h;
			unexploredNodes.push(successorDown);
		}

		//left
		if (mapInstace[head.x - 1][head.y] == 0 || mapInstace[head.x][head.y - 1] == 1) {
			Node successorLeft(&head, head.x -1, head.y);
			successorLeft.g = calculateDistance(startNode, successorLeft);
			successorLeft.h = calculateDistance(successorLeft, targetNode);
			successorLeft.f = successorLeft.g + successorLeft.h;
			unexploredNodes.push(successorLeft);
		}

		//right
		if (mapInstace[head.x + 1][head.y] == 0 || mapInstace[head.x][head.y - 1] == 1) {
			Node successorRight(&head, head.x + 1, head.y);
			successorRight.g = calculateDistance(startNode, successorRight);
			successorRight.h = calculateDistance(successorRight, targetNode);
			successorRight.f = successorRight.g + successorRight.h;
			unexploredNodes.push(successorRight);
		}

	}

	return false;
}

