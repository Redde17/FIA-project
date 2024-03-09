#pragma once
#include <vector>

class Maze
{
public:
	enum Directions {
		LEFT = 0,
		UP = 1,
		RIGHT = 2,
		DOWN = 3
	};

	struct Position {
		int x;
		int y;

		Position();
		Position(const int& x, const int& y);
	};

	struct Node {
		Position pos;
		//in order from 0 to 3, left up right down
		std::vector<bool> walls{true, true, true, true};
	};

	std::vector<std::vector<Node>> gameMap;

	Maze(const int& sizeX, const int& sizeY);
};

