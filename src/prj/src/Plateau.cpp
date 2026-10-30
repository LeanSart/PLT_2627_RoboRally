#include "Plateau.h"


Plateau::Plateau(const std::string& BoardPath){
        loadPlateau(BoardPath);
    }

Case_Plateau Plateau::getCase (Position position){return Grille.at(position.PosX).at(position.PosY);}

/**
 * Charge un plateau depuis un fichier JSON.
 * 
 * @param filePath Chemin vers le fichier JSON
 * @return void ou erreur
 */
void Plateau::loadPlateau(const std::string& filePath){
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + filePath);
    }
    json donneesJson;
    try {
        file >> donneesJson;
    } catch (const json::parse_error& e) {
        throw std::runtime_error("Erreur de syntaxe JSON : " + std::string(e.what()));
    }
     
    if (donneesJson.contains("width")) {
        Largeur = donneesJson["width"].get<int>();
    } else {
        throw std::invalid_argument("Le champ 'width' est introuvable.");
    } 
    
    if (donneesJson.contains("height")) {
        Hauteur = donneesJson["height"].get<int>();
    } else {
        throw std::invalid_argument("Le champ 'height' est introuvable.");
    }
    std::string grid_json;
    if (donneesJson.contains("grid")) {
        grid_json = donneesJson["grid"].get<std::string>();
    } else {
        throw std::invalid_argument("Le champ 'grid' est introuvable.");
    }

    std::vector<std::string> cell_list;
    std::stringstream ss(grid_json);
    std::string cell;
    
    while (ss >> cell) { //Parsing des cases plateau en une liste
        cell_list.push_back(cell);
    }
    
    Grille.assign(Hauteur, std::vector<Case_Plateau>(Largeur));
    int col = 0;
    int row = 0; 
    for(const auto& cell : cell_list){
        Case_Plateau current_cell;

        std::vector<std::string> el_list;
        std::stringstream ss2(cell);
        std::string el;
    
        while (std::getline(ss2, el, '_')) { //Parsing des cases plateau en une liste
            el_list.push_back(el);
        }

        for(const auto& el : el_list){
            if(el.front() == 'a'){
                current_cell.vec_case.push_back(TypeCase::CASE_SIMPLE);
            }else if(el.front() == 'b'){
                current_cell.vec_case.push_back(TypeCase::MUR);
            }else if(el.front() == 'c'){
                current_cell.vec_case.push_back(TypeCase::REPAIR_SITE_SIMPLE);
            }else if(el.front() == 'd'){
                current_cell.vec_case.push_back(TypeCase::REPAIR_SITE_DOUBLE);
            }else if(el.front() == 'f'){
                current_cell.vec_case.push_back(TypeCase::CONV_SIMPLE_STRAIGHT);
            }else if(el.front() == 'g'){
                current_cell.vec_case.push_back(TypeCase::CONV_SIMPLE_RIGHT);
            }else if(el.front() == 'h'){
                current_cell.vec_case.push_back(TypeCase::CONV_SIMPLE_LEFT);
            }else if(el.front() == 'i'){
                current_cell.vec_case.push_back(TypeCase::CONV_SIMPLE_STRAIGHT_RIGHT);
            }else if(el.front() == 'j'){
                current_cell.vec_case.push_back(TypeCase::CONV_SIMPLE_STRAIGHT_LEFT);
            }else if(el.front() == 'k'){
                current_cell.vec_case.push_back(TypeCase::CONV_DOUBLE_STRAIGHT);
            }else if(el.front() == 'l'){
                current_cell.vec_case.push_back(TypeCase::CONV_DOUBLE_RIGHT);
            }else if(el.front() == 'm'){
                current_cell.vec_case.push_back(TypeCase::CONV_DOUBLE_LEFT);
            }else if(el.front() == 'o'){
                current_cell.vec_case.push_back(TypeCase::CONV_DOUBLE_STRAIGHT_RIGHT);
            }else if(el.front() == 'p'){
                current_cell.vec_case.push_back(TypeCase::CONV_DOUBLE_STRAIGHT_LEFT);
            }else if(el.front() == 'q'){
                current_cell.vec_case.push_back(TypeCase::GEAR_RIGHT);
            }else if(el.front() == 'r'){
                current_cell.vec_case.push_back(TypeCase::GEAR_LEFT);
            }else if(el.front() == 't'){
                current_cell.vec_case.push_back(TypeCase::PUSHER_EVEN);
            }else if(el.front() == 'u'){
                current_cell.vec_case.push_back(TypeCase::PUSHER_ODD);
            }else if(el.front() == 'v'){
                current_cell.vec_case.push_back(TypeCase::LASER_SIMPLE);
            }else if(el.front() == 'x'){
                current_cell.vec_case.push_back(TypeCase::LASER_DOUBLE);
            }else if(el.front() == 'y'){
                current_cell.vec_case.push_back(TypeCase::LASER_TRIPLE);
            }else if(el.front() == 'z'){
                current_cell.vec_case.push_back(TypeCase::WELL);
            }else if(el.front() == '1'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_1);
            }else if(el.front() == '2'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_2);
            }else if(el.front() == '3'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_3);
            }else if(el.front() == '4'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_4);
            }else if(el.front() == '5'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_5);
            }else if(el.front() == '6'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_6);
            }else if(el.front() == '7'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_7);
            }else if(el.front() == '8'){
                current_cell.vec_case.push_back(TypeCase::START_DOCK_8);
            }
            
            if(el.back() == 'E'){
                current_cell.vec_orientation.push_back(Orientation::EST);
            }else if(el.back() == 'S'){
                current_cell.vec_orientation.push_back(Orientation::SUD);
            }else if(el.back() == 'W'){
                current_cell.vec_orientation.push_back(Orientation::OUEST);
            }else if(el.back() == 'N'){
                current_cell.vec_orientation.push_back(Orientation::NORD);
            }
        }
        Grille[row][col] = current_cell; 
        if(col == (Largeur-1)){col = 0;row++;} else{col++;}
        if(row == (Hauteur)) {break;}
    }
    return;
}


