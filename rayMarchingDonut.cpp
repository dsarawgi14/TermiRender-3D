#include "Vec.h"
#include "VecFunctions.h"
#include "RayMarcher.h"

using namespace std;

void stepFunc(double increment, double currectTime, ObjectCollection& collection) {
    collection.donut["Donut 1"].rotX(4 * sin(currectTime) * increment);
    collection.donut["Donut 1"].rotY(2 * cos(currectTime) * increment);
    collection.donut["Donut 1"].setPos(vec3(-2*sin(currectTime*1.2), 0, 0));
    collection.sphere["Sphere 1"].setPos(vec3(2*sin(currectTime*1.2), 0, 0));
}

int main() {
    ObjectCollection collection;
    collection.camera.setPos({0,0,5});
    collection.camera.setU({0, 0, -1});
    collection.camera.setV({-1, 0, 0});
    collection.lightSource = vec3(3, 3, 3);

    collection.sphere["Sphere 1"];
    collection.sphere["Sphere 1"].setRadius(0.7);
    collection.donut["Donut 1"];
    collection.donut["Donut 2"];

    SDFMixer SphereNDonut(&collection.donut["Donut 1"], &collection.sphere["Sphere 1"]);
    SphereNDonut.setSmoothUnion();
    SDFMixer SphereNDonut2(&collection.donut["Donut 2"], &SphereNDonut);
    SphereNDonut2.setSmoothUnion();

    Scene scene(&SphereNDonut, collection);
    scene.setStepFunction(stepFunc);
    // scene.setMaxFrameRate(100);
    scene.clear();
    scene.startFrameTimer();
    while(1) {
        scene.render();
    }
    return 0;
}