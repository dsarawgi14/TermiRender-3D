#pragma once

#include "Vec.h"
#include "VecFunctions.h"
#include <map>
#include <cmath>
#include <chrono>
#include <thread>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <type_traits>
#include <sys/ioctl.h>

class ObjectClass {
private:
    vec3 _pos, _u, _v;
public:
    vec3 pos() const {return _pos;}
    vec3 u() const {return _u;}
    vec3 v() const {return _v;}
    void setPos(vec3 p) {_pos = p;}
    void setU(vec3 p) {_u = p.normalise();}
    void setV(vec3 p) {_v = p.normalise();}
    void rotX(float angle) {_u = rotateX(_u, angle); _v = rotateX(_v, angle);}
    void rotY(float angle) {_u = rotateY(_u, angle); _v = rotateY(_v, angle);}
    void rotZ(float angle) {_u = rotateZ(_u, angle); _v = rotateZ(_v, angle);}
    ObjectClass(): _pos(0), _u(1, 0, 0), _v({0, 1, 0}) {}
};

class RenderableObject{
public:
    virtual float sdf(const vec3& p) const = 0;
};

class SphereObj : public ObjectClass, public RenderableObject {
private:
    float _radius;
public:
    SphereObj() {
        _radius = 1;
    }
    float radius() {return _radius;}
    void setRadius(float rad) {_radius = rad;}
    float sdf(const vec3& p) const {
        return (p - pos()).length() - _radius;
    }
};

class DonutObj : public ObjectClass, public RenderableObject {
private:
    float _majorRadius, _minorRadius;
public:
    DonutObj() {
        _majorRadius = 1;
        _minorRadius = 0.5;
    }
    float majorRadius() {return _majorRadius;}
    float minorRadius() {return _minorRadius;}
    void setMajorRadius(float rad) {_majorRadius = rad;}
    void setMinorRadius(float rad) {_minorRadius = rad;}
    float sdf(const vec3& p) const {
        vec3 rp = vec3((p-pos()).dot(u()), (p-pos()).dot(v()), (p-pos()).dot(u().cross(v()))); 
        return vec2(vec2(rp.x, rp.y).length() - _majorRadius, rp.z).length() - _minorRadius;
    }
};

class CubeObj : public ObjectClass, public RenderableObject {
private:
    float _l, _b, _h;
public:
    CubeObj() {
        _l = _b = _h = 1;
    }
    float l() {return _l;} 
    float b() {return _b;}
    float h() {return _h;}
    void setL(float l) {_l = l;}    
    void setB(float b) {_b = b;}
    void setH(float h) {_h = h;}
    float sdf(const vec3& p) const {
        vec3 q = vec3(fabs((p-pos()).dot(u())), fabs((p-pos()).dot(v())), fabs((p-pos()).dot(u().cross(v())))) - vec3(_l, _b, _h); 
        return max(q, vec3(0)).length() + fmin(fmax(fmax(q.x, q.y), q.z), 0);
    }
};

class CameraObject : public ObjectClass {
private:
    float _angle, _screenWidth, _screenHeight, _aspectRatio;

public:
    CameraObject() {
        _angle = M_PI / 2;
        _screenWidth = 100;
        _screenHeight = 30;
        _aspectRatio = 24.0f/ 11.0f;
    }
    float angle() {return _angle;}
    float screenWidth() {return _screenWidth;}
    float screenHeight() {return _screenHeight;}
    float aspectRatio() {return _aspectRatio;}
    void setAngle(float angle) {_angle = angle;}    
    void setScreenWidth(float screenWidth) {_screenWidth = screenWidth;}
    void setScreenHeight(float screenHeight) {_screenHeight = screenHeight;} 
    void setAspectRatio(float aspectRatio) {_aspectRatio = aspectRatio;}

    vec3 rd(int x, int y) {
        float depth = fmax(_screenHeight * _aspectRatio, _screenWidth)/2 / tan(_angle/2);
        return depth * u() - (x - _screenWidth/2) * v() - (y - _screenHeight/2) * u().cross(v()) * _aspectRatio;
    }
};

