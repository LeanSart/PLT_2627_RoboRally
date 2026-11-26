#include "LoadScreen.h"
#include "GameEngine.h"
#include "Bouton.h"
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <filesystem>

namespace fs = std::filesystem;
namespace UMLDiagram {
inline void centerText(sf::Text& text, sf::Vector2f pos) {
    const sf::FloatRect r = text.getLocalBounds();
    text.setOrigin(r.left + r.width / 2.f, r.top + r.height / 2.f);
    text.setPosition(pos);
}

namespace {
    constexpr float Y_TITRE      = 120.f;
    constexpr float Y_LIST_TOP     = 190.f;
    constexpr float Y_LIST_BOTTOM  = 680.f;
    constexpr float Y_LIST_PADDING = 45.f; 
    constexpr float Y_LIST_STEP    = 80.f;
    constexpr float Y_LIST_START = 240.f;
    constexpr float Y_LANCER     = 750.f;
    constexpr float Y_RETOUR     = 840.f;
    constexpr float SCROLL_SPEED   = 45.f;
}

LoadScreen::LoadScreen(AppContext& context)
    : Screen(context),
      cx(context.window.getSize().x / 2.f),
      titre("Selection de la sauvegarde", context.font, 56),
      messageVide("Aucune sauvegarde trouvee dans le dossier saves/", context.font, 28),
      lancer("Lancer", {cx, Y_LANCER}, context.font),
      retour("Retour", {cx, Y_RETOUR}, context.font) {

    centerText(titre, {cx, Y_TITRE});
    centerText(messageVide, {cx, 400.f});
    messageVide.setFillColor(sf::Color(180, 180, 180));

    const float winWidth = static_cast<float>(context.window.getSize().x);
    listZone = sf::FloatRect(0.f, Y_LIST_TOP, winWidth, Y_LIST_BOTTOM - Y_LIST_TOP);
    // 1. Récupération des fichiers .json dans saves/
    std::vector<fs::path> foundFiles;
    std::error_code ec;
    if (fs::exists("saves", ec) && fs::is_directory("saves", ec)) {
        for (const auto& entry : fs::directory_iterator("saves", ec)) {
            if (entry.is_regular_file() && entry.path().extension() == ".json") {
                foundFiles.push_back(entry.path());
            }
        }
    }

    // Tri par date de modification (la plus récente en haut)
    std::sort(foundFiles.begin(), foundFiles.end(), [](const fs::path& a, const fs::path& b) {
        std::error_code ecA, ecB;
        return fs::last_write_time(a, ecA) > fs::last_write_time(b, ecB);
    });

    // 2. Création des boutons pour chaque sauvegarde
    for (size_t i = 0; i < foundFiles.size(); ++i) {
        const fs::path& p = foundFiles[i];
        std::string saveName = p.stem().string(); // Nom du fichier sans ".json"
        std::string dateStr  = formatLastWriteTime(p);
        std::string label    = saveName + "  -  " + dateStr;

        float yPos = Y_LIST_START + static_cast<float>(i) * Y_LIST_STEP;
        saveSlots.emplace_back(p, label, sf::Vector2f{cx, yPos}, context.font);
    }

    if (!saveSlots.empty()) {
        float totalContentHeight = Y_LIST_PADDING * 2.f + static_cast<float>(saveSlots.size() - 1) * Y_LIST_STEP;
        maxScroll = std::max(0.f, totalContentHeight - listZone.height);

        selectedIndex = 0;
        refreshSelectionVisuals();
    }

    // Sélectionne automatiquement la sauvegarde la plus récente s'il y en a une
    if (!saveSlots.empty()) {
        selectedIndex = 0;
        refreshSelectionVisuals();
    }
}

sf::View LoadScreen::createListView(const sf::RenderTarget& target) const {
    const sf::Vector2u winSize = target.getSize();
    const float w = static_cast<float>(winSize.x);
    const float h = static_cast<float>(winSize.y);

    // La caméra regarde la portion décalée par scrollOffset
    sf::View view(sf::FloatRect(listZone.left, listZone.top + scrollOffset, listZone.width, listZone.height));
    // Le viewport restreint l'affichage uniquement à listZone sur la fenêtre
    view.setViewport(sf::FloatRect(
        listZone.left / w,
        listZone.top / h,
        listZone.width / w,
        listZone.height / h
    ));
    return view;
}

void LoadScreen::clampScroll() {
    scrollOffset = std::clamp(scrollOffset, 0.f, maxScroll);
}

void LoadScreen::ensureSelectedVisible() {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(saveSlots.size())) return;

    const float itemCenterY = Y_LIST_TOP + Y_LIST_PADDING + static_cast<float>(selectedIndex) * Y_LIST_STEP;
    const float itemTop     = itemCenterY - 40.f;
    const float itemBottom  = itemCenterY + 40.f;

    const float visibleTop    = listZone.top + scrollOffset;
    const float visibleBottom = visibleTop + listZone.height;

    if (itemTop < visibleTop) {
        scrollOffset = itemTop - listZone.top;
    } else if (itemBottom > visibleBottom) {
        scrollOffset = itemBottom - listZone.top - listZone.height;
    }
    clampScroll();
}

