#pragma once
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <list>

class PathFinder
{
	struct Node {
		int x;
		int y;
		float g = 0;
		float h = 0;
		float f = 0;
		Node* parent = NULL;

		Node(int x, int y) {
			this->x = x;
			this->y = y;
		}

		Node(Node* parent, int x, int y) {
			this->parent = parent;
			this->x = x;
			this->y = y;
		}

		Node(Node* baseNode) {
			this->parent = baseNode->parent;
			this->x = baseNode->x;
			this->x = baseNode->y;
			this->x = baseNode->g;
			this->x = baseNode->h;
			this->x = baseNode->f;
		}
	};

	template<
		class T,
		class Container = std::vector<T>,
		class Compare = std::less<typename Container::value_type>
> class CustomPriorityQueue : public std::priority_queue<T, Container, Compare>
	{
	public:
		typedef typename
			std::priority_queue<
			T,
			Container,
			Compare>::container_type::const_iterator const_iterator;

		bool findNode(const Node& val) const
		{
			auto first = this->c.cbegin();
			auto last = this->c.cend();
			while (first != last) {
				if (((Node)*first).x == val.x && ((Node)*first).y == val.y) return true;
				++first;
			}
			
			return false;
		}
	};



	struct NodeComparator {
		bool operator()(const Node& n1, const Node& n2) const {
			return n1.f > n2.f;
		}
	};

	//int** mapInstace;
	//std::list<Node> finalPath;

	static float calculateDistance(Node start, Node target);

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

	bool findPath(std::vector<std::vector<int>> mapInstance, int xStart, int yStart, int xTarget, int yTarget);
};

