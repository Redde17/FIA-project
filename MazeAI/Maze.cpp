#include "Maze.h"

Maze::Position::Position() {
	x = 0;
	y = 0;
}

Maze::Position::Position(const int& x, const int& y) {
	this->x = x;
	this->y = y;
}

Maze::Maze(){}

Maze::Maze(const int& sizeX, const int& sizeY) {
	gameMap.resize(sizeX);
	for (int x = 0; x < sizeX; x++) {
		gameMap[x].resize(sizeY);
		for (int y = 0; y < sizeY; y++) {
			gameMap[x][y].pos.x = x;
			gameMap[x][y].pos.y = y;
		}
	}

	//define game walls
	for (int x = 0; x < sizeX; x++) {
		gameMap[x][0].isGameWall = true;
		gameMap[x][0].isExplored = true;
	}

	for (int x = 0; x < sizeX; x++) {
		gameMap[x][sizeY - 1].isGameWall = true;
		gameMap[x][sizeY - 1].isExplored = true;
	}

	for (int y = 0; y < sizeY; y++) {
		gameMap[0][y].isGameWall = true;
		gameMap[0][y].isExplored = true;
	}

	for (int y = 0; y < sizeY; y++) {
		gameMap[sizeX - 1][y].isGameWall = true;
		gameMap[sizeX - 1][y].isExplored = true;
	}

}