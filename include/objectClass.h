#pragma once

#include "vec.h"
#include "vecFunctions.h"
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
    vec3 pos() const;
    vec3 u() const;
    vec3 v() const;
    void setPos(vec3 p);
    void setU(vec3 p);
    void setV(vec3 p);
    void rotX(float angle);
    void rotY(float angle);
    void rotZ(float angle);
    ObjectClass();
};
