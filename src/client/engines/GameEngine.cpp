#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include "file/file.h"
#include "../../../extern/jsoncpp-1.8.0/json/json.h"

namespace engines {
GameEngine::GameEngine() = default;

void GameEngine::startGame(){
    plateau.loadPlateau(file::path("assets/boards/" + plateau_name + ".json"), orientation_plateau, file::path("assets/boards/" + docking_bay_name + ".json"));
 
    Liste_Joueurs.clear();
    for(int i = 0; i < nb_joueurs; ++i){
        Liste_Joueurs.push_back(players::Joueur(players::Tableau_2_bord(),board::Robot(board::Orientation::NORD,{5,14}),board::Archive(i + 1,{0,0})));
    }
 
    players::Programme cartes_programme(0, players::TypeMouvement::AVANCER_1_CASE); //Valeurs ignorées seules celles du fichier JSON seront prises en compte
    cartes_programme.loadCartes("./assets/cards/cards_program.json",Pioche_Programme);
}

bool GameEngine::save(file::path savePath) {
    try {

        Json::Value j;
        j["nb_joueurs"] = nb_joueurs;
        j["docking_bay"] = docking_bay_name;
        j["plateau"] = plateau_name;
        j["orientation_plateau"] = static_cast<int>(orientation_plateau);
        // ajouter ici les autres valeurs à sauvegarder

        std::ofstream out(savePath.string());
        if (!out) {
            std::cerr << "[Erreur] Impossible d'ecrire la sauvegarde : " << savePath.string() << std::endl;
            return false;
        }
        Json::StreamWriterBuilder builder;
        builder["indentation"] = "    ";
        out << Json::writeString(builder, j) << '\n';
        return static_cast<bool>(out);
    } catch (const std::exception& e) {
        std::cerr << "[Erreur] Sauvegarde impossible : " << e.what() << std::endl;
        return false;
    }
}

bool GameEngine::load(file::path savePath) {
    std::ifstream file(savePath.string());
    try {
        Json::Value save_data;
        file >> save_data; // lève Json::Exception (dérive de std::exception) si invalide

        if (!save_data.isMember("nb_joueurs") || !save_data.isMember("plateau") || !save_data.isMember("docking_bay") || !save_data.isMember("orientation_plateau"))
            throw std::runtime_error("Sauvegarde corrompue");

        nb_joueurs = save_data["nb_joueurs"].asInt();
        docking_bay_name = save_data["docking_bay"].asString();
        plateau_name = save_data["plateau"].asString();
        orientation_plateau = (board::Orientation)save_data["orientation_plateau"].asInt();
        // ajouter ici les autres valeurs à charger
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[Erreur] Sauvegarde illisible (" << savePath.string() << ") : " << e.what() << std::endl;
        return false;
    }
}

const board::Plateau& GameEngine::getPlateau() const {
    return plateau;
}

const std::vector<players::Joueur>& GameEngine::getPlayers() const {
    return Liste_Joueurs;
}
}