#pragma once
#include "vec.h"
#include "vecFunctions.h"

class RenderableObject{
public:
    virtual float sdf(const vec3& p) const = 0;
};