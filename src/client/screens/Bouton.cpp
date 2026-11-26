#include "Bouton.h"
#include <SFML/Graphics.hpp>
#include <algorithm>

namespace screens {
    inline void centerText(sf::Text& text, sf::Vector2f pos) {
        const sf::FloatRect r = text.getLocalBounds();
        text.setOrigin(r.left + r.width / 2.f, r.top + r.height / 2.f);
        text.setPosition(pos);
    }

    const sf::Color Bouton::couleurNormale         = sf::Color(60, 60, 60);
    const sf::Color Bouton::couleurSurvol          = sf::Color(100, 100, 100);
    const sf::Color Bouton::couleurSelectionFond   = sf::Color(80, 75, 50);
    const sf::Color Bouton::couleurSelectionSurvol = sf::Color(110, 100, 65);
    const sf::Color Bouton::couleurSelection       = sf::Color(255, 215, 0);

    Bouton::Bouton(const std::string& label, sf::Vector2f centre, const sf::Font& font,
                sf::Vector2f taille, unsigned int charSize)
        : tailleMin(taille) {
        forme.setSize(taille);
        forme.setOrigin(taille / 2.f);
        forme.setPosition(centre);
        forme.setOutlineThickness(0.f);
        forme.setOutlineColor(couleurSelection);

        texte.setFont(font);
        texte.setCharacterSize(charSize);
        setLabel(label);
        appliquerStyle();
    }

        void Bouton::setLabel(const std::string& label) {
        texte.setString(label);

        // Ajuste la largeur si le texte est plus large que tailleMin.x (avec marge de 40px)
        const float largeurTexte = texte.getLocalBounds().width + 40.f;
        const float nouvelleLargeur = std::max(tailleMin.x, largeurTexte);
        const sf::Vector2f centre = forme.getPosition();

        forme.setSize({nouvelleLargeur, tailleMin.y});
        forme.setOrigin(nouvelleLargeur / 2.f, tailleMin.y / 2.f);
        forme.setPosition(centre);

        centerText(texte, centre);
    }

    bool Bouton::contient(sf::Vector2f p) const { return forme.getGlobalBounds().contains(p); }

    void Bouton::setHover(bool survol) {
        if (estSurvole != survol) {
            estSurvole = survol;
            appliquerStyle();
        }
    }

    void Bouton::setSelected(bool selectionne) {
        if (estSelectionne != selectionne) {
            estSelectionne = selectionne;
            appliquerStyle();
        }
    }

    void Bouton::draw(sf::RenderTarget& target) const {
        target.draw(forme);
        target.draw(texte);
    }

    void Bouton::appliquerStyle() {
        if (estSelectionne) {
            forme.setFillColor(estSurvole ? couleurSelectionSurvol : couleurSelectionFond);
            forme.setOutlineThickness(3.f);
            texte.setFillColor(couleurSelection);
        } else {
            forme.setFillColor(estSurvole ? couleurSurvol : couleurNormale);
            forme.setOutlineThickness(0.f);
            texte.setFillColor(sf::Color::White);
        }
    }


}