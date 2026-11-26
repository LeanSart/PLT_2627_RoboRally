#include "Carte.h"

namespace players {
    void Carte::flip() {
        if (status == Face::HIDDEN) {status = Face::VISIBLE;} else {status = Face::HIDDEN;}
    }

    Face Carte::getFace(){return status;}
}