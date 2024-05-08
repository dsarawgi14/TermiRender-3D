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
#include "SDFMixer.h"

float SDFMixer::intersectSDF(const vec3& p) const {
    return fmax(_left -> sdf(p), _right -> sdf(p));
}
float SDFMixer::unionSDF(const vec3& p) const {
    return fmin(_left -> sdf(p), _right -> sdf(p));
}
float SDFMixer::differenceSDF(const vec3& p) const {
    return fmax(_left -> sdf(p), -_right -> sdf(p));
}
float SDFMixer::smoothIntersectSDF(const vec3& p) const {
    float distA = _left -> sdf(p), distB = _right -> sdf(p);
    float h = clamp(0.5 - 0.5*(distA-distB)/k, 0., 1.);
    return mix(distA, distB, h) + k*h*(1.-h);
}
float SDFMixer::smoothUnionSDF(const vec3& p) const {
    float distA = _left -> sdf(p), distB = _right -> sdf(p);
    float h = clamp(0.5 + 0.5*(distA-distB)/k, 0., 1.);
    return mix(distA, distB, h) - k*h*(1.-h); 
}
float SDFMixer::smoothDifferenceSDF(const vec3& p) const {
    float distA = _left -> sdf(p), distB = _right -> sdf(p);
    float h = clamp(0.5 - 0.5*(distB+distA)/k, 0., 1.);
    return mix(distA, -distB, h ) + k*h*(1.-h); 
}

SDFMixer::SDFMixer(RenderableObject* l, RenderableObject* r): _left(l), _right(r), sdfFunc(&SDFMixer::intersectSDF) {}
void SDFMixer::setIntersect() {sdfFunc = &SDFMixer::intersectSDF;}
void SDFMixer::setUnion() {sdfFunc = &SDFMixer::unionSDF;} 
void SDFMixer::setDifference() {sdfFunc = &SDFMixer::differenceSDF;}
void SDFMixer::setSmoothIntersect(float _k) {
    sdfFunc = &SDFMixer::smoothIntersectSDF; 
    k = _k;
}
void SDFMixer::setSmoothUnion(float _k) {
    sdfFunc = &SDFMixer::smoothUnionSDF; 
    k = _k;
} 
void SDFMixer::setSmoothDifference(float _k) {
    sdfFunc = &SDFMixer::smoothDifferenceSDF; 
    k = _k;
}

float SDFMixer::sdf(const vec3& p) const {
    return (this->*sdfFunc)(p);
}