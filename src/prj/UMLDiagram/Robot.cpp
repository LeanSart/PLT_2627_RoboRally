#include "Robot.h"
namespace UMLDiagram{
Robot::Robot(Orientation orientation, Position position) : Pion(position), orientation(orientation) {};

Orientation Robot::getOrientation() const{
    return this->orientation;
}

}