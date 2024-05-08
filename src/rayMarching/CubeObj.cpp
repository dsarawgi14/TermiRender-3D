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
#include "CubeObj.h"

CubeObj::CubeObj(float val) {
    _l = _b = _h = val;
}
float CubeObj::l() {return _l;} 
float CubeObj::b() {return _b;}
float CubeObj::h() {return _h;}
void CubeObj::setL(float l) {_l = l;}    
void CubeObj::setB(float b) {_b = b;}
void CubeObj::setH(float h) {_h = h;}
float CubeObj::sdf(const vec3& p) const {
    vec3 q = vec3(fabs((p-pos()).dot(u())), fabs((p-pos()).dot(v())), fabs((p-pos()).dot(u().cross(v())))) - vec3(_l/2, _b/2, _h/2); 
    return max(q, vec3(0)).length() + fmin(fmax(fmax(q.x, q.y), q.z), 0);
}