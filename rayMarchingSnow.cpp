#include "RayMarcher.h"

using namespace std;

void stepFunc(double interval, double currentTime, ObjectCollection& collection) {
    for(auto s: collection.sphere) {
        s.setPos(s.pos() - )
    }
}

int main() {
    ObjectCollection& collection;
    collection.camera.pos = vec3(-1, 0, 1);
    
    collection.sphere["sphere 1"];
    collection.sphere["sphere 1"].setRadius(0.2);
    collection.sphere["sphere 1"].setPos();

    CubeObj repeatCube;
    repeatCube.setPos(0.5);

    Scene myScene;
    myScene.wrapInCube(&repeatCube);

}