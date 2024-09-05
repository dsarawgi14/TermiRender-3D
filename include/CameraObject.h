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

class CameraObject : public ObjectClass {
private:
    float _angle, _screenWidth, _screenHeight, _aspectRatio;

public:
    CameraObject();
    float angle();
    float screenWidth();
    float screenHeight();
    float aspectRatio();
    void setAngle(float angle);
    void setScreenWidth(float screenWidth);
    void setScreenHeight(float screenHeight);
    void setAspectRatio(float aspectRatio);

    vec3 rd(int x, int y);
};