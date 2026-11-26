#include "GameEngine.h"
#include <iostream>
#include <fstream>
#include <string>
#include <fstream>
#include <filesystem>
#include "../../../extern/jsoncpp-1.8.0/json/json.h"

namespace fs = std::filesystem;

namespace UMLDiagram {
GameEngine::GameEngine() = default;

void GameEngine::startGame(){
    plateau.loadPlateau("assets/boards/" + plateau_name + ".json");
 
    Liste_Joueurs.clear();
    for(int i = 0; i < nb_joueurs; ++i){
        Liste_Joueurs.push_back(Joueur(Tableau_2_bord(),Robot(Orientation::NORD,{0,0}),Archive(i + 1,{0,0})));
    }
 
    Programme cartes_programme(0, TypeMouvement::AVANCER_1_CASE); //Valeurs ignorées seules celles du fichier JSON seront prises en compte
    cartes_programme.loadCartes("./assets/cards/cards_program.json",Pioche_Programme);
}

bool GameEngine::save(const std::string& savePath) {
    try {
        const std::filesystem::path p(savePath);

        Json::Value j;
        j["nb_joueurs"] = nb_joueurs;
        j["plateau"] = plateau_name;
        // ajouter ici les autres valeurs à sauvegarder

        std::ofstream out(p);
        if (!out) {
            std::cerr << "[Erreur] Impossible d'ecrire la sauvegarde : " << savePath << std::endl;
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

bool GameEngine::load(const std::string& savePath) {
    std::ifstream file(savePath);
    try {
        Json::Value save_data;
        file >> save_data; // lève Json::Exception (dérive de std::exception) si invalide

        if (!save_data.isMember("nb_joueurs") || !save_data.isMember("plateau"))
            throw std::runtime_error("champ 'nb_joueurs' ou 'plateau' manquant");

        nb_joueurs = save_data["nb_joueurs"].asInt();
        plateau_name = save_data["plateau"].asString();
        // ajouter ici les autres valeurs à charger
        return true;
    } catch (const std::exception& e) {
        std::cerr << "[Erreur] Sauvegarde illisible (" << savePath << ") : " << e.what() << std::endl;
        return false;
    }
}

const Plateau& GameEngine::getPlateau() const {
    return plateau;
}

const std::vector<Joueur>& GameEngine::getPlayers() const {
    return Liste_Joueurs;
}
}