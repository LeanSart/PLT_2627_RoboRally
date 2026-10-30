#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace UMLDiagram {
    
    void GameEngine::run() {
        bool running = true;
        try {
            sf::RenderWindow window;
            Plateau plateau = Plateau("assets/boards/CellTest.json");

            window.create(sf::VideoMode(1680, 1050), "PLT : RoboRally", sf::Style::Titlebar | sf::Style::Close);
            window.setFramerateLimit(60);

            while (running && window.isOpen()){
                sf::Event event{};

                while (window.pollEvent(event)){
                    if (event.type == sf::Event::Closed){
                        window.close();
                        running = false;
                        break;
                    }
                }

            window.clear(sf::Color::Black);
            plateau.affichePlateau(window);
            window.display();

            }
        } catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << "\n";
            running = false;
        }
    }
}