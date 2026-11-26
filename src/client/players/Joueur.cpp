#include "Joueur.h"
#include <utility>

namespace players {
Joueur::Joueur(Tableau_2_bord t2b, board::Robot robot, board::Archive archive) : tableau_2_bord(std::move(t2b)), robot(robot), archive(archive){
    main.cards.reserve(5);
}

const Tableau_2_bord& Joueur::getT2B() const {
    return tableau_2_bord;
}

Tableau_2_bord& Joueur::getT2B() {
    return tableau_2_bord;
}

const board::Robot& Joueur::getRobot() const {
    return robot;
}

board::Robot& Joueur::getRobot() {
    return robot;
}
}