std::string LoadScreen::formatLastWriteTime(const fs::path& p) {
    std::error_code ec;
    auto ftime = fs::last_write_time(p, ec);
    if (ec) return "Date inconnue";

    // Conversion compatible C++17/C++20 de file_time_type vers system_clock
    auto sctp = std::chrono::time_point_cast<std::chrono::system_clock::duration>(
        ftime - fs::file_time_type::clock::now() + std::chrono::system_clock::now()
    );
    std::time_t tt = std::chrono::system_clock::to_time_t(sctp);

    std::tm tm{};
#ifdef _WIN32
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y %H:%M");
    return oss.str();
}

void LoadScreen::refreshSelectionVisuals() {
    for (size_t i = 0; i < saveSlots.size(); ++i) {
        saveSlots[i].bouton.setSelected(static_cast<int>(i) == selectedIndex);
    }
}

void LoadScreen::loadSelectedGame() {
    if (selectedIndex < 0 || selectedIndex >= static_cast<int>(saveSlots.size())) {
        std::cerr << "[Info] Aucune sauvegarde selectionnee." << std::endl;
        return;
    }

    const fs::path& file_path = saveSlots[selectedIndex].filepath;

    // Charge directement la partie sans ouvrir le fichier en écriture !
    if (ctx.engine.load(file_path)) {
        ctx.goTo(ScreenId::PARTIE);
        return;
    }

    std::cerr << "[Erreur] Sauvegarde corrompue ou invalide : " << file_path << std::endl;
}

void LoadScreen::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseWheelScrolled &&
        event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
        const sf::Vector2f mousePos = ctx.window.mapPixelToCoords({event.mouseWheelScroll.x, event.mouseWheelScroll.y});
        if (listZone.contains(mousePos)) {
            scrollOffset -= event.mouseWheelScroll.delta * SCROLL_SPEED;
            clampScroll();
        }
    }

    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        const sf::Vector2f p = ctx.window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});

        // Clic sur une des sauvegardes
        for (size_t i = 0; i < saveSlots.size(); ++i) {
            if (saveSlots[i].bouton.contient(p)) {
                selectedIndex = static_cast<int>(i);
                refreshSelectionVisuals();
                break;
            }
        }

        // Clic sur Lancer (uniquement si une partie est sélectionnée)
        if (lancer.contient(p) && selectedIndex != -1) {
            loadSelectedGame();
        }

        if (retour.contient(p)) {
            ctx.goTo(ScreenId::TITRE);
        }
    }

    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Return && selectedIndex != -1) {
            loadSelectedGame();
        }
        if (event.key.code == sf::Keyboard::Escape) {
            ctx.goTo(ScreenId::TITRE);
        }
        // Navigation au clavier (Haut / Bas)
        if (!saveSlots.empty()) {
            if (event.key.code == sf::Keyboard::Up) {
                selectedIndex = (selectedIndex - 1 + static_cast<int>(saveSlots.size())) % static_cast<int>(saveSlots.size());
                refreshSelectionVisuals();
                ensureSelectedVisible();
            } else if (event.key.code == sf::Keyboard::Down) {
                selectedIndex = (selectedIndex + 1) % static_cast<int>(saveSlots.size());
                refreshSelectionVisuals();
                ensureSelectedVisible();
            }
        }
    }
}

void LoadScreen::update(float dt) {
    (void)dt;
    const sf::Vector2i pixelPos = sf::Mouse::getPosition(ctx.window);
    const sf::Vector2f m = ctx.window.mapPixelToCoords(pixelPos);
    const bool inListZone = listZone.contains(m);
    const sf::Vector2f listMouse = ctx.window.mapPixelToCoords(pixelPos, createListView(ctx.window));

    for (auto& slot : saveSlots) {
        // Évite le survol fantôme des boutons cachés en dehors de la zone visible
        slot.bouton.setHover(inListZone && slot.bouton.contient(listMouse));
    }

    lancer.setHover(selectedIndex != -1 && lancer.contient(m));
    retour.setHover(retour.contient(m));
}

void LoadScreen::draw(sf::RenderWindow& window) {
    window.draw(titre);

    if (saveSlots.empty()) {
        window.draw(messageVide);
    } else {
        const sf::View defaultView = window.getView();
        window.setView(createListView(window));
        for (const auto& slot : saveSlots) {
            slot.bouton.draw(window);
        }
        window.setView(defaultView);
    }
    if (maxScroll > 0.f) {
        const float trackX = cx + 420.f; 
        const float trackW = 8.f;

        sf::RectangleShape track({trackW, listZone.height});
        track.setPosition(trackX, listZone.top);
        track.setFillColor(sf::Color(255, 255, 255, 40));
        window.draw(track);

        const float totalHeight = listZone.height + maxScroll;
        const float thumbHeight = std::max(30.f, listZone.height * (listZone.height / totalHeight));
        const float thumbY = listZone.top + (scrollOffset / maxScroll) * (listZone.height - thumbHeight);

        sf::RectangleShape thumb({trackW, thumbHeight});
        thumb.setPosition(trackX, thumbY);
        thumb.setFillColor(sf::Color(220, 220, 220, 180));
        window.draw(thumb);
    }
    
    // N'affiche le bouton Lancer que si une sauvegarde est sélectionnée
    if (selectedIndex != -1) {
        lancer.draw(window);
    }
    retour.draw(window);
}

std::string LoadScreen::backgroundPath() const {
    return "assets/background/Conf.jpg";
}
}