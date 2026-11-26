#include "GameScreen.h"
#include "engines/GameEngine.h"
#include "engines/RenderEngine.h"
#include "ScreenManager.h"
#include <iostream>

namespace screens {
    GameScreen::GameScreen(engines::AppContext& context) : Screen(context) {}

    void GameScreen::onEnter() { 
        try {
            ctx.engine.startGame();
            ready = true;
        } catch (const std::exception& e) {
            std::cerr << "[Erreur] Impossible de demarrer la partie : " << e.what() << std::endl;
            ctx.goTo(ScreenId::CONFIGURATION);
        }
    }
 
    void GameScreen::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
        ctx.goTo(ScreenId::TITRE);
    }

    void GameScreen::update(float) {
        // Ici viendra la logique de tour (pioche, programmation, exécution des cartes...)
    }

    void GameScreen::draw(sf::RenderWindow&) {
        if (ready) ctx.render.update(ctx.engine.getPlateau(),
                                     ctx.engine.getPlayers().at(0).getRobot(),
                                     ctx.engine.getPlayers().at(0).getT2B()
        );
    }

    std::string GameScreen::backgroundPath() const {
    return "assets/background/Title.jpg";
    }
}