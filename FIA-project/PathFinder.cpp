#include "PathFinder.h"
#include <iostream>
#include "CustomPriorityQueue.h"

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


	return abs(start.x - target.x) + abs(start.y - target.y);
}


bool PathFinder::findPath(std::vector<std::vector<int>> mapInstance, int xStart, int yStart, int xTarget, int yTarget) {
	//this->mapInstace = mapInstace;
	Node startNode(xStart, yStart);
	Node targetNode(xTarget, yTarget);

	CustomPriorityQueue<Node, std::vector<Node>, NodeComparator> openNodes;
	std::vector<Node> closedNodes;
	
	startNode.f = calculateDistance(startNode, targetNode);
	startNode.h = PathFinder::calculateDistance(startNode, targetNode);
	openNodes.push(startNode);


	while (!openNodes.empty()){
		Node head = openNodes.top();
		std::cout << "popping openNodes: (" << head.x << " , " << head.y << ") f: " << head.f << std::endl;
		std::cout << "start position: (" << startNode.x << " , " << startNode.y << ")" << std::endl;
		std::cout << "target position: (" << targetNode.x << " , " << targetNode.y << ")" << std::endl;
		openNodes.pop();
		closedNodes.push_back(*new Node(head));

		if (head.x == targetNode.x && head.y == targetNode.y) {
			//reconstruct path
			std::cout << "Started path reconstruction" << std::endl;
			Node pathNode = head;
			int x, y;

			while (pathNode.parent != NULL)
			{
				std::cout << "Current node: (" << pathNode.x << "," << pathNode.y << ") " << std::endl;
				std::cout << "Current parent node: (" << pathNode.parent->x << "," << pathNode.parent->y << ") " << std::endl;

				//find action by subtracting x and y
				//could be done probably better if in the node i save
				//the action that the parent makes to reach it
				//not sure if it would work
				x = pathNode.x - pathNode.parent->x;
				y = pathNode.y - pathNode.parent->y;

				switch (x)
				{
				case 1:
					//down
					actionBuffer->push(Action::RIGHT);
					break;
				case -1:
					//up
					actionBuffer->push(Action::LEFT);
					break;
				default:
					//0
					switch (y)
					{
					case 1:
						//right
						actionBuffer->push(Action::DOWN);
						break;
					case -1:
						//left
						actionBuffer->push(Action::UP);
						break;
					default:
						std::cout << "Unexpected x and y combination during path reconstruction" << std::endl;
						break;
					}
					break;
				}
				pathNode = *pathNode.parent;
			}

			std::cout << "Finished path reconstruction with [" << actionBuffer->size() << "] steps" << std::endl;
			return true;
		}

		//generate successors
		//for each successor
		//set position
		//check if already in openNodes
		//set g, h, f and parent.
		//g gets omitted from f calculation, for some reason it creates loops

		Node* successorNode = new Node(0, 0);
		for (int i = 0; i < 4; i++){
			switch (i)
			{
			case 0: //up
				successorNode->x = head.x - 1;
				successorNode->y = head.y;
				break;
			case 1: //down
				successorNode->x = head.x + 1;
				successorNode->y = head.y;
				break;
			case 2: //left
				successorNode->x = head.x;
				successorNode->y = head.y - 1;
				break;
			case 3: //right
				successorNode->x = head.x;
				successorNode->y = head.y + 1;
				break;
			default:
				//error
				return false;
			}

			////check if successor is in closedSet
			auto it = find_if(closedNodes.begin(), closedNodes.end(), [&successorNode](const Node& node) {
				return node.x == successorNode->x && node.y == successorNode->y;
				});
			if (it != closedNodes.end()) {
				//element found
				std::cout << "successorNode found in closedSet" << std::endl;
				continue;
			}

			//if successor is not empty or apple then skip successor
			if (mapInstance[successorNode->x][successorNode->y] == 0 || mapInstance[successorNode->x][successorNode->y] == 1)
			{
				successorNode->parent = new Node(head);
				successorNode->g = successorNode->parent->g + 1;
				successorNode->h = PathFinder::calculateDistance(*successorNode, targetNode);
				successorNode->f = successorNode->h + successorNode->g;

				//check if successorNode is in openNodes.
				if (!openNodes.findNode(*successorNode)) {
					std::cout << "pushing openNodes." << i << " : (" << successorNode->x << ", " << successorNode->y << ") f : " << successorNode->f << std::endl;
					openNodes.push(*successorNode);
				}
			}
		}
	}

	return false;
}

