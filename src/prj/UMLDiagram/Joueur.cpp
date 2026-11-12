#include "Joueur.h"
namespace UMLDiagram {
Joueur::Joueur(Tableau_2_bord& t2b, Robot robot, Archive archive) : tableau_2_bord(t2b), robot(robot), archive(archive){
    main.cards.reserve(5);
}
}

