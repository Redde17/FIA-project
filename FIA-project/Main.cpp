#include <iostream>
#include <SFML/Graphics.hpp>
#include "GameHandler.h"
#include "PathFinder.h"

#include <chrono>
#include <thread>
#include <fstream>

using namespace std::chrono_literals;
using std::chrono::duration;
using std::chrono::high_resolution_clock;

#define FULL 1
#define SIMPLE 0

#define DATA_COLLECTION_RUN false
#define DATA_COLLECTION_RUN_MAX_AMOUNT 10
#define DATA_COLLECTION_RUN_WAIT 0ms

//on FULL each run will be saved and some avarage data of the whole execution
//on SIMPLE only the avarage data of the whole execution will be saved
#define DATA_COLLECTION_RUN_VERBOSITY SIMPLE

#if DATA_COLLECTION_RUN
    #define GRID_SIZE 0
#else
    #define GRID_SIZE 30
#endif

#define MAP_X 21
#define MAP_Y 21

#define SNAKE_GRID_MODE true

void generateAIAction(GameHandler* GH, AI_Module::PathFinder* PF, bool* snakeIsNotStuck);
void handleEvents(sf::RenderWindow* window, GameHandler* GH, bool* snakeIsNotStuck);
void printDataOnCoutStream(int snakeLenght);
void saveRunDataOnFile(int snakeLenght);
void saveExecutionDataOnFile();
void cleanDataVariables();
void tryUnstuckSnake(AI_Module::PathFinder* PF, GameHandler* GH);

//data collection variables for run
int runNumber = 1;
int generatedPaths = 0;
int generatedSteps = 0;
duration<double, std::milli> generationTime;

//data collection variables for execution
int totalSnakeLenght = 0;
int totalGeneratedPaths = 0;
int totalGeneratedSteps = 0;
duration<double, std::milli> totalGenerationTime;

int main() {
    //window variables
    sf::RenderWindow window(sf::VideoMode(MAP_X * GRID_SIZE, MAP_Y * GRID_SIZE), "SnAIke", sf::Style::Close);

    //game variables
    bool snakeIsNotStuck = true;
    GameHandler GH(MAP_X, MAP_Y, GRID_SIZE, true);
    AI_Module::PathFinder PF;

    while (window.isOpen())
    {
        if (!snakeIsNotStuck && !GH.checkIfSnakeCanMove()) {
            //game ended
            std::cout << "snake cannot move" << std::endl;
#if DATA_COLLECTION_RUN
            //dump game data into a file for data collection
            printDataOnCoutStream(GH.getSnakeLenght());

#if DATA_COLLECTION_RUN_VERBOSITY == FULL
            saveRunDataOnFile(GH.getSnakeLenght());
#endif
            totalSnakeLenght += GH.getSnakeLenght();
            if (runNumber == DATA_COLLECTION_RUN_MAX_AMOUNT) {
                saveExecutionDataOnFile();
                window.close();
                break;
            }

            //wait a second
            std::this_thread::sleep_for(DATA_COLLECTION_RUN_WAIT);

            //restart
            cleanDataVariables();
            snakeIsNotStuck = true;
            GH = *new GameHandler(MAP_X, MAP_Y, GRID_SIZE, SNAKE_GRID_MODE);
            runNumber++;
#endif
        }
        else if (!snakeIsNotStuck && GH.checkIfSnakeCanMove()) {
            //try to unstuck snake for AI
            std::cout << "snake can still move" << std::endl;
            tryUnstuckSnake(&PF, &GH);
            snakeIsNotStuck = true;
        }

        //AI integration
        generateAIAction(&GH, &PF, &snakeIsNotStuck);

        //if not empty do nothing

        //handle events and action buffer
        handleEvents(&window, &GH, &snakeIsNotStuck);

#if !DATA_COLLECTION_RUN
        //draw map
        window.clear();
        GH.drawMap(&window);
        window.display();
#endif
    }

    //delete &GH;
    std::cout << "Program ended" << std::endl;
	return 0;
}

