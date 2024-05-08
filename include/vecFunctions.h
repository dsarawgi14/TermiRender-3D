#pragma once
#include <cmath>
#include "vec.h"

float clamp(float value, float min, float max);
double sign(double a);

vec3 norm(const vec3& v);
vec3 max(const vec3& v, const vec3& u);
vec3 min(const vec3& v, const vec3& u);

template <typename T>
T mix(const T &a,const T &b, float h);
extern template float mix<float>(const float&, const float&, float);

vec3 rotateX(const vec3& a, double angle);
vec3 rotateY(const vec3& a, double angle);
vec3 rotateZ(const vec3& a, double angle);

vec2 rotate(const vec2& a, double angle);