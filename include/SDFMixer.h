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

class SDFMixer : public RenderableObject {
private:
    RenderableObject* _left;
    RenderableObject* _right;
    float k;
    float (SDFMixer::*sdfFunc) (const vec3& p) const;
    float intersectSDF(const vec3& p) const;
    float unionSDF(const vec3& p) const;
    float differenceSDF(const vec3& p) const;
    float smoothIntersectSDF(const vec3& p) const;
    float smoothUnionSDF(const vec3& p) const;
    float smoothDifferenceSDF(const vec3& p) const;
public:
    SDFMixer(RenderableObject* l, RenderableObject* r);
    void setIntersect();
    void setUnion();
    void setDifference();
    void setSmoothIntersect(float _k = 1);
    void setSmoothUnion(float _k = 1);
    void setSmoothDifference(float _k = 1);

    float sdf(const vec3& p) const override;
};