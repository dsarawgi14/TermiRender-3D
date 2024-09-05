#include "vecBundle.h"
#include <cmath>

vec2::vec2(float val): x(val), y(val) {}
vec2::vec2(const vec2& other): x(other.x), y(other.y) {}
vec2::vec2(float _x, float _y): x(_x), y(_y) {}

float vec2::length() const {return std::sqrt(x*x + y*y);}
vec2& vec2::normalise() {
    *this /= this -> length();
    return *this;
}

vec2 vec2::operator+(const vec2& other) const {return vec2(x + other.x, y + other.y);}
vec2 vec2::operator-(const vec2& other) const {return vec2(x - other.x, y - other.y);}
vec2 vec2::operator*(const vec2& other) const {return vec2(x * other.x, y * other.y);}
vec2 vec2::operator/(const vec2& other) const {return vec2(x / other.x, y / other.y);}

vec2& vec2::operator+=(const vec2& other) { x += other.x; y += other.y; return *this; }
vec2& vec2::operator-=(const vec2& other) { x -= other.x; y -= other.y; return *this; }
vec2& vec2::operator*=(const vec2& other) { x *= other.x; y *= other.y; return *this; }
vec2& vec2::operator/=(const vec2& other) { x /= other.x; y /= other.y; return *this; }

vec2 vec2::operator+(float val) const {return vec2(x + val, y + val);}
vec2 vec2::operator-(float val) const {return vec2(x - val, y - val);}
vec2 vec2::operator*(float val) const {return vec2(x * val, y * val);}
vec2 vec2::operator/(float val) const {return vec2(x / val, y / val);}

float vec2::dot(const vec2& other) const { return x * other.x + y * other.y; }

vec2 operator+(float val, vec2& v) {return vec2(v.x + val, v.y + val);}
vec2 operator-(float val, vec2& v) {return vec2(v.x - val, v.y - val);}
vec2 operator*(float val, vec2& v) {return vec2(v.x * val, v.y * val);}
vec2 operator/(float val, vec2& v) {return vec2(v.x / val, v.y / val);}


vec3::vec3(float val): x(val), y(val), z(val) {}
vec3::vec3(const vec3& other): x(other.x), y(other.y), z(other.z) {}
vec3::vec3(float _x, float _y, float _z): x(_x), y(_y), z(_z) {}

float vec3::length() const {return std::sqrt(x*x + y*y + z*z);}
vec3& vec3::normalise() {
    *this /= this -> length();
    return *this;
}

vec3 vec3::operator+(const vec3& other) const {return vec3(x + other.x, y + other.y, z + other.z);}
vec3 vec3::operator-(const vec3& other) const {return vec3(x - other.x, y - other.y, z - other.z);}
vec3 vec3::operator*(const vec3& other) const {return vec3(x * other.x, y * other.y, z * other.z);}
vec3 vec3::operator/(const vec3& other) const {return vec3(x / other.x, y / other.y, z / other.z);}

vec3& vec3::operator+=(const vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
vec3& vec3::operator-=(const vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
vec3& vec3::operator*=(const vec3& other) { x *= other.x; y *= other.y; z *= other.z; return *this; }
vec3& vec3::operator/=(const vec3& other) { x /= other.x; y /= other.y; z /= other.z; return *this; }

vec3 vec3::operator+(float val) const {return vec3(x + val, y + val, z + val);}
vec3 vec3::operator-(float val) const {return vec3(x - val, y - val, z - val);}
vec3 vec3::operator*(float val) const {return vec3(x * val, y * val, z * val);}
vec3 vec3::operator/(float val) const {return vec3(x / val, y / val, z / val);}

float vec3::dot(const vec3& other) const { return x * other.x + y * other.y + z * other.z; }
vec3 vec3::cross(const vec3& other) const {
    return vec3(
        y * other.z - z * other.y,
        z * other.x - x * other.z,
        x * other.y - y * other.x
    );
}

vec3 operator+(float val, vec3 v) {return vec3(v.x + val, v.y + val, v.z + val);}
vec3 operator-(float val, vec3 v) {return vec3(v.x - val, v.y - val, v.z - val);}
vec3 operator*(float val, vec3 v) {return vec3(v.x * val, v.y * val, v.z * val);}
vec3 operator/(float val, vec3 v) {return vec3(v.x / val, v.y / val, v.z / val);}

vec4::vec4(float val): x(val), y(val), z(val), w(val) {}
vec4::vec4(const vec4& other): x(other.x), y(other.y), z(other.z), w(other.w) {}
vec4::vec4(const vec3& other, float val): x(other.x), y(other.y), z(other.z), w(val) {}
vec4::vec4(float _x, float _y, float _z, float _w): x(_x), y(_y), z(_z), w(_w) {}

float vec4::length() const {return std::sqrt(x*x + y*y + z*z + w*w);}
vec4& vec4::normalise() {
    *this /= this -> length();
    return *this;
}

vec4 vec4::operator+(const vec4& other) const {return vec4(x + other.x, y + other.y, z + other.z, w + other.w);}
vec4 vec4::operator-(const vec4& other) const {return vec4(x - other.x, y - other.y, z - other.z, w + other.w);}
vec4 vec4::operator*(const vec4& other) const {return vec4(x * other.x, y * other.y, z * other.z, w + other.w);}
vec4 vec4::operator/(const vec4& other) const {return vec4(x / other.x, y / other.y, z / other.z, w + other.w);}

vec4& vec4::operator+=(const vec4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
vec4& vec4::operator-=(const vec4& other) { x -= other.x; y -= other.y; z -= other.z; w += other.w; return *this; }
vec4& vec4::operator*=(const vec4& other) { x *= other.x; y *= other.y; z *= other.z; w += other.w; return *this; }
vec4& vec4::operator/=(const vec4& other) { x /= other.x; y /= other.y; z /= other.z; w += other.w; return *this; }

vec4 vec4::operator+(float val) const {return vec4(x + val, y + val, z + val, w + val);}
vec4 vec4::operator-(float val) const {return vec4(x - val, y - val, z - val, w - val);}
vec4 vec4::operator*(float val) const {return vec4(x * val, y * val, z * val, w * val);}
vec4 vec4::operator/(float val) const {return vec4(x / val, y / val, z / val, w / val);}

float vec4::dot(const vec4& other) const { return x * other.x + y * other.y + z * other.z + w * other.w; }

vec4 operator+(float val, vec4& v) {return vec4(v.x + val, v.y + val, v.z + val, v.w + val);}
vec4 operator-(float val, vec4& v) {return vec4(v.x - val, v.y - val, v.z - val, v.w - val);}
vec4 operator*(float val, vec4& v) {return vec4(v.x * val, v.y * val, v.z * val, v.w * val);}
vec4 operator/(float val, vec4& v) {return vec4(v.x / val, v.y / val, v.z / val, v.w / val);}