void Plateau::affichePlateau(sf::RenderWindow& window){
    float PosY = 100.f;
    for(auto& col : Grille){
        float PosX = 100.f;
        for(auto& cell : col){
            for(size_t i=0; i<cell.vec_case.size();i++){
                sf::Texture texture;
                sf::Sprite sprite;
                if (cell.vec_case[i] == TypeCase::CASE_SIMPLE){//Applique la texture correspondant à la case
                    if (!texture.loadFromFile("assets/floor/plain.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::MUR){
                    if (!texture.loadFromFile("assets/wall/WallN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::REPAIR_SITE_SIMPLE){
                    if (!texture.loadFromFile("assets/wrench/repair1.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::REPAIR_SITE_DOUBLE){
                    if (!texture.loadFromFile("assets/wrench/repair2.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_STRAIGHT){
                    if (!texture.loadFromFile("assets/floor/Conv/yellow/ConvSN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_RIGHT){
                    if (!texture.loadFromFile("assets/floor/Conv/yellow/ConvSE.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_LEFT){
                    if (!texture.loadFromFile("assets/floor/Conv/yellow/ConvSW.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_STRAIGHT_RIGHT){
                    if (!texture.loadFromFile("assets/floor/Conv/yellow/ConvSEN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_SIMPLE_STRAIGHT_LEFT){
                    if (!texture.loadFromFile("assets/floor/Conv/yellow/ConvSWN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_STRAIGHT){
                    if (!texture.loadFromFile("assets/floor/Conv/blue/ConvB_SN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_RIGHT){
                    if (!texture.loadFromFile("assets/floor/Conv/blue/ConvB_SE.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_LEFT){
                    if (!texture.loadFromFile("assets/floor/Conv/blue/ConvB_SW.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_STRAIGHT_RIGHT){
                    if (!texture.loadFromFile("assets/floor/Conv/blue/ConvB_SEN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::CONV_DOUBLE_STRAIGHT_LEFT){
                    if (!texture.loadFromFile("assets/floor/Conv/blue/ConvB_SWN.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::GEAR_RIGHT){
                    if (!texture.loadFromFile("assets/cogwheel/CogGreen.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::GEAR_LEFT){
                    if (!texture.loadFromFile("assets/cogwheel/CogRed.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::PUSHER_EVEN){
                    if (!texture.loadFromFile("assets/pusher/1.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::PUSHER_ODD){
                    if (!texture.loadFromFile("assets/pusher/6.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::LASER_SIMPLE){
                    if (!texture.loadFromFile("assets/laser/LaserStartD.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::LASER_DOUBLE){
                    if (!texture.loadFromFile("assets/laser/LaserDStartD.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::LASER_TRIPLE){
                    if (!texture.loadFromFile("assets/laser/LaserTStartD.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::WELL){
                    if (!texture.loadFromFile("assets/floor/hole.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_1){
                    if (!texture.loadFromFile("assets/startposition/1.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_2){
                    if (!texture.loadFromFile("assets/startposition/2.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_3){
                    if (!texture.loadFromFile("assets/startposition/3.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_4){
                    if (!texture.loadFromFile("assets/startposition/4.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_5){
                    if (!texture.loadFromFile("assets/startposition/5.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_6){
                    if (!texture.loadFromFile("assets/startposition/6.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_7){
                    if (!texture.loadFromFile("assets/startposition/7.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }else if(cell.vec_case[i] == TypeCase::START_DOCK_8){
                    if (!texture.loadFromFile("assets/startposition/8.png")) {
                        std::cerr << "Erreur : Impossible de charger l'image !" << std::endl;
                    }
                }
                sprite.setTexture(texture);
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
