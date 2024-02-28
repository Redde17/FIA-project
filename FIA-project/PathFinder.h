#pragma once
#include <string>
#include <stack>

class PathFinder
{
	struct Node {
		int x;
		int y;
		float g = 0;
		float h = 0;
		float f = 0;
		Node* parent;

		Node(int x, int y) {
			this->x = x;
			this->y = y;
		}

		Node(Node* parent, int x, int y) {
			this->parent = parent;
			this->x = x;
			this->y = y;
		}
	};

	struct NodeComparator {
		bool operator()(const Node& n1, const Node& n2) const {
			return n1.f < n2.f;
		}
	};

	int** mapInstace;
	//std::list<Node> finalPath;

	float calculateDistance(Node start, Node target);

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

	bool findPath(int** &mapInstance, int xStart, int yStart, int xTarget, int yTarget);
};

