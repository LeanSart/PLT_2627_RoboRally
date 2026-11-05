#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

namespace UMLDiagram {
    GameEngine::GameEngine () = default;
    void GameEngine::run() {
        Pile_2_cartes pioche;
        Programme cartes_programme(0, TypeMouvement::AVANCER_1_CASE); //Valeurs ignorées seules celles du fichier JSON seront prises en compte
        cartes_programme.loadCartes("./assets/cards/cards_program.json",pioche);
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