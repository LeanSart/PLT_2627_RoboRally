#include "Tableau_2_Bord.h"
#include <stdexcept>

Tableau_2_Bord::Tableau_2_Bord() 
    : Point_2_vie(3), Point_2_degat(0), Hors_tension(false) {}

int Tableau_2_Bord::getPoint_2_vie() {
    return Point_2_vie;
}

int Tableau_2_Bord::getPoint_2_degat() {
    return Point_2_degat;
}

std::vector<Programme> Tableau_2_Bord::getvecteur_programme() {
    return Vecteur_programme;
}

bool Tableau_2_Bord::gethors_tension() {
    return Hors_tension;
}

void Tableau_2_Bord::setpoint_2_vie(int point_2_vie) {
    Point_2_vie = point_2_vie;
}

void Tableau_2_Bord::setpoint_2_degat(int point_2_degat) {
    Point_2_degat = point_2_degat;
}

void Tableau_2_Bord::setvecteur_programme(Programme carte_programme) {
    if (Vecteur_programme.size() > 5) {
        throw std::overflow_error("Le vecteur de programme contient déjà 5 cartes.");
    }
    Vecteur_programme.push_back(carte_programme);
}

void Tableau_2_Bord::vider_vecteur_programme() {
    Vecteur_programme.clear();
}

void Tableau_2_Bord::sethors_tension(bool hors_tension) {
    Hors_tension = hors_tension;
}