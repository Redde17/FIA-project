#include "MazeSolver.h"
#include <iostream>

MazeSolver::Comparator::Comparator(std::map<Maze::Node*, float>& score) : score(score) {}


bool MazeSolver::Comparator::operator()(Maze::Node* leftNode, Maze::Node* rightNode) const {
	return score.at(leftNode) > score.at(rightNode);
}

std::vector<Maze::Node*> MazeSolver::aStarAlgorithm(Maze* maze, Maze::Node* start, Maze::Node* target) {
	std::map<Maze::Node*, Maze::Node*> cameFrom;
	std::map<Maze::Node*, float> gScore;
	std::map<Maze::Node*, float> fScore;

	//Comparator comparator(fScore);
	//std::priority_queue<Maze::Node*, std::vector<Maze::Node*>, Comparator> openNodes(comparator);

	std::vector<Maze::Node*> openNodes;

	openNodes.push_back(start);
	gScore[start] = 0;
	fScore[start] = getStraighLineDistance(start, target);

	auto minComparator = [&fScore](Maze::Node* n1, Maze::Node* n2) {
		return fScore.at(n1) < fScore.at(n2);
	};

	Maze::Node* currentNode;
	while (!openNodes.empty()) {
		//get node with lowest fScore
		auto minNoteIt = std::min_element(openNodes.begin(), openNodes.end(), minComparator);
		currentNode = *minNoteIt;
		openNodes.erase(minNoteIt);

		//if current node is goal then reconstruct path
		if (currentNode == target) {
			std::cout << "Path found" << std::endl;
			return std::vector<Maze::Node*>();
			//return recostructPath();
		}

		//get neighbors
		std::vector<Maze::Node*> neighbors = getNeighbours(maze, currentNode);
		std::cout << "found ["<< neighbors.size() <<"] neighbors" << std::endl;

		//for each neighbor check for score and add in openSet if not present
		for (Maze::Node* neighbor : neighbors) {
			float tentativeNeighborGscore = gScore[currentNode] + 1; //value of the movement between nodes is constant
			if (tentativeNeighborGscore < gScore[neighbor]) {
				cameFrom[neighbor] = currentNode;
				gScore[neighbor] = tentativeNeighborGscore;
				fScore[neighbor] = tentativeNeighborGscore + getStraighLineDistance(neighbor, target);
				
				auto findComparator = [&neighbor](const Maze::Node* openNode) {
					return neighbor == openNode;
				};
				if (std::find_if(openNodes.begin(), openNodes.end(), findComparator) != openNodes.end()) {
					std::cout << "Not found node in openNodes, pushing" << std::endl;
					openNodes.push_back(neighbor);
				}
			}
		}
		std::cout << "A* working" << std::endl;
	}
	return std::vector<Maze::Node*>();
}

std::vector<Maze::Node*> MazeSolver::getNeighbours(Maze* maze, Maze::Node* currentNode) {
	std::vector<Maze::Node*> neighbours;

	if (!currentNode->walls[Maze::Directions::LEFT] && !maze->gameMap[currentNode->pos.x - 1][currentNode->pos.y].isGameWall)
		neighbours.push_back(&maze->gameMap[currentNode->pos.x - 1][currentNode->pos.y]);

	if (!currentNode->walls[Maze::Directions::UP] && !maze->gameMap[currentNode->pos.x][currentNode->pos.y - 1].isGameWall)
		neighbours.push_back(&maze->gameMap[currentNode->pos.x][currentNode->pos.y - 1]);

	if (!currentNode->walls[Maze::Directions::RIGHT] && !maze->gameMap[currentNode->pos.x + 1][currentNode->pos.y].isGameWall)
		neighbours.push_back(&maze->gameMap[currentNode->pos.x + 1][currentNode->pos.y]);

	if (!currentNode->walls[Maze::Directions::DOWN] && !maze->gameMap[currentNode->pos.x][currentNode->pos.y + 1].isGameWall)
		neighbours.push_back(&maze->gameMap[currentNode->pos.x][currentNode->pos.y + 1]);
	
	return neighbours;
}

float MazeSolver::getStraighLineDistance(Maze::Node* start, Maze::Node* target) {
	float xDiff = abs(start->pos.x - target->pos.x);
	float yDiff = abs(start->pos.y - target->pos.y);

	return sqrt(pow(xDiff, 2) + pow(yDiff, 2));
}

std::vector<Maze::Node*> MazeSolver::findPath(PathFinders algorithm, Maze* maze, Maze::Node* start, Maze::Node* target) {
	switch (algorithm){
		case MazeSolver::AStar:
			return aStarAlgorithm(maze, start, target);
		default: {
			throw "Path finder algorithm type not defined";
			return std::vector<Maze::Node*>();
		}
	}
}