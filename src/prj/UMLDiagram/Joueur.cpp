#include "Joueur.h"
#include <utility>

namespace UMLDiagram {
Joueur::Joueur(Tableau_2_bord t2b, Robot robot, Archive archive) : tableau_2_bord(std::move(t2b)), robot(robot), archive(archive){
    main.cards.reserve(5);
}

const Tableau_2_bord& Joueur::getT2B() const {
    return tableau_2_bord;
}

Tableau_2_bord& Joueur::getT2B() {
    return tableau_2_bord;
}
}

