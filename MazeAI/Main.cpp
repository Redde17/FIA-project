#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameHandler.h"

#define MAP_X 21
#define MAP_Y 21
#define GRID_SIZE 30

int main() {
	sf::RenderWindow window(sf::VideoMode(MAP_X * GRID_SIZE, MAP_Y * GRID_SIZE), "MazeAI", sf::Style::Close);
	GameHandler GH(MAP_X, MAP_Y, GRID_SIZE);

	GH.generateNewMaze();

	std::cout << "Program start" << std::endl;
	while (window.isOpen())
	{
		//draw map
		window.clear();
		GH.drawMaze(&window);
		window.display();
	}
	return 0;
}