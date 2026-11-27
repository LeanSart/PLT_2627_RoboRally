#include "Plateau.h"

#include <SFML/Graphics.hpp>
#include "../../../extern/jsoncpp-1.8.0/json/json.h"
#include "file/file.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace board {
Plateau::Plateau(): Hauteur(1), Largeur(1){
        Grille.resize(Hauteur, std::vector<Case_Plateau>(Largeur));
    }

Case_Plateau Plateau::getCase (Position position){return Grille.at(position.PosY).at(position.PosX);}

/**
 * Charge un plateau depuis un fichier JSON.
 * 
 * @param filePath Chemin vers le fichier JSON
 * @return void ou erreur
 */
void Plateau::loadPlateau(file::path BoardPath, Orientation orientation, file::path docking_bayPath){
    std::ifstream Boardfile(BoardPath);
    std::ifstream Docking_bayfile(docking_bayPath);
    if (!Boardfile.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + BoardPath.string());
    }
    if (!Docking_bayfile.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier : " + docking_bayPath.string());
    }
    Json::Value BoardDataJson;
    Json::Value Docking_bayDataJson;
    try {
        Boardfile >> BoardDataJson;
        Docking_bayfile >> Docking_bayDataJson;
    } catch (const Json::Exception& e) {
        throw std::runtime_error("Erreur de syntaxe JSON : " + std::string(e.what()));
    }
     
    if (BoardDataJson.isMember("width") && Docking_bayDataJson.isMember("width") && BoardDataJson["width"].asInt() == Docking_bayDataJson["width"].asInt()) {
        Largeur = BoardDataJson["width"].asInt();
    } else {
        throw std::invalid_argument("Le champ 'width' est introuvable ou invalide.");
    } 
    
    if (BoardDataJson.isMember("height") && Docking_bayDataJson.isMember("height")) {
        Hauteur = BoardDataJson["height"].asInt() + Docking_bayDataJson["height"].asInt();
    } else {
        throw std::invalid_argument("Le champ 'height' est introuvable.");
    }
    std::string grid_json;
    if (BoardDataJson.isMember("grid") && Docking_bayDataJson.isMember("grid")) {
        grid_json = set_Board_Orientation(BoardDataJson["grid"].asString(), orientation, Largeur, BoardDataJson["height"].asInt()) + " " + Docking_bayDataJson["grid"].asString();
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

std::string Plateau::set_Board_Orientation(std::string Board_grid,Orientation orientation, int Width, int Height){
    std::vector<std::string> cell_list;
    std::stringstream ss(Board_grid);
    std::string cell;
    
    while (ss >> cell) { //Parsing des cases plateau en une liste
        cell_list.push_back(cell);
    }

    if((int)cell_list.size() != Width*Height){
        throw std::invalid_argument(" 'grid' ne correspond pas à la taille du plateau");
    }

    std::string output_grid;
    switch (orientation)
    {
    case EST:
        for (int c = 0; c < Width; ++c) {
                for (int r = Height - 1; r >= 0; --r) {
                    if (!output_grid.empty()) output_grid += " ";
                    output_grid += cell_list[r * Width + c];
                }
        }
        return output_grid;
    case SUD:
        for (int i = cell_list.size() - 1; i >= 0; --i) {
                if (!output_grid.empty()) output_grid += " ";
                output_grid += cell_list[i];
        }
        return output_grid;
    case OUEST: 
        for (int c = Width - 1; c >= 0; --c) {
                for (int r = 0; r < Height; ++r) {
                    if (!output_grid.empty()) output_grid += " ";
                    output_grid += cell_list[r * Width + c];
                }
        }
        return output_grid;
    default:
        return Board_grid;
    }
}
}
