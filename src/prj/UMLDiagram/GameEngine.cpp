#include "GameEngine.h"

namespace UMLDiagram {
GameEngine::GameEngine(const std::string& boardPath) : plateau(boardPath) {}

void GameEngine::run(){
    Liste_Joueurs.push_back(Joueur(Tableau_2_bord(),Robot(Orientation::NORD,{0,0}),Archive(1,{0,0})));
    Pile_2_cartes pioche;
    Programme cartes_programme(0, TypeMouvement::AVANCER_1_CASE); //Valeurs ignorées seules celles du fichier JSON seront prises en compte
    cartes_programme.loadCartes("./assets/cards/cards_program.json",pioche);
}

const Plateau& GameEngine::getPlateau() const {
    return plateau;
}

const std::vector<Joueur>& GameEngine::getPlayers() const {
    return Liste_Joueurs;
}

}