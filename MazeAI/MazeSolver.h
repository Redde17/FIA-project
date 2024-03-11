#pragma once
#include "Maze.h"

#include <vector>
#include <queue>
#include <map>
#include <unordered_set>
#include <stack>

class MazeSolver
{
	struct Comparator {
		std::map<Maze::Node*, float>& score;

		Comparator(std::map<Maze::Node*, float>& score);
		bool operator()(Maze::Node* leftNode, Maze::Node* rightNode) const;
	};

	static std::stack<Maze::Node*> aStarAlgorithm(Maze* maze, Maze::Node* start, Maze::Node* target);
	static std::vector<Maze::Node*> getNeighbours(Maze* maze, Maze::Node* currentNode);
	static std::stack<Maze::Node*> recostructPath(std::map<Maze::Node*, Maze::Node*>* cameFrom, Maze::Node* current);
	static float getStraighLineDistance(Maze::Node* start, Maze::Node* target);

public:
	enum PathFinders {
		AStar
	};

	static std::stack<Maze::Node*> findPath(PathFinders algorithm, Maze* maze, Maze::Node* start, Maze::Node* target);
};

