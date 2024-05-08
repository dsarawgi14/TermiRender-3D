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
#include "Scene.h"

void Scene::fold(vec3 &p) {
    p = -floor(((p - cube -> pos()).dot(cube -> u()) + cube -> l()/2)/cube -> l())*cube -> l() *cube -> u() + p;
    p = -floor(((p - cube -> pos()).dot(cube -> v()) + cube -> b()/2)/cube -> b())*cube -> b() *cube -> v() + p;
    vec3 w = (cube -> u()).cross(cube -> v());
    p = -floor(((p - cube -> pos()).dot(w) + cube -> h()/2)/cube -> h())*cube -> h()*w + p;
}
float Scene::rayMarch(vec3 ro, vec3 rd) {
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
vec3 Scene::getNormal(vec3 p) {
    float e = 0.01f, d = objectSDF -> sdf(p);
    vec3 n(
        d - objectSDF -> sdf({p.x - e, p.y, p.z}),
        d - objectSDF -> sdf({p.x, p.y - e, p.z}),
        d - objectSDF -> sdf({p.x, p.y, p.z - e})
    );
    n.normalise();
    return n;
}
float Scene::getIntensity(vec3& l, vec3 ro, vec3 rd) {
    rd.normalise();
    float dO = rayMarch(ro, rd);
    if(dO >= MAX_DISTANCE) return 0;
    vec3 p = ro + dO * rd;
    vec3 n = getNormal(p);
    return clamp(0.1 + 0.6*(fmax(0.0, (l-p).normalise().dot(n))), 0, 1);
}

void Scene::internalTimer() {
    while(internalClockOn) {
        std::chrono::duration<double> duration(1.0 / _maxFrameRate);
        std::this_thread::sleep_for(duration);
        nextFrame = 1;
    }
}

void Scene::waitNextFrame() {
    while(!nextFrame) {}
    nextFrame = false;
}

void Scene::threadRender() {
    for(int i = 0; i < NUM_THREADS; i++) {
        RenderThreads[i] = std::thread(&Scene::threadRenderer, this, i);
    }
    for(int i = 0; i < NUM_THREADS; i++) {
        RenderThreads[i].join();
    }
    std::cout << buffs;
}

void Scene::threadRenderer(int i) {
    int i1 = static_cast<int>((int)collection.camera.screenHeight() / NUM_THREADS * i + i);
    int i2 = static_cast<int>(std::min((int)collection.camera.screenHeight(), (int)collection.camera.screenHeight()/NUM_THREADS*(i + 1) + i + 1));
    int offset = static_cast<int>((int)(collection.camera.screenWidth()+5) * i1);
    int cnt = 0;
    for(int k = i1; k < i2; k++) {
        sprintf(buffs + offset + cnt, "  ");
        cnt += 2;
        for(int j = 0; j < collection.camera.screenWidth(); j++) {
            float intensity = getIntensity(collection.lightSource, collection.camera.pos(), collection.camera.rd(j, k));
            sprintf(buffs + offset + cnt, "%c", gradient[(int)(gradientSize * intensity)]);
            cnt++;
        }
        sprintf(buffs + offset + cnt, "  \n");
        cnt += 3;
    }
    if(i2 != (int)collection.camera.screenHeight()) buffs[offset + cnt] = ' ';
    return;
}

void Scene::noThreadRender() {
    for(int i = 0; i < collection.camera.screenHeight(); i++) {
        std::cout << "  ";
        for(int j = 0; j < collection.camera.screenWidth(); j++) {
            float intensity = getIntensity(collection.lightSource, collection.camera.pos(), collection.camera.rd(j, i));
            std::cout << gradient[(int)(gradientSize * intensity)];
        }
        std::cout << "  \n";
    }
}

Scene::Scene(RenderableObject* objsdf, ObjectCollection& coll): objectSDF(objsdf), collection(coll) {
    maintainMaxFrameRate = false;
    internalClockOn = false;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    prevIncrementTime = 0.01;
    MAX_DISTANCE = 100;
    MAX_STEPS = 100;
    SURF_DIST = 0.01f;
    prevTime = std::chrono::high_resolution_clock::now();
    cursor = BringBackCursor();
}
void Scene::setMaxFrameRate(unsigned short int rate) {
    _maxFrameRate = rate; 
    maintainMaxFrameRate = true;
}
unsigned short int Scene::maxFrameRate() {
    return _maxFrameRate;
}
void Scene::removeMaxFrameRate() {
    maintainMaxFrameRate = false;
    _maxFrameRate = 0;
}
void Scene::clear() {
    if(fork() == 0) {
        execlp("clear" ,"clear", NULL);
    }
    wait(nullptr);
}
void Scene::wrapInCube(CubeObj* c) {
    wrapInCubeBool = true;
    cube = c;
}
void Scene::removeWrapInCube() {
    wrapInCubeBool = false;
    cube = nullptr;
}
CubeObj* Scene::getWrapingCube() {return cube;}

void Scene::setStepFunction(stepFuncPtr func) {
    privateStep = func;
}

void Scene::step() {
    this->privateStep(prevIncrementTime, currentTime, collection);
}

void Scene::startFrameTimer() {
    prevTime = std::chrono::high_resolution_clock::now();
    if(maintainMaxFrameRate && !internalClockOn) {
        internalClockOn = true;
        internalClock = std::thread(&Scene::internalTimer, this);
    }
}

void Scene::stopFrameTimer() {
    internalClockOn = false;
    internalClock.join();
}

void Scene::noThreading() {
    NO_THREADING = 1;
}

void Scene::enableThreading() {
    NO_THREADING = 0;
}

void Scene::render() {
    printf("\x1b[H\e[?25l");
    if(maintainMaxFrameRate) waitNextFrame();
    step();
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size); //58x240 screen size looks really good, with a steady 70 fps, without maxFrameRate has20 fps even on 110x480
    collection.camera.setScreenHeight(size.ws_row - 2);
    collection.camera.setScreenWidth(size.ws_col - 4);
    printf("Frame rate: %4d%*s\n", (int)(1/prevIncrementTime), size.ws_col - 16, "");
    if(NO_THREADING) noThreadRender();
    else threadRender();
    endTime = std::chrono::high_resolution_clock::now();
    printf("%*s", size.ws_col, "");
    std::cout << "\e[?25h";
    std::chrono::duration<double> duration = endTime - prevTime;
    prevTime = endTime;
    prevIncrementTime = duration.count();
    currentTime += prevIncrementTime;
}