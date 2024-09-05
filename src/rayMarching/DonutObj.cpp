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
#include "DonutObj.h"

DonutObj::DonutObj() {
    _majorRadius = 1;
    _minorRadius = 0.5;
}
float DonutObj::majorRadius() {return _majorRadius;}
float DonutObj::minorRadius() {return _minorRadius;}
void DonutObj::setMajorRadius(float rad) {_majorRadius = rad;}
void DonutObj::setMinorRadius(float rad) {_minorRadius = rad;}
float DonutObj::sdf(const vec3& p) const {
    vec3 rp = vec3((p-pos()).dot(u()), (p-pos()).dot(v()), (p-pos()).dot(u().cross(v()))); 
    return vec2(vec2(rp.x, rp.y).length() - _majorRadius, rp.z).length() - _minorRadius;
}