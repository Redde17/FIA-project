#include "PathFinder.h"
#include "CustomPriorityQueue.h"




//private functions
bool AI_Module::PathFinder::NodeComparator::operator()(const Node& n1, const Node& n2) const {
	return n1.f > n2.f;
}

bool AI_Module::PathFinder::InvertedNodeComparator::operator()(const Node& n1, const Node& n2) const {
	return n1.f < n2.f;
}

AI_Module::PathFinder::Node::Node() {
	x = 0;
	y = 0;
}

AI_Module::PathFinder::Node::Node(int x, int y) {
	this->x = x;
	this->y = y;
}

AI_Module::PathFinder::Node::Node(Node* parent, int x, int y) {
	this->parent = parent;
	this->x = x;
	this->y = y;
}

AI_Module::PathFinder::Node::Node(Node* baseNode) {
	this->parent = baseNode->parent;
	this->x = baseNode->x;
	this->x = baseNode->y;
	this->x = baseNode->g;
	this->x = baseNode->h;
	this->x = baseNode->f;
}

bool AI_Module::PathFinder::Node::operator==(const Node& a) const {
	return (x == a.x && y == a.y);
}

void AI_Module::PathFinder::debug_PrintMessage(std::string msg) {
	std::cout << "=== AI debug message start ===" << std::endl;
	std::cout << msg << std::endl;
	std::cout << "===  AI debug message end  ===" << std::endl;
}

float AI_Module::PathFinder::calculateDistance(Node start, Node target) {
	//best path possibile is the path with less steps
	//not a diagonal but a sum of the distance on the x axis and y axis between the nodes

	return abs(start.x - target.x) + abs(start.y - target.y);
}

void AI_Module::PathFinder::reconstructPath(Node* startNode) {
	//std::cout << "Started path reconstruction" << std::endl;
	int x, y;

	while (startNode->parent != NULL)
	{
		//std::cout << "Current node: (" << startNode->x << "," << startNode->y << ") " << std::endl;
		//std::cout << "Current parent node: (" << startNode->parent->x << "," << startNode->parent->y << ") " << std::endl;

		//find action by subtracting x and y
		//could be done probably better if in the node i save
		//the action that the parent makes to reach it
		//not sure if it would work
		x = startNode->x - startNode->parent->x;
		y = startNode->y - startNode->parent->y;

		if(x == 1)
			actionBuffer->push(Action::RIGHT);
		else if(x == -1)
			actionBuffer->push(Action::LEFT);
		else if(y == 1)
			actionBuffer->push(Action::DOWN);
		else if(y == -1)
			actionBuffer->push(Action::UP);
		else //unexpected (x, y) value
			std::cout << "Unexpected x and y combination during path reconstruction" << std::endl;
		startNode = startNode->parent;
	}

	std::cout << "Found path with [" << actionBuffer->size() << "] steps" << std::endl;
}

bool AI_Module::PathFinder::algorithmAstar(std::vector<std::vector<int>> mapInstance, Node startNode, Node targetNode, PathType pathType) {
	pathFinderVision = mapInstance;
	
	//CustomPriorityQueue<Node, std::vector<Node>, NodeComparator> openNodes;
	std::deque<Node> openNodes;
	std::vector<Node> closedNodes;

	startNode.f = calculateDistance(startNode, targetNode);
	startNode.h = PathFinder::calculateDistance(startNode, targetNode);
	openNodes.push_front(startNode);

	while (!openNodes.empty()) {

		//std::cout << "popping openNodes: (" << head.x << " , " << head.y << ") f: " << head.f << std::endl;
		//std::cout << "start position: (" << startNode.x << " , " << startNode.y << ")" << std::endl;
		//std::cout << "target position: (" << targetNode.x << " , " << targetNode.y << ")" << std::endl;

		Node head(0, 0);
		if (pathType == PathType::Short) {
			head = openNodes.front();
			openNodes.pop_front();
		}
		else {
			head = openNodes.back();
			openNodes.pop_back();
		}

		closedNodes.push_back(*new Node(head));
		//change map for visualizer
		pathFinderVision[closedNodes.back().x][closedNodes.back().y] = VisualizerTiles::closedSet;

		if (head == targetNode) {
			reconstructPath(&head);
			return true;
		}

		//generate successors
		//for each successor
		//set position
		//check if already in openNodes
		//set g, h, f and parent.
		//g gets omitted from f calculation, for some reason it creates loops

		Node* successorNode = new Node(0, 0);
		for (int i = 0; i < 4; i++) {
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

			//check if successor is in closedSet
			auto findInClosedNodesResult = find_if(closedNodes.begin(), closedNodes.end(), [&successorNode](const Node& node) {
				return *successorNode == node;
			});
			if (findInClosedNodesResult != closedNodes.end()) {
				//element found
				//std::cout << "successorNode found in closedSet" << std::endl;
				continue;
			}

			//if successor is not empty or apple then skip successor
			if (mapInstance[successorNode->x][successorNode->y] == 0 || mapInstance[successorNode->x][successorNode->y] == 1)
			{
				//include in the heuristic how much map access does the node have
				successorNode->parent = new Node(head);
				//g() is useless in this case, as each node is equally distant from each oter.
				//so if there are two paths to the finish point, with equal h(), calculating g() will not have impact on f()
				//successorNode->g = PathFinder::calculateDistance(startNode, *successorNode); 
				successorNode->h = PathFinder::calculateDistance(*successorNode, targetNode);
				successorNode->f = successorNode->h;

				auto findInOpenNodesResult = find_if(openNodes.begin(), openNodes.end(), [&successorNode](const Node& node) {
					return node == *successorNode;
					});

				//check if successorNode is in openNodes.
				if (findInOpenNodesResult == openNodes.end()) {
					//std::cout << "pushing openNodes." << i << " : (" << successorNode->x << ", " << successorNode->y << ") f : " << successorNode->f << std::endl;
					openNodes.push_front(*successorNode);
					//change map for visualizer
					pathFinderVision[openNodes.front().x][openNodes.front().y] = VisualizerTiles::openSet;

					std::sort(openNodes.begin(), openNodes.end(), [](const Node& n1, const Node& n2) {
						return n1.f < n2.f;
					});
				}
			}
		}
		drawVisualizerMap(this->window);
	}

	return false;
}