void generateAIAction(GameHandler* GH, AI_Module::PathFinder* PF, bool* snakeIsNotStuck) {
    //check if action buffer is empty or not
    if (!PF->actionBuffer->empty()) {
        switch (PF->actionBuffer->top()) {
        case AI_Module::PathFinder::Action::UP:
            GH->moveSnakeUp();
            break;
        case AI_Module::PathFinder::Action::DOWN:
            GH->moveSnakeDown();
            break;
        case AI_Module::PathFinder::Action::LEFT:
            GH->moveSnakeLeft();
            break;
        case AI_Module::PathFinder::Action::RIGHT:
            GH->moveSnakeRight();
            break;
        default:
            std::cout << "Error while parsing buffered move" << std::endl;
            break;
        }
        PF->lastActionPerformed = PF->actionBuffer->top();
        PF->actionBuffer->pop();
    }
    else {
        if (*snakeIsNotStuck) {
            auto t1 = high_resolution_clock::now();
            //checks the snake lenght, if the lenght is greater then a certain number
            //switch path finding type to try to find a longer path rather then a short one
            //the idea behind this choice is to try and increase snake lenght as much as possible
            //because if the snake takes a longer route, it shoul coil around itself less, and thus get stuck less
            //snake lenght check number is based on some test runs
            //probably could be a much better number
            if (GH->getSnakeLenght() < MAP_X) {
                *snakeIsNotStuck = PF->findPath(
                    GH->getMap(),
                    GH->getSnakeHeadPositionX(),
                    GH->getSnakeHeadPositionY(),
                    GH->getApplePositionX(),
                    GH->getApplePositionY(),
                    AI_Module::PathFinder::Short
                );
            }
            else {
                *snakeIsNotStuck = PF->findPath(
                    GH->getMap(),
                    GH->getSnakeHeadPositionX(),
                    GH->getSnakeHeadPositionY(),
                    GH->getApplePositionX(),
                    GH->getApplePositionY(),
                    AI_Module::PathFinder::Long
                );
            }
            auto t2 = high_resolution_clock::now();
            duration<double, std::milli> ms_double = t2 - t1;

            std::cout << ms_double.count() << "ms\n";

            //data collection
            generatedPaths++;
            generatedSteps += PF->actionBuffer->size();
            generationTime += ms_double;

            totalGeneratedPaths++;
            totalGeneratedSteps += PF->actionBuffer->size();
            totalGenerationTime += ms_double;
        }
        else {
            //std::cout << "Snake is stuck" << std::endl;
        }
    }
}

