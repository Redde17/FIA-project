#include <time.h>
#include <stdlib.h>
#include "GameHandler.h"
#include <SFML/Graphics.hpp>
#include <iostream>


GameHandler::GameHandler(int mapSizeX, int mapSizeY, float gridSize, bool gridMode) {
    this->mapSizeX = mapSizeX;
    this->mapSizeY = mapSizeY;
    this->gridSize = gridSize;
    this->gridMode = gridMode;

    //snake tile definition
    snakeTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    snakeTile->setFillColor(sf::Color(0, 255, 0));

    //apple tile definition
    appleTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    appleTile->setFillColor(sf::Color(255, 0, 0));

    //error tile definition
    errorTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    errorTile->setFillColor(sf::Color(0, 0, 255));

    //testing tile definition
    testingTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    testingTile->setFillColor(sf::Color(0, 0, 0, 255));
    testingTile->setOutlineThickness(-0.5f);
    testingTile->setOutlineColor(sf::Color(145, 145, 145));

    //map creation
    gameMap = new int*[mapSizeX];
    for (int x = 0; x < mapSizeX; x++)
        gameMap[x] = new int[mapSizeY];
    
    for (int x = 0; x < mapSizeX; x++)
        for (int y = 0; y < mapSizeY; y++)
            gameMap[x][y] = Tile::Empty;

    //spawn snake in the center of the map
    snake.headPosition.x = mapSizeX / 2;
    snake.headPosition.y = mapSizeY / 2;
    snake.tailPosition.x = snake.headPosition.x;
    snake.tailPosition.y = snake.headPosition.y;
    gameMap[snake.headPosition.x][snake.headPosition.y] = Tile::SnakeHead;

    //spawn first apple in random position
    spawnApple();
}

GameHandler::~GameHandler() {
    delete snakeTile;
    delete appleTile;
    delete testingTile;

    for (int x = 0; x < mapSizeX; x++)
        delete[] gameMap[x];
    delete[] gameMap;
}

void GameHandler::moveSnakeUp() {
    if (snake.headPosition.y == 0) //movement not possible
        return;

    moveSnake(snake.headPosition.x, snake.headPosition.y - 1);
}

void GameHandler::moveSnakeDown() {
    if (snake.headPosition.y == mapSizeY - 1) //movement not possible
        return;

    moveSnake(snake.headPosition.x, snake.headPosition.y + 1);
}

void GameHandler::moveSnakeLeft() {
    if (snake.headPosition.x == 0) //movement not possible
        return;

    moveSnake(snake.headPosition.x - 1, snake.headPosition.y);
}

void GameHandler::moveSnakeRight() {
    if (snake.headPosition.x == mapSizeX - 1) //movement not possible
        return;

    moveSnake(snake.headPosition.x + 1, snake.headPosition.y);
}

void GameHandler::drawMap(sf::RenderWindow *window) {
    sf::RectangleShape* toBeDrawnTile = NULL;

    //draw grid
    for (int x = 0; x < mapSizeX; x++) {
        for (int y = 0; y < mapSizeY; y++) {
            switch (gameMap[x][y]){
            case Tile::Empty: //empty tile
                if (gridMode)
                    toBeDrawnTile = testingTile;
                break;

            case Tile::Apple: //apple tile
                toBeDrawnTile = appleTile;
                break;

            case Tile::SnakeHead:
            case Tile::SnakeBody:
            case Tile::SnakeTail:
                toBeDrawnTile = snakeTile;
                break;

            default: //error?
                break;
            }

            toBeDrawnTile->setPosition(x * gridSize, y * gridSize);
            window->draw(*toBeDrawnTile);
        }
    }
}

void GameHandler::spawnApple() {
    srand(time(NULL));

    while (!apple.isSpawned){
        apple.position.x = rand() % mapSizeX;
        apple.position.y = rand() % mapSizeY;

        if (gameMap[apple.position.x][apple.position.y] == Tile::Empty) {
            gameMap[apple.position.x][apple.position.y] = Tile::Apple;
            apple.isSpawned = true;
        }
    }   
}

void GameHandler::moveSnake(int x, int y) {
    //check if valid move
    if (gameMap[x][y] != Tile::Empty && gameMap[x][y] != Tile::Apple) //invalid move, next tile is not empty(0)
        return;

    bool appleEaten = false;
    bool leftIsOutOfMap = false;
    bool upIsOutOfMap = false;
    bool rightIsOutOfMap = false;
    bool downIsOutOfMap = false;

    if (gameMap[x][y] == Tile::Apple) {
        snake.lenght++;
        appleEaten = true;
        apple.isSpawned = false;
    }

    //check if apple eaten
    if (appleEaten) {
        //add body
        if (snake.lenght > 1)
            gameMap[snake.headPosition.x][snake.headPosition.y] = Tile::SnakeBody;
        else
            gameMap[snake.headPosition.x][snake.headPosition.y] = Tile::SnakeTail;

        //move head
        snake.headPosition.x = x;
        snake.headPosition.y = y;
        gameMap[x][y] = Tile::SnakeHead;

        spawnApple();
    }
    else {
        //move head
        gameMap[snake.headPosition.x][snake.headPosition.y] = Tile::SnakeBody;
        snake.headPosition.x = x;
        snake.headPosition.y = y;
        gameMap[x][y] = Tile::SnakeHead;

        //move tail
        //clean old tail from map
        gameMap[snake.tailPosition.x][snake.tailPosition.y] = Tile::Empty;
        
        //check around for direction
        if (snake.tailPosition.x == 0)
            leftIsOutOfMap = true;
        else if (snake.tailPosition.x == mapSizeX - 1)
            rightIsOutOfMap = true;

        if (snake.tailPosition.y == 0)
            upIsOutOfMap = true;
        else if (snake.tailPosition.y == mapSizeY - 1)
            downIsOutOfMap = true;
         
        //left

        if (!leftIsOutOfMap && gameMap[snake.tailPosition.x - 1][snake.tailPosition.y] == Tile::SnakeBody) {
            snake.tailPosition.x = snake.tailPosition.x - 1;
            snake.tailPosition.y = snake.tailPosition.y;
            gameMap[snake.tailPosition.x][snake.tailPosition.y] = Tile::SnakeTail;
        }

        //top
        else if (!upIsOutOfMap && gameMap[snake.tailPosition.x][snake.tailPosition.y - 1] == Tile::SnakeBody) {
            snake.tailPosition.x = snake.tailPosition.x;
            snake.tailPosition.y = snake.tailPosition.y - 1;
            gameMap[snake.tailPosition.x][snake.tailPosition.y] = Tile::SnakeTail;
        }

        //right
        else if (!rightIsOutOfMap && gameMap[snake.tailPosition.x + 1][snake.tailPosition.y] == Tile::SnakeBody) {
            snake.tailPosition.x = snake.tailPosition.x + 1;
            snake.tailPosition.y = snake.tailPosition.y;
            gameMap[snake.tailPosition.x][snake.tailPosition.y] = Tile::SnakeTail;
        }

        //down
        else if (!downIsOutOfMap && gameMap[snake.tailPosition.x][snake.tailPosition.y + 1] == Tile::SnakeBody) {
            snake.tailPosition.x = snake.tailPosition.x;
            snake.tailPosition.y = snake.tailPosition.y + 1;
            gameMap[snake.tailPosition.x][snake.tailPosition.y] = Tile::SnakeTail;
        }
    }
}