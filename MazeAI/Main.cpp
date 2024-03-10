#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameHandler.h"

#define MAP_X 100
#define MAP_Y 100
#define GRID_SIZE 10

int main() {
	sf::RenderWindow window(sf::VideoMode(MAP_X * GRID_SIZE, MAP_Y * GRID_SIZE), "MazeAI", sf::Style::Close);
	GameHandler GH(MAP_X, MAP_Y, GRID_SIZE);

	GH.generateNewMaze();

	std::cout << "Program start" << std::endl;
	while (window.isOpen())
	{
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

		window.clear();
		GH.drawMaze(&window);
		window.display();
	}
	return 0;
}