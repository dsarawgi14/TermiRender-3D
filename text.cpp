#include "Vec.h"
#include "VecFunctions.h"

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>
#include <atomic>

using namespace std;

#define MAX_DISTANCE 500
#define MAX_STEPS 100
#define SURF_DIST 0.01f

atomic<bool> ContinueBool(true);

struct DonutObjSpace{
    vec3 pos;
    vec3 u;
    vec3 v;
    vec3 color;

    DonutObjSpace(): pos(vec3(0)), u(vec3(1, 0, 0)), v(vec3(0, 1, 0)), color(vec3(0, 1, 0)) {}

    void rotX(float angle) {u = rotateX(u, angle); v = rotateX(v, angle);}
    void rotY(float angle) {u = rotateY(u, angle); v = rotateY(v, angle);}
    void rotZ(float angle) {u = rotateZ(u, angle); v = rotateZ(v, angle);}

    float sdf(const vec3& p) {
        vec3 rp = vec3((p-(pos)).dot(u), (p-(pos)).dot(v), (p-(pos)).dot(u.cross(v))); 
        return vec2(vec2(rp.x, rp.y).length() - 1, rp.z).length() - 0.5;
    }
} donut;

struct BoxObjSpace{
    vec3 pos;
    vec3 u;
    vec3 v;
    float l, b, h;

    BoxObjSpace(): pos(vec3(0)), u(vec3(1, 0, 0)), v(vec3(0, 1, 0)) {
        l = b = h = 50;
    }

    void rotX(float angle) {u = rotateX(u, angle); v = rotateX(v, angle);}
    void rotY(float angle) {u = rotateY(u, angle); v = rotateY(v, angle);}
    void rotZ(float angle) {u = rotateZ(u, angle); v = rotateZ(v, angle);}

} cube;

struct CameraObjSpace{
    vec3 pos;
    vec3 u;
    vec3 v;
    float angle;
    float screenWidth;
    float screenHeight;
    const float aspectRatio = 24.0f/ 11.0f;

    CameraObjSpace(): pos(vec3(0)), u(vec3(1, 0, 0)), v(vec3(0, 1, 0)) {
        angle = M_PI / 2;
        screenWidth = 100;
        screenHeight = 30;
    }

    void rotX(float angle) {u = rotateX(u, angle); v = rotateX(v, angle);}
    void rotY(float angle) {u = rotateY(u, angle); v = rotateY(v, angle);}
    void rotZ(float angle) {u = rotateZ(u, angle); v = rotateZ(v, angle);}

    vec3 rd(int x, int y) {
        float depth = fmax(screenHeight * aspectRatio, screenWidth)/2 / tan(angle/2);
        return depth * u - (x - screenWidth/2) * v - (y - screenHeight/2) * u.cross(v) * aspectRatio;
    }
} camera;

struct SphereObjSpace{
    vec3 pos;
    float radius;
    vec3 color;

    SphereObjSpace(): pos(vec3(0)), radius(1.0f), color(vec3(1, 0, 0)) {}

    float sdf(const vec3& p) {
        return (p - pos).length() - radius;
    }
} sphere;

float getDist(const vec3& p) {
    float distA = donut.sdf(p);
    return distA;
}

float rayMarch(vec3 ro, vec3 rd) {
    float dO = 0, dS;
    for(int i = 0; i < MAX_STEPS; i++) {
        vec3 p = ro + rd * dO;  
        dS = getDist(p);
        dO += dS;
        if(dS < SURF_DIST || dO > MAX_DISTANCE) break;
    }
    return dO;
}

void fold(vec3 &p) {
    p = -floor(((p - cube.pos).dot(cube.u) + cube.l/2)/cube.l)*cube.l*cube.u + p;
    p = -floor(((p - cube.pos).dot(cube.v) + cube.b/2)/cube.b)*cube.b*cube.v + p;
    vec3 w = cube.u.cross(cube.v);
    p = -floor(((p - cube.pos).dot(w) + cube.h/2)/cube.h)*cube.h*w + p;
}

float rayMarchInfinite(vec3 ro, vec3 rd) {
    float dO = 0, dS;
    for(int i = 0; i < MAX_STEPS; i++) {
        vec3 p = ro + rd * dO;
        fold(p);
        dS = getDist(p);
        dO += dS;
        if(dS < SURF_DIST || dO > MAX_DISTANCE) break;
    }
    return dO;
}

vec3 getNormal(vec3 p) {
    float e = 0.01f, d = getDist(p);
    vec3 n(
        d - getDist({p.x - e, p.y, p.z}),
        d - getDist({p.x, p.y - e, p.z}),
        d - getDist({p.x, p.y, p.z - e})
    );
    n.normalise();
    return n;
}

void clear() {
    if(fork() == 0) {
        execlp("clear" ,"clear", NULL);
    }
    wait(nullptr);
}

template <typename T>
T mix(const T &a,const T &b, float h) {
    return a*(1-h) + b*h;
}


float getIntensity(vec3& l, vec3 ro, vec3 rd) {
    rd.normalise();
    float dO = rayMarchInfinite(ro, rd);
    if(dO >= MAX_DISTANCE) return 0;
    vec3 p = ro + dO * rd;
    vec3 n = getNormal(p);
    return clamp(pow(1/(p - ro).length(), 2) + 3 * (fmax(0.0d, (l-p).normalise().dot(n)))/(pow((l-p).length(), 2)), 0, 1);
}

int main() {
    clear();
    const char* gradient = " .:!/r(l1Z4H9W8$@";
    int gradientSize = strlen(gradient) - 2;
    int width = 100, height = 30;
    float aspectRatio = 24.0/11.0;
    vec3 l(3), vec(0, 0, 1);
    sphere.radius = 0.7;
    sphere.pos = vec3(0, 0, 10000);
    float t = 0.0f, tstep = 0.01f;

    camera.pos = vec3(0,0,10);
    camera.u = vec3(0,0,-1);
    camera.v = vec3(-1,0,0);
    // camera.angle = 2 * atan(0.5);

    while(1){
        while(ContinueBool) {
            printf("\x1b[H");
            for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                    vec3 p((j - width/2), -(i - height/2)*aspectRatio, 0);
                    float intensity = getIntensity(l, camera.pos, camera.rd(j, i));
                    cout << gradient[(int)(gradientSize * intensity)];
                }
                cout << "\n";
            }
            std::chrono::duration<double> duration(tstep);
            std::this_thread::sleep_for(duration);
            t += tstep;
        }
    }
    return 0;
}