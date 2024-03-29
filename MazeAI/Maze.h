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
		//in order from 0 to 3, left up right down
		std::vector<bool> walls{ true, true, true, true };
		Position pos;
		bool isGameWall = false;

		//generation variables
		bool isExplored = false;

		//Search variables
		bool isPath = false;
		bool hasBeenVisited = false;

		bool operator==(const Node& node)const;
	};

	std::vector<std::vector<Node>> gameMap;
	Position startingNode;
	Position endingNode;

	Maze();
	Maze(const int& sizeX, const int& sizeY);

	void setStartingNode(const Node &node);
	void setEndingNode(const Node& node);
};

