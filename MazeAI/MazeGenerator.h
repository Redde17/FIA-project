#pragma once
#include "Maze.h"

#include <stack>
#include <time.h>

static class MazeGenerator
{
	static Maze generateRandomDepthFirstMaze(const int& sizeX, const int& sizeY);
	static Maze::Node* getRandomNeighbor(Maze* newMaze, Maze::Node* currentNode);
	static void deleteWall(Maze::Node* fromNode, Maze::Node* toNode);

public:
	enum Generators {
		RandomDepthFirst
	};

	static Maze generateMaze(Generators generator, const int& sizeX, const int& sizeY);
};

