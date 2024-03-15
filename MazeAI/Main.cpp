#include <SFML/Graphics.hpp>

#include "GameHandler.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

#define MAP_X 150
#define MAP_Y 72
#define GRID_SIZE 15

int main() {
    srand((unsigned)time(NULL));
	GameHandler GH(MAP_X, MAP_Y, GRID_SIZE);

    int runs = 0;
    duration<double, std::milli> mazeGenerationTime = mazeGenerationTime.zero();
    duration<double, std::milli> mazePathFindingTime = mazePathFindingTime.zero();

    auto t1 = high_resolution_clock::now();
    GH.generateNewMaze();
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    mazeGenerationTime += ms_double;

    std::cout << "Maze generation time: " << ms_double.count() << "ms\n";
    GH.drawMaze();

    t1 = high_resolution_clock::now();
    GH.solveMaze();
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;
    mazePathFindingTime += ms_double;

    std::cout << "Path generation time: " << ms_double.count() << "ms\n";

    t1 = high_resolution_clock::now();
    GH.drawMaze();
    t2 = high_resolution_clock::now();
    ms_double = t2 - t1;

    std::cout << "Draw time: " << ms_double.count() << "ms\n";
    runs++;


    std::cout << "Avg time for [" << runs << "] runs" << std::endl;
    std::cout << "Avg generation time: " << mazeGenerationTime.count() / runs << std::endl;
    std::cout << "Avg pathFingind time: " << mazePathFindingTime.count() / runs << std::endl;

    std::cout << "\n=============\n";
    if (runs == 500)
        GH.window->close();

	std::cout << "Program start" << std::endl;

	while (GH.window->isOpen())
	{
        sf::Event event;
        while (GH.window->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                GH.window->close();
                break;
            default:
                break;
            }
        }
    }
	return 0;
}