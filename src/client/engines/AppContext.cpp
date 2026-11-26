#include "AppContext.h"

namespace engines {
    void AppContext::goTo (screens::ScreenId id){
        requested = id;
    }
}