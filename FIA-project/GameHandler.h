#pragma once
#include <SFML/Graphics.hpp>


class GameHandler
{
private:
	enum Tile : int {
		Empty = 0,
		Apple = 1,
		SnakeHead = 2,
		SnakeBody = 3,
		SnakeTail = 4
	};

	struct Position {
		int x;
		int y;
	};

	struct Apple {
		bool isSpawned = false;
		Position position;
	} apple;

	struct Snake {
		Position headPosition;
		Position tailPosition;
		int lenght = 0;
	} snake;

	int** gameMap;
	int mapSizeX;
	int mapSizeY;
	float gridSize;
	bool gridMode;

	sf::RectangleShape *snakeTile;
	sf::RectangleShape *appleTile;
	sf::RectangleShape *errorTile;
	sf::RectangleShape *testingTile;


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

