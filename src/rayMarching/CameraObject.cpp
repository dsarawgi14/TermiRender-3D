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
#include "CameraObject.h"

CameraObject::CameraObject() {
    _angle = M_PI / 2;
    _screenWidth = 100;
    _screenHeight = 30;
    _aspectRatio = 24.0f/ 11.0f;
}
float CameraObject::angle() {return _angle;}
float CameraObject::screenWidth() {return _screenWidth;}
float CameraObject::screenHeight() {return _screenHeight;}
float CameraObject::aspectRatio() {return _aspectRatio;}
void CameraObject::setAngle(float angle) {_angle = angle;}    
void CameraObject::setScreenWidth(float screenWidth) {_screenWidth = screenWidth;}
void CameraObject::setScreenHeight(float screenHeight) {_screenHeight = screenHeight;} 
void CameraObject::setAspectRatio(float aspectRatio) {_aspectRatio = aspectRatio;}

vec3 CameraObject::rd(int x, int y) {
    float depth = fmax(_screenHeight * _aspectRatio, _screenWidth)/2 / tan(_angle/2);
    return depth * u() - (x - _screenWidth/2) * v() - (y - _screenHeight/2) * u().cross(v()) * _aspectRatio;
}