class SDFMixer : public RenderableObject {
private:
    RenderableObject* _left;
    RenderableObject* _right;
    float k;
    float (SDFMixer::*sdfFunc) (const vec3& p) const;
    float intersectSDF(const vec3& p) const {
        return fmax(_left -> sdf(p), _right -> sdf(p));
    }
    float unionSDF(const vec3& p) const {
        return fmin(_left -> sdf(p), _right -> sdf(p));
    }
    float differenceSDF(const vec3& p) const {
        return fmax(_left -> sdf(p), -_right -> sdf(p));
    }
    float smoothIntersectSDF(const vec3& p) const {
        float distA = _left -> sdf(p), distB = _right -> sdf(p);
        float h = clamp(0.5 - 0.5*(distA-distB)/k, 0., 1.);
        return mix(distA, distB, h) + k*h*(1.-h);
    }
    float smoothUnionSDF(const vec3& p) const {
        float distA = _left -> sdf(p), distB = _right -> sdf(p);
        float h = clamp(0.5 + 0.5*(distA-distB)/k, 0., 1.);
        return mix(distA, distB, h) - k*h*(1.-h); 
    }
    float smoothDifferenceSDF(const vec3& p) const {
        float distA = _left -> sdf(p), distB = _right -> sdf(p);
        float h = clamp(0.5 - 0.5*(distB+distA)/k, 0., 1.);
        return mix(distA, -distB, h ) + k*h*(1.-h); 
    }
public:
    SDFMixer(RenderableObject* l, RenderableObject* r): _left(l), _right(r), sdfFunc(&SDFMixer::intersectSDF) {}
    void setIntersect() {sdfFunc = &SDFMixer::intersectSDF;}
    void setUnion() {sdfFunc = &SDFMixer::unionSDF;} 
    void setDifference() {sdfFunc = &SDFMixer::differenceSDF;}
    void setSmoothIntersect(float _k = 1) {
        sdfFunc = &SDFMixer::smoothIntersectSDF; 
        k = _k;
    }
    void setSmoothUnion(float _k = 1) {
        sdfFunc = &SDFMixer::smoothUnionSDF; 
        k = _k;
    } 
    void setSmoothDifference(float _k = 1) {
        sdfFunc = &SDFMixer::smoothDifferenceSDF; 
        k = _k;
    }

    float sdf(const vec3& p) const override {
        return (this->*sdfFunc)(p);
    }
};

class ObjectCollection {    
public:
    std::map<std::string, CubeObj> cube;
    std::map<std::string, SphereObj> sphere;
    std::map<std::string, DonutObj> donut;
    CameraObject camera;
    vec3 lightSource;
    ObjectCollection(): lightSource(0) {}
};

class Scene {
private:
    typedef void (*stepFuncPtr)(double, double, ObjectCollection&);

    const char* gradient = " .:!/r(l1Z4H9W8$@";
    const int gradientSize = strlen(gradient) - 2;
    struct winsize size;
    int MAX_DISTANCE, MAX_STEPS; float SURF_DIST;
    unsigned short int _maxFrameRate;
    bool maintainMaxFrameRate, wrapInCubeBool;
    volatile bool internalClockOn, nextFrame;
    double prevIncrementTime, currentTime;
    std::thread internalClock;
    ObjectCollection& collection;
    RenderableObject* objectSDF;
    CubeObj* cube;
    stepFuncPtr privateStep = [](double, double, ObjectCollection&){}; 
    std::chrono::high_resolution_clock::time_point startTime, endTime; 

