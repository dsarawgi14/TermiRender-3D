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

class CubeObj : public ObjectClass, public RenderableObject {
private:
    float _l, _b, _h;
public:
    CubeObj(float val = 1);
    float l();
    float b();
    float h();
    void setL(float l);
    void setB(float b);
    void setH(float h);
    float sdf(const vec3& p) const;
};