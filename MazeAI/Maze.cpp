#include "Maze.h"

Maze::Position::Position() {
	x = 0;
	y = 0;
}

Maze::Position::Position(const int& x, const int& y) {
	this->x = x;
	this->y = y;
}

Maze::Maze(const int& sizeX, const int& sizeY){
	gameMap.resize(sizeX);
	for (int x = 0; x < sizeX; x++) {
		gameMap[x].resize(sizeY);
		for (int y = 0; y < sizeY; y++) {
			gameMap[x][y].pos.x = x;
			gameMap[x][y].pos.x = y;
		}
	}
}