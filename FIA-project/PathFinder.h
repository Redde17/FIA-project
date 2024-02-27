#pragma once
#include <string>
#include <stack>

class PathFinder
{
	struct Node {
		int x;
		int y;

		Node(int x, int y) {
			this->x = x;
			this->y = y;
		}
	};

	int** mapInstace;
	//std::list<Node> finalPath;

	float H(Node start, Node target);
	void G(Node node);

	void algorithmAstar(Node start, Node target, float h);

	void debug_PrintMessage(std::string msg);

public:
	enum Action {
		UP = 0,
		DOWN = 1,
		LEFT = 2,
		RIGHT = 3
	};

	std::stack<Action>* actionBuffer;

	PathFinder();
	~PathFinder();

	void findPath(int** mapInstance, int xStart, int yStart, int xTarget, int yTarget);
};

