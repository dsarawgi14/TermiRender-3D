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
#include "SphereObj.h"

SphereObj::SphereObj() {
    _radius = 1;
}
float SphereObj::radius() {return _radius;}
void SphereObj::setRadius(float rad) {_radius = rad;}
float SphereObj::sdf(const vec3& p) const {
    return (p - pos()).length() - _radius;
}