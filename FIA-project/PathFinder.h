#pragma once
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <list>


namespace AI_Module {


	class PathFinder{
	public:
		enum PathType {
			Short,
			Long
		};

		enum Action {
			UP = 0,
			DOWN = 1,
			LEFT = 2,
			RIGHT = 3
		};

		struct Node {
			int x;
			int y;
			float g = 0;
			float h = 0;
			float f = 0;
			Node* parent = NULL;

			Node() {
				x = 0;
				y = 0;
			}

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

			bool operator==(const Node& a) const {
				return (x == a.x && y == a.y);
			}

			Node& operator=(const Node& a) {
				x = a.x;
				y = a.y;

				return *this;
			}
		};

		struct NodeComparator {
			bool operator()(const Node& n1, const Node& n2) const;
		};

		struct InvertedNodeComparator {
			bool operator()(const Node& n1, const Node& n2) const;
		};

	private:

		void debug_PrintMessage(std::string msg);

		float calculateDistance(Node start, Node target);
		void reconstructPath(Node* startNode);

		bool algorithmAstar(std::vector<std::vector<int>> mapInstance, Node start, Node target, PathType pathType);
		bool algorithHamiltonianCycle(std::vector<std::vector<int>> mapInstance, Node start);
		bool findHamiltionianCycle(std::vector<Node> path, int pos);
		bool isValidStep();
	public:
		std::stack<Action>* actionBuffer;
		Action lastActionPerformed;

		PathFinder();
		~PathFinder();

		bool findPath(std::vector<std::vector<int>> mapInstance, int xStart, int yStart, int xTarget, int yTarget, PathType pathType);

	};

	template<
		class T,
		class Container = std::vector<T>,
		class Compare = std::less<typename Container::value_type>>
		class CustomPriorityQueue : public std::priority_queue<T, Container, Compare>
	{
	public:
		typedef typename
			std::priority_queue<
			T,
			Container,
			Compare>::container_type::const_iterator const_iterator;

		bool findNode(const PathFinder::Node& val) const {
			auto first = this->c.cbegin();
			auto last = this->c.cend();
			while (first != last) {
				if (((PathFinder::Node)*first).x == val.x && ((PathFinder::Node)*first).y == val.y)
					return true;
				++first;
			}

			return false;
		}
	};
}