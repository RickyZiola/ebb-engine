#ifndef EBB_VECTOR_HPP
#define EBB_VECTOR_HPP

#include <math.h>

namespace Ebb {
namespace Math {

template<typename T>
class vec2 {
public:
    T x,y;

    vec2() :         x(0), y(0) {}
    vec2(T x) :      x(x), y(x) {}
    vec2(T x, T y) : x(x), y(y) {}

    vec2<T> operator+(const vec2<T>& v) const { return vec2<T>(x+v.x, y+v.y); }
    vec2<T> operator-(const vec2<T>& v) const { return vec2<T>(x-v.x, y-v.y); }
    vec2<T> operator*(const vec2<T>& v) const { return vec2<T>(x*v.x, y*v.y); }
    vec2<T> operator/(const vec2<T>& v) const { return vec2<T>(x/v.x, y/v.y); }

    T dot (const vec2<T>& v) const { return x*v.x + y*v.y; }
    T length() const { return std::sqrt(x*x + y*y); }

private:

};

template<typename T>
class vec3 {
public:
    T x,y,z;

    vec3() :              x(0), y(0), z(0) {}
    vec3(T x) :           x(x), y(x), z(x) {}
    vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    vec3<T> operator+(const vec3<T>& v) const { return vec3<T>(x+v.x, y+v.y, z+v.z); }
    vec3<T> operator-(const vec3<T>& v) const { return vec3<T>(x-v.x, y-v.y, z-v.z); }
    vec3<T> operator*(const vec3<T>& v) const { return vec3<T>(x*v.x, y*v.y, z*v.z); }
    vec3<T> operator/(const vec3<T>& v) const { return vec3<T>(x/v.x, y/v.y, z/v.z); }

    T dot (const vec3<T>& v) const { return x*v.x + y*v.y + z*v.z; }
    T length() const { return std::sqrt(x*x + y*y + z*z); }

    vec3<T> normalize() const { return *this / vec3<T>(this->length()); }

    vec3<T> cross(const vec3<T>& v) const {
        return vec3<T>(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

private:

};

template<typename T>
class vec4 {
public:
    T x,y,z,w;

    vec4() :                   x(0), y(0), z(0), w(0) {}
    vec4(T x) :                x(x), y(x), z(x), w(x) {}
    vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    vec4<T> operator+(const vec4<T>& v) const { return vec4<T>(x+v.x, y+v.y, z+v.z, w+v.w); }
    vec4<T> operator-(const vec4<T>& v) const { return vec4<T>(x-v.x, y-v.y, z-v.z, w-v.w); }
    vec4<T> operator*(const vec4<T>& v) const { return vec4<T>(x*v.x, y*v.y, z*v.z, w*v.w); }
    vec4<T> operator/(const vec4<T>& v) const { return vec4<T>(x/v.x, y/v.y, z/v.z, w/v.w); }

    T dot (const vec4<T>& v) const { return x*v.x + y*v.y + z*v.z + w*v.w; }
    T length() const { return std::sqrt(x*x + y*y + z*z + w*w); }

private:

};

typedef vec2<float>   float2;
typedef vec2<double> double2;
typedef vec2<int>       int2;

typedef vec3<float>   float3;
typedef vec3<double> double3;
typedef vec3<int>       int3;

typedef vec4<float>   float4;
typedef vec4<double> double4;
typedef vec4<int>       int4;

}; };

#endif // EBB_VECTOR_HPP