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
#include "CubeObj.h"
#include "SphereObj.h"
#include "DonutObj.h"
#include "CameraObject.h"

class ObjectCollection {    
public:
    std::map<std::string, CubeObj> cube;
    std::map<std::string, SphereObj> sphere;
    std::map<std::string, DonutObj> donut;
    CameraObject camera;
    vec3 lightSource;
    ObjectCollection(): lightSource(0) {}
};