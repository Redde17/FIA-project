#include <SFML/Graphics.hpp>

#include "GameHandler.h"

#include <chrono>
#include <iostream>

using namespace std::chrono_literals;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

#define MAP_X 200
#define MAP_Y 100
#define GRID_SIZE 10

//int runs = 0;
duration<double, std::milli> mazeGenerationTime = mazeGenerationTime.zero();
duration<double, std::milli> mazePathFindingTime = mazePathFindingTime.zero();

void generateMaze(GameHandler& GH);
void solveMaze(GameHandler& GH);

int main() {
    srand((unsigned)time(NULL));
	GameHandler GH(MAP_X, MAP_Y, GRID_SIZE);

    //std::cout << "Avg time for [" << runs << "] runs" << std::endl;
    //std::cout << "Avg generation time: " << mazeGenerationTime.count() / runs << std::endl;
    //std::cout << "Avg pathFinding time: " << mazePathFindingTime.count() / runs << std::endl;

    std::cout << "Program start" << std::endl;
    generateMaze(GH);
    GH.drawMaze();

	while (GH.window->isOpen())
	{
        sf::Event event;
        while (GH.window->pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                GH.window->close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.scancode)
                {
                case sf::Keyboard::Scan::Space:
                    generateMaze(GH);
                    GH.drawMaze();

                    solveMaze(GH);
                    GH.drawMaze();
                    //runs++;
                    break;
                case sf::Keyboard::Scan::G:
                    generateMaze(GH);
                    GH.drawMaze();
                    break;
                case sf::Keyboard::Scan::S:
                    solveMaze(GH);
                    GH.drawMaze();
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }
    }
	return 0;
}

void generateMaze(GameHandler& GH) {
    auto t1 = high_resolution_clock::now();
    GH.generateNewMaze();
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    mazeGenerationTime += ms_double;
    std::cout << "Maze generation time: " << ms_double.count() << "ms\n";
}

void solveMaze(GameHandler& GH) {
    auto t1 = high_resolution_clock::now();
    GH.solveMaze();
    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    mazePathFindingTime += ms_double;
    std::cout << "Path generation time: " << ms_double.count() << "ms\n\n";
    
}