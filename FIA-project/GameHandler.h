#pragma once
#include <SFML/Graphics.hpp>
#include <list>


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
		std::list<Position> *snakePart = new std::list<Position>();
	} snake;

	int** gameMap;
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
	void moveSnakeUp();
	void moveSnakeDown();
	void moveSnakeLeft();
	void moveSnakeRight();
	void drawMap(sf::RenderWindow *window);
	void spawnApple();

private:
	void moveSnake(int x, int y);
};