bool AI_Module::PathFinder::algorithHamiltonianCycle(std::vector<std::vector<int>> mapInstance, Node start) {
	int pathLenght = mapInstance.size();
	std::vector<Node> path(pathLenght++);
	path[0] = start;
	return findHamiltionianCycle(path, 1);
}

bool AI_Module::PathFinder::findHamiltionianCycle(std::vector<Node> path, int pos) {
	//note: the map can be seen as a graph.
	//each location is a node, and the operation needed to move is a vertex
	//based on this assumption, each node has 4 vertices
	if (pos == path.size())
		return path[path.size() - 1] == path[0];

	//for each valid neghbour
	//get neighbour nodes
	Node* neighbour = new Node(0, 0);
	std::vector<Node> neighbours;
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0: //up
			neighbour->x = path[pos - 1].x - 1;
			neighbour->y = path[pos - 1].y;
			break;
		case 1: //down
			neighbour->x = path[pos - 1].x + 1;
			neighbour->y = path[pos - 1].y;
			break;
		case 2: //left
			neighbour->x = path[pos - 1].x;
			neighbour->y = path[pos - 1].y - 1;
			break;
		case 3: //right
			neighbour->x = path[pos - 1].x;
			neighbour->y = path[pos - 1].y + 1;
			break;
		default:
			//error
			return false;
		}
		neighbours.push_back(*neighbour);
	}

	//explore each neighbour
	for (int i = 1; i < neighbours.size(); i++)
	{
		if (isValidStep()) {
			//TODO
		}
	}

	return false;
}

bool AI_Module::PathFinder::isValidStep() {
	return false;
}

void AI_Module::PathFinder::checkPathMapAccess() {
	//emulate new map with proposed path

	//get percentage of accessible map space for the snake

	//if accessible map is less then a certain percentage find new path
}

//public functions

bool AI_Module::PathFinder::findPath(std::vector<std::vector<int>> mapInstance, int xStart, int yStart, int xTarget, int yTarget, PathType pathType) {
	return algorithmAstar(mapInstance, Node(xStart, yStart), Node(xTarget, yTarget), pathType);
}

//temp testing function for visualizing path generation
void AI_Module::PathFinder::drawVisualizerMap(sf::RenderWindow* window) {
	int mapSizeX = pathFinderVision[0].size();
	int mapSizeY = mapSizeX;
	int gridSize = 30;

	sf::RectangleShape* toBeDrawnTile = NULL;

	//snake tile definition
	sf::RectangleShape* snakeTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	snakeTile->setFillColor(sf::Color(0, 255, 0));

	//apple tile definition
	sf::RectangleShape* appleTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	appleTile->setFillColor(sf::Color(255, 0, 0));

	//wall tile definition
	sf::RectangleShape* wallTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	wallTile->setFillColor(sf::Color(255, 255, 255));

	//error tile definition
	sf::RectangleShape* errorTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	errorTile->setFillColor(sf::Color(0, 0, 255));

	//grid tile definition
	sf::RectangleShape* emptyTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	emptyTile->setOutlineThickness(-0.5f);
	emptyTile->setOutlineColor(sf::Color(145, 145, 145));
	emptyTile->setFillColor(sf::Color(0, 0, 0, 255));


	sf::RectangleShape* openSetTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	openSetTile->setFillColor(sf::Color(52, 229, 235));

	sf::RectangleShape* closedSetTile = new sf::RectangleShape(sf::Vector2f(gridSize, gridSize));
	closedSetTile->setFillColor(sf::Color(89, 52, 235));

	window->clear();
	//can be optimized
	//draw grid
	for (int x = 0; x < mapSizeX; x++) {
		for (int y = 0; y < mapSizeY; y++) {
			switch (pathFinderVision[x][y]) {
			case VisualizerTiles::Empty:
				toBeDrawnTile = emptyTile;
				break;

			case VisualizerTiles::Apple:
				toBeDrawnTile = appleTile;
				break;

			case VisualizerTiles::Wall:
				toBeDrawnTile = wallTile;
				break;

			case VisualizerTiles::SnakeBody:
				toBeDrawnTile = snakeTile;
				break;

			case VisualizerTiles::openSet:
				toBeDrawnTile = openSetTile;
				break;

			case VisualizerTiles::closedSet:
				toBeDrawnTile = closedSetTile;
				break;

			default: //error?
				break;
			}

			toBeDrawnTile->setPosition(x * gridSize, y * gridSize);
			window->draw(*toBeDrawnTile);
		}
	}
	window->display();
	std::this_thread::sleep_for(20ms);
}

AI_Module::PathFinder::PathFinder(sf::RenderWindow* window) {
	this->window = window;
	actionBuffer = new std::stack<Action>();
}

AI_Module::PathFinder::~PathFinder() {
	//not sure if it works this way
	//actionBuffer->empty();
	delete[] actionBuffer;
}