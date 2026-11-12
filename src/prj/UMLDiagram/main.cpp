#include "GameEngine.h"
#include "RenderEngine.h"
using namespace UMLDiagram;

int main(void){
    GameEngine engine("assets/boards/CellTest.json");
    RenderEngine render;
    engine.run();
    render.run(engine);
    return 0;
}