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

class SphereObj : public ObjectClass, public RenderableObject {
private:
    float _radius;
public:
    SphereObj();
    float radius();
    void setRadius(float rad);
    float sdf(const vec3& p) const;
};