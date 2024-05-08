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

class DonutObj : public ObjectClass, public RenderableObject {
private:
    float _majorRadius, _minorRadius;
public:
    DonutObj();
    float majorRadius();
    float minorRadius();
    void setMajorRadius(float rad);
    void setMinorRadius(float rad);
    float sdf(const vec3& p) const;
};