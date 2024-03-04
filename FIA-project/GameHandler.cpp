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

    //wall tile definition
    wallTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    wallTile->setFillColor(sf::Color(255, 255, 255));

    //error tile definition
    errorTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    errorTile->setFillColor(sf::Color(0, 0, 255));

    //grid tile definition
    emptyTile = new sf::RectangleShape(sf::Vector2f(this->gridSize, this->gridSize));
    if (gridMode) {
        emptyTile->setOutlineThickness(-0.5f);
        emptyTile->setOutlineColor(sf::Color(145, 145, 145));
    }
    emptyTile->setFillColor(sf::Color(0, 0, 0, 255));

    //new map creation where map is std::vector<std::vector<int>>
    gameMap.resize(mapSizeX, std::vector<int>(mapSizeY, Tile::Empty));


    //old map creation where map is int**
    ////map creation
    //gameMap = new int*[mapSizeX];
    //for (int x = 0; x < mapSizeX; x++)
    //    gameMap[x] = new int[mapSizeY];
    //

    ////center of the map
    //for (int x = 1; x < mapSizeX - 1; x++)
    //    for (int y = 1; y < mapSizeY - 1; y++) 
    //            gameMap[x][y] = Tile::Empty;

    //top wall
    for (int x = 0; x < mapSizeX; x++)
        gameMap[x][0] = Tile::Wall;

    //bottom wall
    for (int x = 0; x < mapSizeX; x++)
        gameMap[x][mapSizeY - 1] = Tile::Wall;

    //left wall
    for (int y = 1; y < mapSizeY - 1; y++)
        gameMap[0][y] = Tile::Wall;

    //right wall
    for (int y = 1; y < mapSizeY - 1; y++)
        gameMap[mapSizeX - 1][y] = Tile::Wall;

    //spawn snake in the center of the map
    snake.snakePart.push_front(*new Position(mapSizeX / 2, mapSizeY / 2));
    gameMap[snake.snakePart.front().x][snake.snakePart.front().y] = Tile::SnakeBody;

    //spawn first apple in random position
    spawnApple();
}

GameHandler::~GameHandler() {
    delete snakeTile;
    delete appleTile;
    delete wallTile;
    delete errorTile;
    delete emptyTile;

    //for (int x = 0; x < mapSizeX; x++)
    //    delete[] gameMap[x];
    //delete[] gameMap;
}

std::vector<std::vector<int>> GameHandler::getMap() {
    return gameMap;
}

int GameHandler::getSnakeHeadPositionX() {
    return snake.snakePart.front().x;
}

int GameHandler::getSnakeHeadPositionY() {
    return snake.snakePart.front().y;
}

int GameHandler::getApplePositionX() {
    return apple.position.x;
}

int GameHandler::getApplePositionY() {
    return apple.position.y;
}

int GameHandler::getSnakeLenght() {
    return snake.snakePart.size();
}

bool GameHandler::moveSnakeUp() {
    return moveSnake(snake.snakePart.front().x, snake.snakePart.front().y - 1);
}

bool GameHandler::moveSnakeDown() {
    return moveSnake(snake.snakePart.front().x, snake.snakePart.front().y + 1);
}

bool GameHandler::moveSnakeLeft() {
    return moveSnake(snake.snakePart.front().x - 1, snake.snakePart.front().y);
}

bool GameHandler::moveSnakeRight() {
    return moveSnake(snake.snakePart.front().x + 1, snake.snakePart.front().y);
}

void GameHandler::drawMap(sf::RenderWindow *window) {
    sf::RectangleShape* toBeDrawnTile = NULL;
    //can be optimized
    //draw grid
    for (int x = 0; x < mapSizeX; x++) {
        for (int y = 0; y < mapSizeY; y++) {
            switch (gameMap[x][y]){
            case Tile::Empty:
                toBeDrawnTile = emptyTile;
                break;

            case Tile::Apple:
                toBeDrawnTile = appleTile;
                break;

            case Tile::Wall: 
                toBeDrawnTile = wallTile;
                break;

            case Tile::SnakeBody:
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

    apple.isSpawned = false;
    while (!apple.isSpawned){
        apple.position.x = rand() % mapSizeX;
        apple.position.y = rand() % mapSizeY;

        if (gameMap[apple.position.x][apple.position.y] == Tile::Empty) {
            gameMap[apple.position.x][apple.position.y] = Tile::Apple;
            apple.isSpawned = true;
        }
    }   
}

bool GameHandler::checkIfSnakeCanMove() {
    //get snake head surroundings 
    int surroingTiles[4] = {
        gameMap[snake.snakePart.front().x][snake.snakePart.front().y - 1], //up
        gameMap[snake.snakePart.front().x][snake.snakePart.front().y + 1], //down
        gameMap[snake.snakePart.front().x - 1][snake.snakePart.front().y], //left
        gameMap[snake.snakePart.front().x + 1][snake.snakePart.front().y]  //right
    };
    
    if (surroingTiles[0] == Tile::Empty || surroingTiles[0] == Tile::Apple ||
        surroingTiles[1] == Tile::Empty || surroingTiles[1] == Tile::Apple ||
        surroingTiles[2] == Tile::Empty || surroingTiles[2] == Tile::Apple ||
        surroingTiles[3] == Tile::Empty || surroingTiles[3] == Tile::Apple)
        return true;

    return false;
}

bool GameHandler::moveSnake(int x, int y) {
    //check if valid move
    if (gameMap[x][y] != Tile::Empty && gameMap[x][y] != Tile::Apple) //invalid move, next tile is not empty(0)
        return false;

    if (gameMap[x][y] == Tile::Apple) {
        //move head
        gameMap[x][y] = Tile::SnakeBody;

        snake.snakePart.push_front(*new Position(x, y));

        spawnApple();
    }
    else {
        //move head
        gameMap[snake.snakePart.back().x][snake.snakePart.back().y] = Tile::Empty;
        gameMap[x][y] = Tile::SnakeBody;

        snake.snakePart.pop_back();
        snake.snakePart.push_front(*new Position(x, y));
    }

    return true;
}