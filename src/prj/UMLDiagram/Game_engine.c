#include "Game_engine.h"


void GameEngine::run(){}

void GameEngine::changeState(std::unique_ptr<AbstractState> newState){
    currentState = std::move(newState);
}

void GameEngine::quit(){}