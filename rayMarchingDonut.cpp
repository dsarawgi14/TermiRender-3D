#include "Vec.h"
#include "VecFunctions.h"
#include "RayMarcher.h"

using namespace std;

void stepFunc(double increment, double currectTime, ObjectCollection& collection) {
    collection.cube["Donut 1"].rotX(4 * sin(currectTime) * increment);
    collection.cube["Donut 1"].rotY(2 * cos(currectTime) * increment);
    collection.cube["Donut 1"].setPos(vec3(-2*sin(currectTime*1.2), 0, 0));
    collection.sphere["Sphere 1"].setPos(vec3(2*sin(currectTime*1.2), 0, 0));
}

int main() {
    ObjectCollection collection;
    collection.camera.setPos({0,0,5});
    collection.camera.setU({0, 0, -1});
    collection.camera.setV({-1, 0, 0});
    collection.camera.setAngle(2 * atan(0.5));
    collection.lightSource = vec3(3, 3, 3);

    collection.sphere["Sphere 1"];
    // collection.sphere["Sphere 1"].setRadius(0.7);
    collection.cube["Donut 1"];
    // collection.donut["Donut 2"];
    // collection.donut["Donut 2"].setMajorRadius(0.5);
    // collection.donut["Donut 2"].setMinorRadius(0.1);
    
    SDFMixer SphereNDonut(&collection.cube["Donut 1"], &collection.sphere["Sphere 1"]);
    SphereNDonut.setSmoothDifference();

    // SDFMixer FullMix()

    Scene scene(&SphereNDonut, collection);
    scene.setStepFunction(stepFunc);
    scene.setMaxFrameRate(100);
    scene.clear();
    scene.startFrameTimer();
    while(1) {
        scene.render();
    }
    return 0;
}