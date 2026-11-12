#include "GameEngine.h"

namespace UMLDiagram {
GameEngine::GameEngine(const std::string& boardPath) : plateau(boardPath) {}

void GameEngine::run(){
    Tableau_2_bord t2bJ1 = Tableau_2_bord();
    Joueur J1 = Joueur(t2bJ1,Robot(Orientation::NORD,{0,0}),Archive(1,{0,0}));
    Pile_2_cartes pioche;
    Programme cartes_programme(0, TypeMouvement::AVANCER_1_CASE); //Valeurs ignorées seules celles du fichier JSON seront prises en compte
    cartes_programme.loadCartes("./assets/cards/cards_program.json",pioche);
}

const Plateau& GameEngine::getPlateau() const {
    return plateau;
}
}