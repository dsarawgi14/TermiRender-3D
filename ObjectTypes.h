#pragma once
#include "VecFunctions.h"
#include <iostream>
#include <string>
#include <list>

class RenderablObj {
public:
    virtual float sdf() = 0;
    virtual void rotateX(float angle) = 0;
    virtual void rotateY(float angle) = 0;
    virtual void rotateZ(float angle) = 0;
    virtual void translate(const vec3& v) = 0;
};

class Window{
private:
    const char* gradient;
public:
    Window() {
        // sprintf(gradient, " .:!/r(l1Z4H9W8$@");
        // gradientSize = 16;
        gradient = " .:!/r(l1Z4H9W8$@";
    }
    // Window(int a): Window() {
    // }
};

class Scene{
    std::list<RenderablObj*> objs;
    Scene(): objs() {}
    void insertObj(RenderablObj& newObj) {
        objs.emplace_back(&newObj);
    }
    float trace(const vec3& ro, vec3& rd) const {
        float t = 0.0f;
        const float maxDistance = 100.0f;
        const int maxSteps = 100;
        vec3 point(ro);
        for(int i = 0; i < maxSteps && t < maxDistance; i++) {

        }
        return 0;
    }

};

class Plane: RenderablObj {};
class Sphere: RenderablObj {
    vec3 center;
    float radius;
    Sphere(const vec3& _center, float _radius): center(_center), radius(_radius) {}
    Sphere(): Sphere(vec3(0,0,0), 1.0) {}
    Sphere(const Sphere& other): Sphere(other.center, other.radius) {}
    float sdf(const vec3& p) {
        return (p-center).length() - radius;
    }
    void rotateX(float angle) {}
    void rotateY(float angle) {}
    void rotateZ(float angle) {}
    void translate(const vec3& v) {
        center += v;
    }
};
class Donut: RenderablObj {};
class Cube: RenderablObj {};