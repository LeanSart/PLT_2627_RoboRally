#include "TitleScreen.h"
#include "AppContext.h"
namespace UMLDiagram {
namespace {
    float centreX(const AppContext& ctx) { return ctx.window.getSize().x / 2.f; }
 
    const sf::Vector2f TAILLE_BOUTON{420.f, 70.f};
    constexpr float Y_PREMIER  = 450.f;
    constexpr float ESPACEMENT = 90.f;
}

inline void centerText(sf::Text& text, sf::Vector2f pos) {
    const sf::FloatRect r = text.getLocalBounds();
    text.setOrigin(r.left + r.width / 2.f, r.top + r.height / 2.f);
    text.setPosition(pos);
}

TitleScreen::TitleScreen(AppContext& context)
    : Screen(context),
      titre("ROBORALLY", context.font, 96),
      nouvellePartie("Nouvelle partie",    {centreX(context), Y_PREMIER + 0 * ESPACEMENT}, context.font, TAILLE_BOUTON),
      charger       ("Charger une partie", {centreX(context), Y_PREMIER + 1 * ESPACEMENT}, context.font, TAILLE_BOUTON),
      multijoueur   ("Multijoueur",        {centreX(context), Y_PREMIER + 2 * ESPACEMENT}, context.font, TAILLE_BOUTON),
      quitter       ("Quitter",            {centreX(context), Y_PREMIER + 3 * ESPACEMENT}, context.font, TAILLE_BOUTON) {
    centerText(titre, {centreX(ctx), 250.f});
}

void TitleScreen::ouvrirMultijoueur() {
    // Volontairement vide pour le moment : le mode reseau sera ajoute plus tard
    // (ex. ctx.goTo(ScreenId::MULTIJOUEUR) une fois l'ecran cree).
}

void TitleScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f p = ctx.window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
        if (nouvellePartie.contient(p)) ctx.goTo(ScreenId::CONFIGURATION);
        if (charger.contient(p))        ctx.goTo(ScreenId::CHARGEMENT);
        if (multijoueur.contient(p))    ouvrirMultijoueur();
        if (quitter.contient(p))        ctx.goTo(ScreenId::QUITTER);
    }
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return) ctx.goTo(ScreenId::CONFIGURATION);
        if (event.key.code == sf::Keyboard::Escape) ctx.goTo(ScreenId::QUITTER);
    }
}

void TitleScreen::update(float) {
    const sf::Vector2f m = ctx.window.mapPixelToCoords(sf::Mouse::getPosition(ctx.window));
    for (Bouton* b : {&nouvellePartie, &charger, &multijoueur, &quitter})
        b->setHover(b->contient(m));
}
 
void TitleScreen::draw(sf::RenderWindow& window) {
    window.draw(titre);
    for (const Bouton* b : {&nouvellePartie, &charger, &multijoueur, &quitter})
        b->draw(window);
}

std::string TitleScreen::backgroundPath() const {
    return "assets/background/Title.jpg";
}
}