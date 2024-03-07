#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <queue>
#include <list>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

//change this to define the weighted h() for faster A* search, must be > 1
//note, by speeding up the search the algorithm will sacrifice some optimality in the solutione path
#define WEIGHTED_CONSTANT 5.0f
	
//POSSIBLE UPDATES
//include map access in the heuristic for the snake
//make the snake aware of it's moving body during path finding
//make path visualizer for the algorithm

namespace AI_Module {
	class PathFinder{
		enum VisualizerTiles {
			Wall = -1,
			Empty = 0,
			Apple = 1,
			SnakeBody = 2,
			openSet = 3,
			closedSet = 4
		};

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
			float g = std::numeric_limits<int>::max();
			float h = 0;
			float f = 0;
			Node* parent = NULL;

			Node();
			Node(int x, int y);
			Node(Node* parent, int x, int y);
			Node(Node* baseNode);
			bool operator==(const Node& a) const;
		};

		struct NodeComparator {
			bool operator()(const Node& n1, const Node& n2) const;
		};

		struct InvertedNodeComparator {
			bool operator()(const Node& n1, const Node& n2) const;
		};

	private:
		std::vector<std::vector<int>> pathFinderVision;
		sf::RenderWindow* window;

		void debug_PrintMessage(std::string msg);

		float calculateDistance(Node start, Node target);
		void reconstructPath(Node* startNode);

		bool algorithmAstar(std::vector<std::vector<int>> mapInstance, Node start, Node target, PathType pathType);
		
		bool algorithHamiltonianCycle(std::vector<std::vector<int>> mapInstance, Node start);
		bool findHamiltionianCycle(std::vector<Node> path, int pos);
		bool isValidStep();

		void checkPathMapAccess();

	public:
		std::stack<Action>* actionBuffer;
		Action lastActionPerformed;

		bool findPath(std::vector<std::vector<int>> mapInstance, int xStart, int yStart, int xTarget, int yTarget, PathType pathType);
		void drawVisualizerMap(sf::RenderWindow* window);

		PathFinder(sf::RenderWindow *window);
		~PathFinder();
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