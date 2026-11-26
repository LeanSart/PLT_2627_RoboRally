    #include "RenderEngine.h"
    #include "Screen.h"
    #include "ScreenManager.h"
    #include <filesystem>
    #include <algorithm>
    #include <iostream>
    #include <string>
    #include <stdexcept>
    #include "Orientation.h"
    #include "TypeCase.h"
    #include "AppContext.h"
    #include "GameEngine.h"
    
    namespace UMLDiagram {

    namespace {
        
        /// CONSTANTES
        
        // Disposition de la fenetre (1900 x 1000)
        constexpr unsigned int WINDOW_W = 1900;
        constexpr unsigned int WINDOW_H = 1000;

        constexpr float MARGIN = 150.f;       // sert uniquement au calcul de la taille des cases (inchange)
        constexpr float BOARD_MARGIN = 50.f;  // marge entre le plateau et les bords (haut / gauche)
        constexpr float BOARD_AREA_W = 860.f; // largeur de la zone du plateau (a gauche)

        const sf::Vector2f BOARD_ORIGIN{ BOARD_MARGIN + 50.f, BOARD_MARGIN };
        
        //Tableau de bord à droite
        constexpr float DASH_X   = BOARD_MARGIN + BOARD_AREA_W + 50.f;
        constexpr float DASH_TOP = BOARD_MARGIN; // aligne avec le haut du plateau


        constexpr float HT_WIDTH         = 80.f;  // icone hors-tension (carree)
        constexpr float LIFE_WIDTH       = 300.f; // icone des vies (300 x 60)
        constexpr float LIFE_OFFSET_X    = 300.f;
        constexpr float LIFE_OFFSET_Y    = 10.f;
        constexpr int   MAX_LIVES        = 3;
        constexpr int   MAX_DAMAGE       = 10;
        constexpr float WARN_SIZE        = 60.f; // icones de degats ~ taille d'une case
        constexpr float WARN_GAP         = 12.f;
        constexpr float WARN_OFFSET_Y    = 130.f;

        // En cas de texture manquante dans un sprite
        constexpr unsigned int FALLBACK_SIZE = 300;
        constexpr unsigned int FALLBACK_TILE = 50;



        /// UTILITAIRES

        // Taille d'une case : le plateau entier doit tenir dans sa zone
        float computeCellSize(const Plateau& plateau) {
            const size_t rows = plateau.Grille.size();
            const size_t cols = rows > 0 ? plateau.Grille[0].size() : 0;
            if (rows == 0 || cols == 0) return 75.f;
        
            const float maxW = BOARD_AREA_W / static_cast<float>(cols);
            const float maxH = (static_cast<float>(WINDOW_H) - 2.f * MARGIN) / static_cast<float>(rows);
            return std::min(maxW, maxH);
        }
        
        // Redimensionne le sprite en carre de 'size' pixels et centre son origine
        void fitSprite(sf::Sprite& sprite, float size) {
            const sf::FloatRect b = sprite.getLocalBounds();
            if (b.width <= 0.f || b.height <= 0.f) return;
            sprite.setOrigin(b.width / 2.f, b.height / 2.f);
            sprite.setScale(size / b.width, size / b.height);
        }
        
        // Redimensionne le sprite pour qu'il fasse 'width' pixels de large (ratio conserve)
        void fitSpriteWidth(sf::Sprite& sprite, float width) {
            const sf::FloatRect b = sprite.getLocalBounds();
            if (b.width <= 0.f) return;
            const float k = width / b.width;
            sprite.setScale(k, k);
        }

        // Chemin de la texture associee a un type de case (nullptr si inconnu)
        const char* texturePath(TypeCase type) {
            switch (type) {
                case TypeCase::CASE_SIMPLE:                return "assets/floor/plain.png";
                case TypeCase::MUR:                        return "assets/wall/WallN.png";
                case TypeCase::REPAIR_SITE_SIMPLE:         return "assets/wrench/repair1.png";
                case TypeCase::REPAIR_SITE_DOUBLE:         return "assets/wrench/repair2.png";
                case TypeCase::CONV_SIMPLE_STRAIGHT:       return "assets/floor/Conv/yellow/ConvSN.png";
                case TypeCase::CONV_SIMPLE_RIGHT:          return "assets/floor/Conv/yellow/ConvSE.png";
                case TypeCase::CONV_SIMPLE_LEFT:           return "assets/floor/Conv/yellow/ConvSW.png";
                case TypeCase::CONV_SIMPLE_STRAIGHT_RIGHT: return "assets/floor/Conv/yellow/ConvSEN.png";
                case TypeCase::CONV_SIMPLE_STRAIGHT_LEFT:  return "assets/floor/Conv/yellow/ConvSWN.png";
                case TypeCase::CONV_DOUBLE_STRAIGHT:       return "assets/floor/Conv/blue/ConvB_SN.png";
                case TypeCase::CONV_DOUBLE_RIGHT:          return "assets/floor/Conv/blue/ConvB_SE.png";
                case TypeCase::CONV_DOUBLE_LEFT:           return "assets/floor/Conv/blue/ConvB_SW.png";
                case TypeCase::CONV_DOUBLE_STRAIGHT_RIGHT: return "assets/floor/Conv/blue/ConvB_SEN.png";
                case TypeCase::CONV_DOUBLE_STRAIGHT_LEFT:  return "assets/floor/Conv/blue/ConvB_SWN.png";
                case TypeCase::GEAR_RIGHT:                 return "assets/cogwheel/CogGreen.png";
                case TypeCase::GEAR_LEFT:                  return "assets/cogwheel/CogRed.png";
                case TypeCase::PUSHER_EVEN:                return "assets/pusher/1.png";
                case TypeCase::PUSHER_ODD:                 return "assets/pusher/6.png";
                case TypeCase::LASER_SIMPLE:               return "assets/laser/LaserStartD.png";
                case TypeCase::LASER_DOUBLE:               return "assets/laser/LaserDStartD.png";
                case TypeCase::LASER_TRIPLE:               return "assets/laser/LaserTStartD.png";
                case TypeCase::WELL:                       return "assets/floor/hole.png";
                case TypeCase::START_DOCK_1:               return "assets/startposition/1.png";
                case TypeCase::START_DOCK_2:               return "assets/startposition/2.png";
                case TypeCase::START_DOCK_3:               return "assets/startposition/3.png";
                case TypeCase::START_DOCK_4:               return "assets/startposition/4.png";
                case TypeCase::START_DOCK_5:               return "assets/startposition/5.png";
                case TypeCase::START_DOCK_6:               return "assets/startposition/6.png";
                case TypeCase::START_DOCK_7:               return "assets/startposition/7.png";
                case TypeCase::START_DOCK_8:               return "assets/startposition/8.png";
                default:                                   return nullptr;
            }
        }

        // Angle de rotation (en degres) correspondant a une orientation
        float rotationFor(Orientation o) {
            switch (o) {
                case Orientation::EST:   return 90.f;
                case Orientation::SUD:   return 180.f;
                case Orientation::OUEST: return 270.f;
                default:                 return 0.f;
            }
        }

        // Image de remplacement (damier magenta / noir) quand un asset est introuvable
        sf::Image makeFallbackImage() {
            sf::Image image;
            image.create(FALLBACK_SIZE, FALLBACK_SIZE);
            for (unsigned int y = 0; y < FALLBACK_SIZE; ++y) {
                for (unsigned int x = 0; x < FALLBACK_SIZE; ++x) {
                    const bool magenta = ((x / FALLBACK_TILE) + (y / FALLBACK_TILE)) % 2 == 0;
                    image.setPixel(x, y, magenta ? sf::Color::Magenta : sf::Color::Black);
                }
            }
            return image;
        }

        // Chemin de l'icone de degats numero 'i' (0 = rouge, puis jaunes, puis blanches)
        std::string damageIconPath(int i, int activeCount) {
            if (i >= activeCount) return "assets/dashboard/warning/white.png";
            return i == 0 ? "assets/dashboard/warning/red.png" : "assets/dashboard/warning/yellow.png";
        }

    }

    
    
    void RenderEngine::renderPlateau(const Plateau& plateau){
        const float cell_size = computeCellSize(plateau);
        float PosY = BOARD_ORIGIN.y + cell_size / 2.f; // centre de la case
        for(auto& row : plateau.Grille){
            float PosX = BOARD_ORIGIN.x + cell_size / 2.f;
            for(auto& cell : row){
                for(size_t i=0; i<cell.vec_case.size();i++){
                    const char* path = texturePath(cell.vec_case[i]);
                    if (!path) continue; // type de case sans texture : rien a dessiner
                    sf::Sprite sprite = renderSprite(path);
                    fitSprite(sprite, cell_size); //Taille = une case (origine centree)
                    sprite.setPosition(PosX, PosY);
                    sprite.setRotation(rotationFor(cell.vec_orientation[i]));
                    window.draw(sprite);
                }
                PosX += cell_size;
            }
            PosY += cell_size;
        }
    }
    
    void RenderEngine::renderTableau_2_bord(const Tableau_2_bord& t2b) {
        const int activeDamage = MAX_DAMAGE - t2b.getPoint_2_degat();// Points de degats : 10 icones (rouge, jaunes, puis blanches)
        if (activeDamage >= 0 && activeDamage <= MAX_DAMAGE) {
            float posX = DASH_X;
            for (int i = 0; i < MAX_DAMAGE; ++i) {
                sf::Sprite icon = renderSprite(damageIconPath(i, activeDamage));
                fitSprite(icon, WARN_SIZE);
                icon.setPosition(posX + WARN_SIZE / 2.f, DASH_TOP + WARN_OFFSET_Y + WARN_SIZE / 2.f);
                window.draw(icon);
                posX += WARN_SIZE + WARN_GAP;
            }
        }
    
        // Hors-tension
        sf::Sprite hors = renderSprite(t2b.getHors_tension() ? "assets/dashboard/hors-tension/activate.jpg" : "assets/dashboard/hors-tension/desactivate.jpg");
        fitSpriteWidth(hors, HT_WIDTH);
        hors.setPosition(DASH_X, DASH_TOP);
        window.draw(hors);
    
        // Points de vie
        const int lives = t2b.getPoint_2_vie();
        if (lives >= 0 && lives <= MAX_LIVES) {
            sf::Sprite life = renderSprite("assets/dashboard/lifepoints/" + std::to_string(lives) + "lives.png");
            fitSpriteWidth(life, LIFE_WIDTH);
            life.setPosition(DASH_X + LIFE_OFFSET_X, DASH_TOP + LIFE_OFFSET_Y);
            window.draw(life);
        }
    }
    
    void RenderEngine::renderRobot(const Robot& robot) {
        sf::Sprite rbt = renderSprite("assets/robots/Robot0.png");
        // fitSprite(rbt, ...);
        rbt.setPosition(BOARD_ORIGIN.x + robot.get_position().PosX, BOARD_ORIGIN.y + robot.get_position().PosY);
        rbt.setRotation(rotationFor(robot.orientation));
        window.draw(rbt);
    }

    sf::Sprite RenderEngine::renderSprite(std::filesystem::path assetfile){
        std::string path = assetfile.string();
        
        auto [it, inserted] = textureCache.try_emplace(path);
        sf::Texture& texture = it->second;
        
        if (inserted) {
            const bool exists = std::filesystem::is_regular_file(assetfile);
            if (!exists) {
                std::cerr << "[Erreur] Impossible de trouver le fichier : " << assetfile << std::endl;
                texture.loadFromImage(makeFallbackImage());
            } else if (!texture.loadFromFile(path)) {
                std::cerr << "[Erreur SFML] Impossible de charger l'image : " << assetfile << std::endl;
                texture.loadFromImage(makeFallbackImage());
            }
            texture.setSmooth(true);   // lissage : evite l'effet de damier a la reduction
            texture.generateMipmap();
        }
        
        sf::Sprite sprite;
        sprite.setTexture(texture);
        return sprite;
    }
    
    void RenderEngine::run(){
        try{
            window.create(sf::VideoMode(WINDOW_W, WINDOW_H), "PLT : RoboRally", sf::Style::Titlebar | sf::Style::Close);
            window.setFramerateLimit(60);
            
            sf::Font font;
            const char* fontPath = "assets/fonts/ROBO.ttf";
            if (!font.loadFromFile(fontPath))
                throw std::runtime_error(std::string("Impossible de charger la police : ") + fontPath);
            GameEngine engine = GameEngine();
            AppContext ctx{window, font, *this, engine};
            ScreenManager screens(ctx);
            sf::Clock clock;
                
            while (window.isOpen() && !screens.isFinished()){
                sf::Event event{};
                while (window.pollEvent(event)){
                    if (event.type == sf::Event::Closed){
                        window.close();
                        break;
                    }
                    screens.handleEvent(event);
                }
                if (!window.isOpen()) break;
                
                screens.update(clock.restart().asSeconds());
                screens.draw(window);
                window.display();
                screens.applyTransition(); // changement d'ecran applique en fin de frame
            }
            if (window.isOpen()) window.close();
        } catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << "\n";
        }
    }
    
    void RenderEngine::update(const Plateau& plateau,const Robot& robot,const Tableau_2_bord& t2b) {
        renderPlateau(plateau);
        renderRobot(robot);
        renderTableau_2_bord(t2b);
    }
} // namespace UMLDiagram