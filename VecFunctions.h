#pragma once
#include <cmath>
#include "Vec.h"

float clamp(float value, float min, float max) { return fmax(fmin(value, max), min); }
double sign(double a) { return (0 < a) - (a < 0); }

vec3 norm(const vec3& v) {return vec3(v / v.length());}

vec3 rotateX(const vec3& a, double angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return vec3(
        a.x,
        cosA * a.y - sinA * a.z,
        sinA * a.y + cosA * a.z
    );
}
vec3 rotateY(const vec3& a, double angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return vec3(
        sinA * a.z + cosA * a.x,
        a.y,
        cosA * a.z - sinA * a.x
    );
}
vec3 rotateZ(const vec3& a, double angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return vec3(
        sinA * a.x + cosA * a.y,
        cosA * a.x - sinA * a.y,
        a.z
    );
}

vec2 rotate(const vec2& a, double angle) {
    float cosA = std::cos(angle);
    float sinA = std::sin(angle);
    return vec2(
        sinA * a.x + cosA * a.y,
        cosA * a.x - sinA * a.y
    );
}