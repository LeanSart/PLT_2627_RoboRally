#include "Pion.h"
namespace board {
Pion::Pion(Position position) : position(position) {}

void Pion::set_position (Position position){
    this->position = position;
}

Position Pion::get_position () const {
    return this->position;
}
}
