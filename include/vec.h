#pragma once
#include <cmath>

class vec2{
public:
    float x, y;
    vec2(float val);
    vec2(const vec2& other);
    vec2(float _x, float _y);

    float length() const;
    vec2& normalise();

    vec2 operator+(const vec2& other) const;
    vec2 operator-(const vec2& other) const;
    vec2 operator*(const vec2& other) const;
    vec2 operator/(const vec2& other) const;

    vec2& operator+=(const vec2& other);
    vec2& operator-=(const vec2& other);
    vec2& operator*=(const vec2& other);
    vec2& operator/=(const vec2& other);

    vec2 operator+(float val) const;
    vec2 operator-(float val) const;
    vec2 operator*(float val) const;
    vec2 operator/(float val) const;

    float dot(const vec2& other) const;
};

vec2 operator+(float val, vec2& v);
vec2 operator-(float val, vec2& v);
vec2 operator*(float val, vec2& v);
vec2 operator/(float val, vec2& v);

class vec3{
public:
    float x, y, z;
    vec3(float val);
    vec3(const vec3& other);
    vec3(float _x, float _y, float _z);
    
    float length() const;
    vec3& normalise();

    vec3 operator+(const vec3& other) const;
    vec3 operator-(const vec3& other) const;
    vec3 operator*(const vec3& other) const;
    vec3 operator/(const vec3& other) const;

    vec3& operator+=(const vec3& other);
    vec3& operator-=(const vec3& other);
    vec3& operator*=(const vec3& other);
    vec3& operator/=(const vec3& other);

    vec3 operator+(float val) const;
    vec3 operator-(float val) const;
    vec3 operator*(float val) const;
    vec3 operator/(float val) const;

    float dot(const vec3& other) const;
    vec3 cross(const vec3& other) const;
};

vec3 operator+(float val, vec3 v);
vec3 operator-(float val, vec3 v);
vec3 operator*(float val, vec3 v);
vec3 operator/(float val, vec3 v);

class vec4{
public:
    float x, y, z, w;
    vec4(float val);
    vec4(const vec4& other);
    vec4(const vec3& other, float val);
    vec4(float _x, float _y, float _z, float _w);
    
    float length() const;
    vec4& normalise();

    vec4 operator+(const vec4& other) const;
    vec4 operator-(const vec4& other) const;
    vec4 operator*(const vec4& other) const;
    vec4 operator/(const vec4& other) const;

    vec4& operator+=(const vec4& other);
    vec4& operator-=(const vec4& other);
    vec4& operator*=(const vec4& other);
    vec4& operator/=(const vec4& other);

    vec4 operator+(float val) const;
    vec4 operator-(float val) const;
    vec4 operator*(float val) const;
    vec4 operator/(float val) const;

    float dot(const vec4& other) const;
};

vec4 operator+(float val, vec4& v);
vec4 operator-(float val, vec4& v);
vec4 operator*(float val, vec4& v);
vec4 operator/(float val, vec4& v);

/*  Optimisations:

 - Use __const__ correctly  = Specifying operator+(float) const {}
        - Mark functions that do not modify the object as const. 
        - This helps the compiler make optimizations and signals to users that the function does not modify the object.

 - To avoid implicit type conversions, consider using __explicit__ for single-argument constructors.
        - explicit vec2(float val) : x(val), y(val) {}
        - explicit vec3(float val) : x(val), y(val), z(val) {}
        = Basically can't have a function like fun(vec2 v) and call it like fun(2.0, 1.0), but why wouldn't I want it here.

 - Remember to profile your code to ensure that these optimizations have a positive impact on performance. 
 - The effectiveness of optimizations can depend on the specific use cases and requirements of your application.

*/