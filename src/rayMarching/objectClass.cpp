#include "objectClass.h"
#include "vec.h"
#include "vecFunctions.h"
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

vec3 ObjectClass::pos() const {return _pos;}
vec3 ObjectClass::u() const {return _u;}
vec3 ObjectClass::v() const {return _v;}
void ObjectClass::setPos(vec3 p) {_pos = p;}
void ObjectClass::setU(vec3 p) {_u = p.normalise();}
void ObjectClass::setV(vec3 p) {_v = p.normalise();}
void ObjectClass::rotX(float angle) {_u = rotateX(_u, angle); _v = rotateX(_v, angle);}
void ObjectClass::rotY(float angle) {_u = rotateY(_u, angle); _v = rotateY(_v, angle);}
void ObjectClass::rotZ(float angle) {_u = rotateZ(_u, angle); _v = rotateZ(_v, angle);}
ObjectClass::ObjectClass(): _pos(0), _u(1, 0, 0), _v({0, 1, 0}) {}
