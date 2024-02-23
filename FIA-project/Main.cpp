#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameHandler.h"

#define MAP_X 31
#define MAP_Y 31
#define GRID_SIZE 30

int map[MAP_X][MAP_Y];

int main() {
    sf::RenderWindow window(sf::VideoMode(MAP_X * GRID_SIZE, MAP_Y * GRID_SIZE), "SnAIke"); 
    
    //Tile definitions
    GameHandler GH(MAP_X, MAP_Y, GRID_SIZE, true);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)){
            switch (event.type){
                case sf::Event::Closed:
                    window.close();
                    break;

                case sf::Event::KeyPressed:
                    switch (event.key.scancode){
                        case sf::Keyboard::Scan::Up:
                            GH.moveSnakeUp();
                            break;

                        case sf::Keyboard::Scan::Down:
                            GH.moveSnakeDown();
                            break;

                        case sf::Keyboard::Scan::Left:
                            GH.moveSnakeLeft();
                            break;

                        case sf::Keyboard::Scan::Right:
                            GH.moveSnakeRight();
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }
        }

        window.clear();
        GH.drawMap(&window);
        window.display();
    }

	return 0;
}