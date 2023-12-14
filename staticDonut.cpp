#include "Vec.h"
#include "VecFunctions.h"
// #include "ObjectTypes.h"

#include <iostream>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstring>
#include <cmath>
#include <chrono>
#include <thread>
#include <atomic>
// #include <conio.h>

using namespace std;

#define MAX_DISTANCE 100
#define MAX_STEPS 100
#define SURF_DIST 0.01f

int HEIGHT = 30, WIDTH = 100;

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
        l = b = h = 10;
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
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        angle = M_PI / 2;
        screenWidth = WIDTH;
        screenHeight = HEIGHT;
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
    float distA = donut.sdf(p), distB = sphere.sdf(p), k = 0.5;
    // return fmax(distA, -distB);
    float h = clamp(0.5 + 0.5*(distA-distB)/k, 0., 1.);
    return distA*(1-h)+distB*h - k*h*(1.-h);
    // return distA;
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

vec4 mixBaseColors(vec3& p) {
    float distA = donut.sdf(p), distB = sphere.sdf(p), k = 0.5;
    float h = clamp(0.5 + 0.5*(distA-distB)/k, 0., 1.);
    vec3 c = mix(donut.color, sphere.color, h);
    float d = mix(distA, distB, h) - k*h*(1-h);

    return vec4(c, d);
}

vec4 getColor(vec3& l, vec3 ro, vec3 rd) {
    rd.normalise();
    float dO = rayMarch(ro, rd);
    if(dO >= MAX_DISTANCE) return 0;
    vec3 p = ro + dO * rd;
    vec3 n = getNormal(p);
    vec4 clr = mixBaseColors(p);
    // return clr;
    return vec4(vec3(clr.x, clr.y,clr.z), 1)*0.3 + mix(clr, vec4(0,0,1,1), (fmax(0.0d, (l-p).normalise().dot(n)))) * 0.6;
}

void printColor(vec4 v, int gradientSize, const char* gradient) {
    int r = static_cast<int>(v.x * 255);
    int g = static_cast<int>(v.y * 255);
    int b = static_cast<int>(v.z * 255);

    std::cout <<"\x1b[38;2;" << r << ";" << g << ";" << b << "m" << gradient[(int)(gradientSize * clamp(v.w, 0, 1))];
}

float getIntensity(vec3& l, vec3 ro, vec3 rd) {
    rd.normalise();
    float dO = rayMarch(ro, rd);
    if(dO >= MAX_DISTANCE) return 0;
    vec3 p = ro + dO * rd;
    vec3 n = getNormal(p);
    return clamp(0.1 + 0.6*(fmax(0.0d, (l-p).normalise().dot(n))), 0, 1);
}

void initScreenSize() {
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    WIDTH = size.ws_col - 4; 
    HEIGHT = size.ws_row - 3;
}

int main() {
    // initScreenSize(); // Looks better without it, just make sure the terminal size is >= 30x100
    clear();
    int check = 0;
    vec4 clr(0);
    // printf("\x1b[2J\x1b[H");
    const char* gradient = " .:!/r(l1Z4H9W8$@";
    int gradientSize = strlen(gradient) - 2;
    // int width = 100, height = 30;
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    int width = WIDTH, height = HEIGHT;
    float aspectRatio = 24.0/11.0;
    vec3 l(3), vec(0, 0, 1);
    sphere.radius = 0.7;
    float t = 0.0f, tstep = 0.01f;
    // if(fork() == 0) pausePlay();

    camera.pos = vec3(0,0,5);
    camera.u = vec3(0,0,-1);
    camera.v = vec3(-1,0,0);
    camera.angle = 2 * atan(0.5);

    while(1){
        while(ContinueBool) {
            printf("\x1b[H");
            donut.rotX(0.04 * sin(t));
            donut.rotY(0.02 * cos(t));
            sphere.pos = vec3(2*sin(t*1.2), 0, 0);
            donut.pos = vec3(2*cos(t*1.2), 0, 0);
            printf("%5.4f %5.4f %5.4f\n", donut.u.x, donut.u.y, donut.u.z);
            printf("%5.4f %5.4f %5.4f\n", donut.v.x, donut.v.y, donut.v.z);
            for(int i = 0; i < height; i++) {
                for(int j = 0; j < width; j++) {
                    // vec3 p((j - width/2), -(i - height/2)*aspectRatio, 0);
                    // float intensity = getIntensity(l, vec * 5, p - vec * fmax(height*aspectRatio, width));
                    float intensity = getIntensity(l, camera.pos, camera.rd(j, i));
                    cout << gradient[(int)(gradientSize * intensity)];
                    // clr = getColor(l, vec * 5, p - vec * max(height, width));
                    // printColor(clr, gradientSize, gradient);
                }
                cout << "\n";
                // cout << "\x1b[0m\n" << flush;
            }
            // printf("%5.4f %5.4f %5.4f, %5.4f\n", clr.x, clr.y, clr.z, clr.w);
            std::chrono::duration<double> duration(tstep);
            std::this_thread::sleep_for(duration);
            t += tstep;
            check++;
        }
    }
    return 0;
}