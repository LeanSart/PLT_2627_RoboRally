#include "Screen.h"

namespace screens {
    Screen::Screen (engines::AppContext& context) : ctx(context) {}
    Screen::~Screen() = default;
    void Screen::onEnter() {}
    void Screen::update(float dt) { 
        (void)dt; }
    std::string Screen::backgroundPath () const {
        return "asset/background/default.png"; // Image par défaut
    }

    void Screen::drawBackground (sf::RenderWindow& window) const {
        sf::Sprite bg = ctx.render.renderSprite(backgroundPath());
        bg.setPosition(0.f, 0.f);
        window.draw(bg);
    }
}