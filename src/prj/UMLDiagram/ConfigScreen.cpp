
#include "ConfigScreen.h"
#include "GameEngine.h"
#include <algorithm>
#include <filesystem>
#include <system_error>
#include <fstream>
#include <iostream>
#include <cmath>
#include "../../../extern/jsoncpp-1.8.0/json/json.h"

namespace fs = std::filesystem;
namespace UMLDiagram{
    inline void centerText(sf::Text& text, sf::Vector2f pos) {
        const sf::FloatRect r = text.getLocalBounds();
        text.setOrigin(r.left + r.width / 2.f, r.top + r.height / 2.f);
        text.setPosition(pos);
    }
    namespace {
        constexpr float Y_NOM     = 250.f;
        constexpr float Y_JOUEURS = 350.f;
        constexpr float Y_PLATEAU = 500.f;
        const sf::Vector2f BTN_PETIT{70.f, 70.f};
    }
    
    ConfigScreen::ConfigScreen(AppContext& context)
        : Screen(context),
        cx(context.window.getSize().x / 2.f),
        titre("Configuration de la partie", context.font, 64),
        labelNom("", context.font, 36),
        champNom({600.f, 60.f}),
        labelJoueurs("", context.font, 36),
        labelPlateau("", context.font, 36),
        joueursMoins("-", {cx - 250.f, Y_JOUEURS}, context.font, BTN_PETIT),
        joueursPlus ("+", {cx + 250.f, Y_JOUEURS}, context.font, BTN_PETIT),
        plateauPrec ("<", {cx - 400.f, Y_PLATEAU}, context.font, BTN_PETIT),
        plateauSuiv (">", {cx + 400.f, Y_PLATEAU}, context.font, BTN_PETIT),
        lancer("Lancer", {cx, 750.f}, context.font),
        retour("Retour", {cx, 850.f}, context.font) {
    
        // Liste des plateaux disponibles
        std::error_code ec;
        for (const auto& entry : fs::directory_iterator("assets/boards", ec)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json")
                plateaux.push_back(entry.path().generic_string());
        }
        std::sort(plateaux.begin(), plateaux.end());
    
        const auto it = std::find(plateaux.begin(), plateaux.end(), current_plateau);
        indexPlateau = (it != plateaux.end()) ? static_cast<std::size_t>(it - plateaux.begin()) : 0;
        current_plateau = plateaux[indexPlateau];
    
        centerText(titre, {cx, 150.f});

        champNom.setOrigin(champNom.getSize() / 2.f);
        champNom.setPosition(cx, Y_NOM);
        champNom.setFillColor(sf::Color(30, 30, 30, 200));
        champNom.setOutlineThickness(2.f);
        champNom.setOutlineColor(sf::Color::White);

        refreshLabels();
    }
    
    void ConfigScreen::changeJoueurs(int delta) {
        current_nb_joueurs = std::clamp(current_nb_joueurs + delta, MIN_JOUEURS, MAX_JOUEURS);
        refreshLabels();
    }
    
    void ConfigScreen::changePlateau(int delta) {
        const int n = static_cast<int>(plateaux.size());
        const int i = (static_cast<int>(indexPlateau) + delta + n) % n;
        indexPlateau = static_cast<std::size_t>(i);
        current_plateau = plateaux[indexPlateau];
        refreshLabels();
    }

    void ConfigScreen::launchNewGame() {
        if (name.empty()) name = "nouvelle_course";
        std::string plateau_name = fs::path(current_plateau).stem().string();

        Json::Value save_data;
        save_data["nb_joueurs"] = current_nb_joueurs;
        save_data["plateau"] = plateau_name;
        fs::path file_path = "saves/" + name + ".json";
        std::ofstream file(file_path);

        if (file.is_open()) {
            Json::StreamWriterBuilder builder;
            builder["indentation"] = "    ";
            file << Json::writeString(builder, save_data);
            file.close();
        } else {
            std::cerr << "Erreur : impossible de créer le fichier " << file_path << std::endl;
        }
        if (ctx.engine.load(file_path)) {
            ctx.goTo(ScreenId::PARTIE);
            return;
        }
        std::cerr << "[Erreur] Sauvegarde absente ou invalide : retour a la configuration" << std::endl;
        ctx.goTo(ScreenId::CONFIGURATION);
        
    }
    
    void ConfigScreen::refreshLabels() {
        const bool curseur = std::fmod(curseurTimer, 1.f) < 0.5f;
        labelNom.setString("Nom : " + name + (curseur ? "_" : " "));
        centerText(labelNom, {cx, Y_NOM});

        labelJoueurs.setString("Joueurs : " + std::to_string(current_nb_joueurs));
        centerText(labelJoueurs, {cx, Y_JOUEURS});
    
        labelPlateau.setString("Plateau : " + fs::path(current_plateau).stem().string());
        centerText(labelPlateau, {cx, Y_PLATEAU});
    }
    
    void ConfigScreen::handleEvent(const sf::Event& event) {
        if (event.type == sf::Event::MouseButtonPressed &&
            event.mouseButton.button == sf::Mouse::Left) {
            const sf::Vector2f p = ctx.window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
            if (joueursMoins.contient(p)) changeJoueurs(-1);
            if (joueursPlus.contient(p))  changeJoueurs(+1);
            if (plateauPrec.contient(p))  changePlateau(-1);
            if (plateauSuiv.contient(p))  changePlateau(+1);
            if (lancer.contient(p))       launchNewGame();
            if (retour.contient(p))       ctx.goTo(ScreenId::TITRE);
        }
        if (event.type == sf::Event::TextEntered) {
            const sf::Uint32 c = event.text.unicode;
            if (c == 8) {                                   // retour arrière
                if (!name.empty()) name.pop_back();
            } else if (name.size() < MAX_NOM &&
                    ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
                        (c >= '0' && c <= '9') || c == '_' || c == '-')) {
                // caractères sûrs pour un nom de fichier
                name += static_cast<char>(c);
            }
            refreshLabels();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Return) launchNewGame();
            if (event.key.code == sf::Keyboard::Escape) ctx.goTo(ScreenId::TITRE);
        }
    }
    
    void ConfigScreen::update(float dt) {
        curseurTimer += dt;
        refreshLabels();
    
        const sf::Vector2f m = ctx.window.mapPixelToCoords(sf::Mouse::getPosition(ctx.window));
        for (Bouton* b : {&joueursMoins, &joueursPlus, &plateauPrec, &plateauSuiv, &lancer, &retour})
            b->setHover(b->contient(m));
    }
    
    void ConfigScreen::draw(sf::RenderWindow& window) {
        window.draw(titre);
        window.draw(champNom);
        window.draw(labelNom);
        window.draw(labelJoueurs);
        window.draw(labelPlateau);
        for (const Bouton* b : {&joueursMoins, &joueursPlus, &plateauPrec, &plateauSuiv, &lancer, &retour})
            b->draw(window);
    }

    std::string ConfigScreen::backgroundPath() const {
    return "assets/background/Conf.jpg";
    }
}