void handleEvents(sf::RenderWindow* window, GameHandler* GH, bool* snakeIsNotStuck) {
    sf::Event event;
    while (window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window->close();
            break;

        case sf::Event::KeyPressed:
            switch (event.key.scancode) {
            case sf::Keyboard::Scan::Up:
                GH->moveSnakeUp();
                break;

            case sf::Keyboard::Scan::Down:
                GH->moveSnakeDown();
                break;

            case sf::Keyboard::Scan::Left:
                GH->moveSnakeLeft();
                break;

            case sf::Keyboard::Scan::Right:
                GH->moveSnakeRight();
                break;
            case sf::Keyboard::Scan::Space:
                *snakeIsNotStuck = true;
                break;
            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

void printDataOnCoutStream(int snakeLenght) {
    std::cout << std::endl;
    std::cout << "==GAME ENDED SNAKE IS STUCK==" << std::endl;
    std::cout << "Run number for execution:              " << runNumber << std::endl;
    std::cout << "Grid size:                             " << "(" << MAP_X << " * " << MAP_Y << ")" << std::endl;
    std::cout << "Snake lenght:                          " << snakeLenght << std::endl;
    std::cout << "Generated paths:                       " << generatedPaths << std::endl;
    std::cout << "Total amount of steps:                 " << generatedSteps << std::endl;
    std::cout << "Avarage steps generated per path:      " << generatedSteps / generatedPaths << std::endl;
    std::cout << "Time passed generatign paths:          " << generationTime.count() << std::endl;
    std::cout << "Avarage generation time:               " << generationTime.count() / generatedPaths << std::endl;
    std::cout << std::endl;
}

void saveRunDataOnFile(int snakeLenght) {
    std::ofstream dataFile("snAIkeDataStorage.txt", std::ios_base::app);
    if (dataFile.is_open()) {
        dataFile << "==GAME ENDED SNAKE IS STUCK==" << std::endl;
        dataFile << "Run number for execution:              " << runNumber << std::endl;
        dataFile << "Grid size:                             " << "(" << MAP_X << " * " << MAP_Y << ")" << std::endl;
        dataFile << "Snake lenght:                          " << snakeLenght << std::endl;
        dataFile << "Generated paths:                       " << generatedPaths << std::endl;
        dataFile << "Total amount of steps:                 " << generatedSteps << std::endl;
        dataFile << "Avarage steps generated per path:      " << generatedSteps / generatedPaths << std::endl;
        dataFile << "Time passed generatign paths:          " << generationTime.count() << std::endl;
        dataFile << "Avarage generation time:               " << generationTime.count() / generatedPaths << std::endl;
        dataFile << std::endl;

        dataFile.close();
    }
    else {
        std::cout << "DATA COLLECTION ERROR: Could not open data storage file" << std::endl;
    }
}

void saveExecutionDataOnFile() {
    std::ofstream dataFile("snAIkeDataStorage.txt", std::ios_base::app);
    if (dataFile.is_open()) {
        dataFile << "==Data recap for " << DATA_COLLECTION_RUN_MAX_AMOUNT << " runs== " << std::endl;
        dataFile << "Grid size:                             " << "(" << MAP_X << " * " << MAP_Y << ")" << std::endl;
        dataFile << "Avarage snake lenght:                  " << totalSnakeLenght / DATA_COLLECTION_RUN_MAX_AMOUNT << std::endl;
        dataFile << "Avarage steps generated per path:      " << totalGeneratedSteps / totalGeneratedPaths << std::endl;
        dataFile << "Avarage time passed generatign paths:  " << totalGenerationTime.count() / totalGeneratedPaths << std::endl;
        dataFile << std::endl;

        dataFile.close();
    }
    else {
        std::cout << "DATA COLLECTION ERROR: Could not open data storage file" << std::endl;
    }
}

void cleanDataVariables() {
    generatedPaths = 0;
    generatedSteps = 0;
    generationTime = generationTime.zero();
}

void tryUnstuckSnake(AI_Module::PathFinder* PF, GameHandler* GH) {
    switch (PF->lastActionPerformed)
    {
    case AI_Module::PathFinder::Action::UP:
        if (GH->moveSnakeUp())
            break;
        if (GH->moveSnakeRight()) {
            GH->moveSnakeDown();
            break;
        }

        if (GH->moveSnakeLeft()) {
            GH->moveSnakeDown();
            break;
        }

    case AI_Module::PathFinder::Action::DOWN:
        if (GH->moveSnakeDown())
            break;
        if (GH->moveSnakeLeft()) {
            GH->moveSnakeUp();
            break;
        }

        if (GH->moveSnakeRight()) {
            GH->moveSnakeUp();
            break;
        }

    case AI_Module::PathFinder::Action::LEFT:
        if (GH->moveSnakeLeft())
            break;
        if (GH->moveSnakeDown()) {
            GH->moveSnakeRight();
            break;
        }

        if (GH->moveSnakeUp()) {
            GH->moveSnakeRight();
            break;
        }

    case AI_Module::PathFinder::Action::RIGHT:
        if (GH->moveSnakeRight())
            break;
        if (GH->moveSnakeUp()) {
            GH->moveSnakeLeft();
            break;
        }

        if (GH->moveSnakeDown()) {
            GH->moveSnakeLeft();
            break;
        }

    default:
        std::cout << "Error while parsing buffered move" << std::endl;
        break;
    }
}