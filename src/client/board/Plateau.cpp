#include "Plateau.h"

#include <SFML/Graphics.hpp>
#include "../../../extern/jsoncpp-1.8.0/json/json.h"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace board {
Plateau::Plateau(): Hauteur(1), Largeur(1){
        Grille.resize(Hauteur, std::vector<Case_Plateau>(Largeur));
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
    Json::Value donneesJson;
    try {
        file >> donneesJson;
    } catch (const Json::Exception& e) {
        throw std::runtime_error("Erreur de syntaxe JSON : " + std::string(e.what()));
    }
     
    if (donneesJson.isMember("width")) {
        Largeur = donneesJson["width"].asInt();
    } else {
        throw std::invalid_argument("Le champ 'width' est introuvable.");
    } 
    
    if (donneesJson.isMember("height")) {
        Hauteur = donneesJson["height"].asInt();
    } else {
        throw std::invalid_argument("Le champ 'height' est introuvable.");
    }
    std::string grid_json;
    if (donneesJson.isMember("grid")) {
        grid_json = donneesJson["grid"].asString();
    } else {
        throw std::invalid_argument("Le champ 'grid' est introuvable.");
    }

    std::vector<std::string> cell_list;
    std::stringstream ss(grid_json);
    std::string cell;
    
    while (ss >> cell) { //Parsing des cases plateau en une liste
        cell_list.push_back(cell);
    }

    Grille.clear();
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
}
