#pragma once

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

#include "vec.h"
#include "vecFunctions.h"
#include "objectClass.h"
#include "renderableObject.h"
#include "ObjectCollection.h"
#include "BringBackCursor.h"
#include "CameraObject.h"

class Scene {
private:
    typedef void (*stepFuncPtr)(double, double, ObjectCollection&);

    const char* gradient = " .:!/r(l1Z4H9W8$@";
    const int gradientSize = strlen(gradient) - 2;
    struct winsize size;
    int MAX_DISTANCE, MAX_STEPS; float SURF_DIST;
    unsigned short int _maxFrameRate;
    bool maintainMaxFrameRate, wrapInCubeBool, NO_THREADING;
    volatile bool internalClockOn, nextFrame;
    double prevIncrementTime, currentTime;
    std::thread internalClock;
    ObjectCollection& collection;
    RenderableObject* objectSDF;
    CubeObj* cube;
    BringBackCursor cursor;
    stepFuncPtr privateStep = [](double, double, ObjectCollection&){}; 
    std::chrono::high_resolution_clock::time_point prevTime, endTime; 

    void fold(vec3 &p);
    float rayMarch(vec3 ro, vec3 rd);
    vec3 getNormal(vec3 p);
    float getIntensity(vec3& l, vec3 ro, vec3 rd);

    void internalTimer();
    void waitNextFrame();

    std::thread RenderThreads[20];
    const int NUM_THREADS = 20;
    char buffs[1000000];
    
    void threadRender();
    void threadRenderer(int i);
    void noThreadRender();

public:
    Scene(RenderableObject* objsdf, ObjectCollection& coll);
    void setMaxFrameRate(unsigned short int rate);
    unsigned short int maxFrameRate();
    void removeMaxFrameRate();
    void clear();
    void wrapInCube(CubeObj* c);
    void removeWrapInCube();
    CubeObj* getWrapingCube();

    void setStepFunction(stepFuncPtr func);
    void step();
    void startFrameTimer();
    void stopFrameTimer();
    void noThreading();
    void enableThreading();
    void render();
};