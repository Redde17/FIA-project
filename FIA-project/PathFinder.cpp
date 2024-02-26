#include "PathFinder.h"
#include <iostream>

void PathFinder::debug_PrintMessage(std::string msg) {
	std::cout << "=== AI debug message start ===" << std::endl;
	std::cout << msg << std::endl;
	std::cout << "===  AI debug message end  ===" << std::endl;
}

float PathFinder::H(Node start, Node target) {
	//best path possibile is the path with less steps
	//not a diagonal but a sum of the distance on the x axis and y axis between the nodes

	float h = 0;
	int hx = start.x - target.x;
	int hy = start.y - target.y;

	h = hx + hy;

	return h;
}

void PathFinder::findPath(int** mapInstance, int xStart, int yStart, int xTarget, int yTarget) {
	this->mapInstace = mapInstace;
	Node* startNode = new Node(xStart, yStart);
	Node* targetNode = new Node(xTarget, yTarget);
	float h = H(*startNode, *targetNode);
	
	std::cout << "h value = " << h << std::endl;
}

