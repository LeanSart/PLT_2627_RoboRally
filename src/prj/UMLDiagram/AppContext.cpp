#include "AppContext.h"

namespace UMLDiagram {
    void AppContext::goTo (ScreenId id){
        requested = id;
    }
}