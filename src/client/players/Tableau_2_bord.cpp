#include "Tableau_2_bord.h"

namespace players{
    Tableau_2_bord::Tableau_2_bord() = default;
    int Tableau_2_bord::getPoint_2_vie() const{
        return this->Point_2_vie;
    }
    int Tableau_2_bord::getPoint_2_degat() const{
        return this->Point_2_degat;
    }
    bool Tableau_2_bord::getHors_tension() const{
        return this->Hors_tension;
    }
    void Tableau_2_bord::remove1Point_2_vie(){
        if(this->Point_2_vie == 0){
            return;    
        }
        this->Point_2_vie = this->Point_2_vie - 1;
        return;
    }
    void Tableau_2_bord::setPoint_2_degat(int newPoint_2_degat){
        this->Point_2_degat = newPoint_2_degat;
        return;
    }
    void Tableau_2_bord::flipHors_tension(){
        if (this->Hors_tension == false) {this->Hors_tension = true;} else {this->Hors_tension = false;}
        return;
    }
}