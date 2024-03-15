#pragma once
#include <SFML/Graphics.hpp>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;

class GameHandler
{
private:
	enum Tile : int {
		Wall = -1,
		Empty = 0,
		Apple = 1,
		SnakeBody = 2,
	};

	struct Position {
		int x;
		int y;

		Position() {
			this->x = 0;
			this->y = 0;
		}

		Position(int x, int y) {
			this->x = x;
			this->y = y; 
		}
	};

	struct Apple {
		bool isSpawned;
		Position position;
	} apple;

	struct Snake {
		std::list<Position> snakePart;
	} snake;

	//int** gameMap;

	std::vector < std::vector<int> > gameMap;

	int mapSizeX;
	int mapSizeY;
	float gridSize;
	bool gridMode;

	sf::RectangleShape *snakeTile;
	sf::RectangleShape *appleTile;
	sf::RectangleShape* wallTile;
	sf::RectangleShape *errorTile;
	sf::RectangleShape *emptyTile;


public:
	GameHandler(int mapSizeX, int mapSizeY, float gridSize, bool gridMode);
	~GameHandler();
	
	std::vector<std::vector<int>> getMap();
	int getSnakeHeadPositionX();
	int getSnakeHeadPositionY();
	int getApplePositionX();
	int getApplePositionY();
	int getSnakeLenght();
	
	bool moveSnakeUp();
	bool moveSnakeDown();
	bool moveSnakeLeft();
	bool moveSnakeRight();
	void drawMap(sf::RenderWindow *window);
	void spawnApple();

	bool checkIfSnakeCanMove();
private:
	bool moveSnake(int x, int y);
};
