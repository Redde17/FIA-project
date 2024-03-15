#include "MazeGenerator.h"
#include <iostream>


//iterative implementation of the Randomized DepthFirst algorithm
Maze MazeGenerator::generateRandomDepthFirstMaze(const int& sizeX, const int& sizeY, GameHandler& GH) {
	Maze newMaze(sizeX, sizeY);
	std::stack<Maze::Node*> unexploredNodes;
	Maze::Node* currentNode;
	Maze::Node* neighborNode;

	//starting node is position (1, 1)
	newMaze.gameMap[1][1].isExplored = true;
	unexploredNodes.push(&newMaze.gameMap[1][1]);

	//while stack is not empty 
	while (!unexploredNodes.empty())
	{
		//get current node
		currentNode = unexploredNodes.top();
		unexploredNodes.pop();

		//get a random neighbor of the current node
		neighborNode = getRandomNeighbor(&newMaze, currentNode);

		//if no available neighbor then skip the loop
		if (neighborNode == NULL)
			continue;

		//Push the current node to the stack
		unexploredNodes.push(currentNode);
			
		//Remove the wall between the current node and the neighbor
		deleteWall(currentNode, neighborNode);

		//Mark the neighor as visited and push it to the stack
		neighborNode->isExplored = true;
		unexploredNodes.push(neighborNode);

		//GH.drawMaze();
	}
	return newMaze;
}

//gets neighbor if is possible
Maze::Node* MazeGenerator::getRandomNeighbor(Maze* newMaze, Maze::Node* currentNode) {
	std::vector<Maze::Node*> neighborNodes;

	//LEFT
	if (!newMaze->gameMap[currentNode->pos.x - 1][currentNode->pos.y].isExplored)
		neighborNodes.push_back(&newMaze->gameMap[currentNode->pos.x - 1][currentNode->pos.y]);
	//UP
	if (!newMaze->gameMap[currentNode->pos.x][currentNode->pos.y - 1].isExplored)
		neighborNodes.push_back(&newMaze->gameMap[currentNode->pos.x][currentNode->pos.y - 1]);
	//RIGHT
	if (!newMaze->gameMap[currentNode->pos.x + 1][currentNode->pos.y].isExplored)
		neighborNodes.push_back(&newMaze->gameMap[currentNode->pos.x + 1][currentNode->pos.y]);
	//DOWN
	if (!newMaze->gameMap[currentNode->pos.x][currentNode->pos.y + 1].isExplored)
		neighborNodes.push_back(&newMaze->gameMap[currentNode->pos.x][currentNode->pos.y + 1]);

	if (neighborNodes.size() == 0)
		return NULL;

	return neighborNodes[rand() % neighborNodes.size()];
}

//deletes walls between nodes
void MazeGenerator::deleteWall(Maze::Node* fromNode, Maze::Node* toNode) {
	//extrapolate direction from pos
	int x = toNode->pos.x - fromNode->pos.x;
	int y = toNode->pos.y - fromNode->pos.y;


	//delete walls based on direction
	//LEFT
	if (x == -1) {
		fromNode->walls[Maze::Directions::LEFT] = false;
		toNode->walls[Maze::Directions::RIGHT] = false;
	}
	//UP
	else if (y == -1) {
		fromNode->walls[Maze::Directions::UP] = false;
		toNode->walls[Maze::Directions::DOWN] = false;
	}
	//RIGHT
	else if (x == 1) {
		fromNode->walls[Maze::Directions::RIGHT] = false;
		toNode->walls[Maze::Directions::LEFT] = false;
	}
	//DOWN
	else if (y == 1) {
		fromNode->walls[Maze::Directions::DOWN] = false;
		toNode->walls[Maze::Directions::UP] = false;
	}
	//unexpected (x, y) value
	else 
		throw "Maze generation unable to delete wall";
}

Maze MazeGenerator::generateMaze(Generators generator, const int& sizeX, const int& sizeY, GameHandler& GH) {	
	switch (generator){
		case MazeGenerator::RandomDepthFirst:
			return generateRandomDepthFirstMaze(sizeX, sizeY, GH);
		default: {
			throw "generator type not defined";
			return *new Maze(0, 0);
		}
	}
}