    void fold(vec3 &p) {
        p = -floor(((p - cube -> pos()).dot(cube -> u()) + cube -> l()/2)/cube -> l())*cube -> l() *cube -> u() + p;
        p = -floor(((p - cube -> pos()).dot(cube -> v()) + cube -> b()/2)/cube -> b())*cube -> b() *cube -> v() + p;
        vec3 w = (cube -> u()).cross(cube -> v());
        p = -floor(((p - cube -> pos()).dot(w) + cube -> h()/2)/cube -> h())*cube -> h()*w + p;
    }
    float rayMarch(vec3 ro, vec3 rd) {
        float dO = 0, dS;
        for(int i = 0; i < MAX_STEPS; i++) {
            vec3 p = ro + rd * dO; 
            if(wrapInCubeBool) fold(p); 
            dS = objectSDF -> sdf(p);
            dO += dS;
            if(dS < SURF_DIST || dO > MAX_DISTANCE) break;
        }
        return dO;
    }
    vec3 getNormal(vec3 p) {
        float e = 0.01f, d = objectSDF -> sdf(p);
        vec3 n(
            d - objectSDF -> sdf({p.x - e, p.y, p.z}),
            d - objectSDF -> sdf({p.x, p.y - e, p.z}),
            d - objectSDF -> sdf({p.x, p.y, p.z - e})
        );
        n.normalise();
        return n;
    }
    float getIntensity(vec3& l, vec3 ro, vec3 rd) {
        rd.normalise();
        float dO = rayMarch(ro, rd);
        if(dO >= MAX_DISTANCE) return 0;
        vec3 p = ro + dO * rd;
        vec3 n = getNormal(p);
        return clamp(0.1 + 0.6*(fmax(0.0d, (l-p).normalise().dot(n))), 0, 1);
    }

    void internalTimer() {
        while(internalClockOn) {
            std::chrono::duration<double> duration(1.0 / _maxFrameRate);
            std::this_thread::sleep_for(duration);
            nextFrame = 1;
        }
    }

    void waitNextFrame() {
        while(!nextFrame) {}
        nextFrame = false;
    }

public:
    Scene(RenderableObject* objsdf, ObjectCollection& coll): objectSDF(objsdf), collection(coll) {
        maintainMaxFrameRate = false;
        internalClockOn = false;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        prevIncrementTime = 0.01;
        MAX_DISTANCE = 100;
        MAX_STEPS = 100;
        SURF_DIST = 0.01f;
    }
    void setMaxFrameRate(unsigned short int rate) {
        _maxFrameRate = rate; 
        maintainMaxFrameRate = true;
    }
    unsigned short int maxFrameRate() {
        return _maxFrameRate;
    }
    void removeMaxFrameRate() {
        maintainMaxFrameRate = false;
        _maxFrameRate = 0;
    }
    void clear() {
        if(fork() == 0) {
            execlp("clear" ,"clear", NULL);
        }
        wait(nullptr);
    }
    void wrapInCube(CubeObj* c) {
        wrapInCubeBool = true;
        cube = c;
    }
    void removeWrapInCube() {
        wrapInCubeBool = false;
        cube = nullptr;
    }
    CubeObj* getWrapingCube() {return cube;}

    void setStepFunction(stepFuncPtr func) {
        privateStep = func;
    }

    void step() {
        this->privateStep(prevIncrementTime, currentTime, collection);
    }

    void startFrameTimer() {
        if(maintainMaxFrameRate && !internalClockOn) {
            internalClockOn = true;
            internalClock = std::thread(&Scene::internalTimer, this);
        }
    }
    
    void stopFrameTimer() {
        internalClockOn = false;
        internalClock.join();
    }

    void render() {
        startTime = std::chrono::high_resolution_clock::now();
        printf("\x1b[H");
        printf("Frame rate: %4d\n", (int)(1/prevIncrementTime));
        if(maintainMaxFrameRate) waitNextFrame();
        collection.camera.setScreenHeight(size.ws_row - 3);
        collection.camera.setScreenWidth(size.ws_col - 4);
        step();
        for(int i = 0; i < collection.camera.screenHeight(); i++) {
            std::cout << "  ";
            for(int j = 0; j < collection.camera.screenWidth(); j++) {
                float intensity = getIntensity(collection.lightSource, collection.camera.pos(), collection.camera.rd(j, i));
                std::cout << gradient[(int)(gradientSize * intensity)];
            }
            std::cout << "\n";
        }
        std::cout << std::flush;
        endTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = endTime - startTime;
        prevIncrementTime = duration.count();
        currentTime += prevIncrementTime;
    }
};