#include "Programme.h"
namespace UMLDiagram {

    Programme::Programme(int vitesse, TypeMouvement mouvement) 
        : Vitesse(vitesse), Mouvement(mouvement) {
    }

    int Programme::getVitesse() {
        return Vitesse;
    }

    TypeMouvement Programme::getMouvement() {
        return Mouvement;
    }

}