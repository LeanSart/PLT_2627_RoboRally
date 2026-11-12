#include "RenderEngine.h"
#include <filesystem>
#include <iostream>
namespace UMLDiagram{
void RenderEngine::renderPlateau(const Plateau& plateau){
    float PosY = 100.f;
    for(auto& col : plateau.Grille){
        float PosX = 100.f;
        for(auto& cell : col){
            for(size_t i=0; i<cell.vec_case.size();i++){
                sf::Sprite sprite;
                if (cell.vec_case[i] == TypeCase::CASE_SIMPLE){//Applique la texture correspondant à la case
                    sprite = renderSprite("assets/floor/plain.png");
                }else if(cell.vec_case[i] == TypeCase::MUR){
                    sprite = renderSprite("assets/wall/WallN.png");
                }else if(cell.vec_case[i] == TypeCase::REPAIR_SITE_SIMPLE){
                    sprite = renderSprite("assets/wrench/repair1.png");
                }else if(cell.vec_case[i] == TypeCase::REPAIR_SITE_DOUBLE){
                    sprite = renderSprite("assets/wrench/repair2.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_STRAIGHT){
                    sprite = renderSprite("assets/floor/Conv/yellow/ConvSN.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_RIGHT){
                    sprite = renderSprite("assets/floor/Conv/yellow/ConvSE.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_LEFT){
                    sprite = renderSprite("assets/floor/Conv/yellow/ConvSW.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_STRAIGHT_RIGHT){
                    sprite = renderSprite("assets/floor/Conv/yellow/ConvSEN.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_STRAIGHT_LEFT){
                    sprite = renderSprite("assets/floor/Conv/yellow/ConvSWN.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_STRAIGHT){
                    sprite = renderSprite("assets/floor/Conv/blue/ConvB_SN.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_RIGHT){
                    sprite = renderSprite("assets/floor/Conv/blue/ConvB_SE.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_LEFT){
                    sprite = renderSprite("assets/floor/Conv/blue/ConvB_SW.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_STRAIGHT_RIGHT){
                    sprite = renderSprite("assets/floor/Conv/blue/ConvB_SEN.png");
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_STRAIGHT_LEFT){
                    sprite = renderSprite("assets/floor/Conv/blue/ConvB_SWN.png");
                }else if(cell.vec_case[i] == TypeCase::GEAR_RIGHT){
                    sprite = renderSprite("assets/cogwheel/CogGreen.png");
                }else if(cell.vec_case[i] == TypeCase::GEAR_LEFT){
                    sprite = renderSprite("assets/cogwheel/CogRed.png");
                }else if(cell.vec_case[i] == TypeCase::PUSHER_EVEN){
                    sprite = renderSprite("assets/pusher/1.png");
                }else if(cell.vec_case[i] == TypeCase::PUSHER_ODD){
                    sprite = renderSprite("assets/pusher/6.png");
                }else if(cell.vec_case[i] == TypeCase::LASER_SIMPLE){
                    sprite = renderSprite("assets/laser/LaserStartD.png");
                }else if(cell.vec_case[i] == TypeCase::LASER_DOUBLE){
                    sprite = renderSprite("assets/laser/LaserDStartD.png");
                }else if(cell.vec_case[i] == TypeCase::LASER_TRIPLE){
                    sprite = renderSprite("assets/laser/LaserTStartD.png");
                }else if(cell.vec_case[i] == TypeCase::WELL){
                    sprite = renderSprite("assets/floor/hole.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_1){
                    sprite = renderSprite("assets/startposition/1.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_2){
                    sprite = renderSprite("assets/startposition/2.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_3){
                    sprite = renderSprite("assets/startposition/3.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_4){
                    sprite = renderSprite("assets/startposition/4.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_5){
                    sprite = renderSprite("assets/startposition/5.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_6){
                    sprite = renderSprite("assets/startposition/6.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_7){
                    sprite = renderSprite("assets/startposition/7.png");
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_8){
                    sprite = renderSprite("assets/startposition/8.png");
                }
                sprite.setScale(0.25f, 0.25f);
                sprite.setPosition(PosX, PosY);
                
                sf::FloatRect bounds = sprite.getLocalBounds();
                sprite.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
                if(cell.vec_orientation[i] == Orientation::EST){ //Rotation de la case si neccessaire
                    sprite.setRotation(90.0f);
                }else if(cell.vec_orientation[i] == Orientation::SUD){
                    sprite.setRotation(180.0f);
                }else if(cell.vec_orientation[i] == Orientation::OUEST){
                    sprite.setRotation(270.0f);
                }
                window.draw(sprite);
            }
            PosX = PosX + 75.f;
        }
        PosY = PosY + 75.f;
    }
}


sf::Sprite RenderEngine::renderSprite(std::filesystem::path assetfile){
    std::string path = assetfile.string();
    if (textureCache.find(path) == textureCache.end()) {
        if (std::filesystem::exists(assetfile) && std::filesystem::is_regular_file(assetfile)) { //Verification que l'image existe
            if (!textureCache[path].loadFromFile(assetfile.string())) {
                std::cerr << "[Erreur SFML] Impossible de charger l'image : " << assetfile << std::endl;
            } 
        } else { //Creation d'une image de remplacement 
            std::cerr << "[Erreur] Impossible de trouver le fichier : " << assetfile << std::endl;
            sf::Image fallbackImage;
            fallbackImage.create(300, 300);
            const unsigned int tailleCase = 50;
            for (unsigned int y = 0; y < 300; ++y) {
                for (unsigned int x = 0; x < 300; ++x) {
                    bool estMagenta = ((x / tailleCase) + (y / tailleCase)) % 2 == 0;
                    if (estMagenta) {
                        fallbackImage.setPixel(x, y, sf::Color::Magenta);
                    } else {
                        fallbackImage.setPixel(x, y, sf::Color::Black);
                    }
                }
            }
            textureCache[path].loadFromImage(fallbackImage);
        }
    }
    sf::Sprite sprite;
    sprite.setTexture(textureCache[path]);
    return sprite;
}

void RenderEngine::run(const GameEngine& engine){
    try{
    window.create(sf::VideoMode(1680, 1050), "PLT : RoboRally", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    sf::Sprite spritearchive = renderSprite("assets/archive/red.png");
    sf::Sprite spriterobot = renderSprite("assets/robots/Robot0.png");;
    spritearchive.setScale(0.25f, 0.25f);
    spritearchive.setPosition(400.f, 600.f);
    spriterobot.setScale(0.1715f, 0.1715f);
    spriterobot.setPosition(400.f, 600.f);
        while (window.isOpen()){ //running &&
            sf::Event event{};
            
            while (window.pollEvent(event)){
                if (event.type == sf::Event::Closed){
                    window.close();
                    //running = false;
                    break;
                }
            }
            window.clear(sf::Color::Black);
            //BG.BoardCellGenerator(grid,window);
            renderPlateau(engine.getPlateau());
            window.draw(spritearchive);
            window.draw(spriterobot);
        window.display();
        }
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << "\n";
        // running = false;
    }
}
void RenderEngine::update() {
    // cartes_programme.affichagepile(pioche);
    // pioche.shuffle();
    // cartes_programme.affichagepile(pioche);
}
}