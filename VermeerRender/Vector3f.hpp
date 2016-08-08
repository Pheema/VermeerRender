#pragma once

#include <iostream>

namespace VermeerRender
{
    class Vector3f
    {
    public:
        float x, y, z;
        Vector3f() {}
        explicit Vector3f(float a) : x(a), y(a), z(a) {}
        Vector3f(float x, float y, float z) : x(x), y(y), z(z) {}

        // Vector3f + Vector3f
        inline Vector3f
        operator+(Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
        
        // Vector3f - Vector3f
        inline Vector3f
        operator-(Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }

        // Vector3f * Vector3f (要素毎)
        inline Vector3f
        operator*(Vector3f& v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }

        // Vector3f / Vector3f (要素毎)
        inline Vector3f
        operator/(Vector3f& v) const { return Vector3f(x / v.x, y / v.y, z / v.z); }

        // Vector3f * number
        template <typename T>
        inline Vector3f
        operator*(T a) const { return Vector3f(x * a, y * a, z * a); }

        // number * Vector3f
        template<typename T>
        friend inline
        Vector3f operator*(T a, Vector3f& v);

    private:
        
    };

    // number * Vector3f
    template<typename T>
    Vector3f
    operator*(T a, Vector3f& v) { return v * a };

    // 内積
    float
    Dot(const Vector3f& v1, const Vector3f& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

    // 外積
    Vector3f
    Cross(const Vector3f& v1, const Vector3f& v2)
    {
        Vector3f v;
        v.x = v1.y * v2.z - v1.z * v2.y;
        v.y = v1.z * v2.x - v1.x * v2.z;
        v.z = v1.x * v2.y - v1.y * v2.x;
        return v;
    }

    std::ostream&
    operator<<(std::ostream& os, const Vector3f& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
}
