#pragma once
#include "Maze.h"

#include <vector>
#include <queue>
#include <map>
#include <unordered_set>

class MazeSolver
{
	struct Comparator {
		std::map<Maze::Node*, float>& score;

		Comparator(std::map<Maze::Node*, float>& score);
		bool operator()(Maze::Node* leftNode, Maze::Node* rightNode) const;
	};

	static std::vector<Maze::Node*> aStarAlgorithm(Maze* maze, Maze::Node* start, Maze::Node* target);
	static std::vector<Maze::Node*> getNeighbours(Maze* maze, Maze::Node* currentNode);
	static std::vector<Maze::Node*> recostructPath();
	static float getStraighLineDistance(Maze::Node* start, Maze::Node* target);

public:
	enum PathFinders {
		AStar
	};

	static std::vector<Maze::Node*> findPath(PathFinders algorithm, Maze* maze, Maze::Node* start, Maze::Node* target);
};

