#include "ScreenManager.h"
#include "TitleScreen.h"
#include "ConfigScreen.h"
#include "LoadScreen.h"
#include "GameScreen.h"

namespace UMLDiagram {

    ScreenManager::ScreenManager(AppContext& context) : ctx(context) {
        ctx.goTo(ScreenId::TITRE);
        applyTransition();
    }
    
    void ScreenManager::handleEvent(const sf::Event& event) {
        if (current) current->handleEvent(event);
    }
    
    void ScreenManager::update(float dt) {
        if (current) current->update(dt);
    }
    
    void ScreenManager::draw(sf::RenderWindow& window) {
        if (!current) return;
        window.clear();
        current->drawBackground(window);
        current->draw(window);
    }

    void ScreenManager::applyTransition() {
        if (ctx.requested == ScreenId::AUCUN) return;
        
        const ScreenId id = ctx.requested;
        ctx.requested = ScreenId::AUCUN;
        
        if (id == ScreenId::QUITTER) {
            finished = true;
            return;
        }
        current = make(id);
        if (current) current->onEnter();
    }

    bool ScreenManager::isFinished() const { return finished; }

    std::unique_ptr<Screen> ScreenManager::make(ScreenId id) {
        switch (id) {
            case ScreenId::TITRE:         return std::make_unique<TitleScreen>(ctx);
            case ScreenId::CONFIGURATION: return std::make_unique<ConfigScreen>(ctx);
            case ScreenId::CHARGEMENT:    return std::make_unique<LoadScreen>(ctx);
            case ScreenId::PARTIE:        return std::make_unique<GameScreen>(ctx);
            default:                      return nullptr;
        }
    }
}