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

        // 正規化されたベクトルを返す
        Vector3f
        Normalized() const
        {
            float norm = sqrt(x * x + y * y + z * z);
            return Vector3f(x / norm, y / norm, z / norm);
        }

        // ベクトルを正規化する
        void Normalize()
        {
            float norm = sqrt(x * x + y * y + z * z);
            x /= norm; y /= norm; z /= norm;
        }

        #pragma region Addition

        // Vector3f + Vector3f
        inline Vector3f
        operator+(const Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
        
        // Vector3f += Vector3f
        inline Vector3f&
        operator+=(const Vector3f& v)
        {
            x += v.x; y += v.y; z += v.z;
            return *this;
        }

        #pragma endregion

        #pragma region Substracion
        
        // Vector3f - Vector3f
        inline Vector3f
        operator-(Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }

        // Vector3f -= Vector3f
        inline Vector3f&
        operator-=(const Vector3f& v)
        {
            x -= v.x; y -= v.y; z -= v.z;
            return *this;
        }

        // -Vector3f
        inline Vector3f
        operator-() const { return Vector3f(-x, -y, -z); }
        #pragma endregion
        
        #pragma region Multiplication

        // Vector3f * Vector3f (要素毎)
        inline Vector3f
        operator*(const Vector3f& v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }

        // Vector3f *= Vector3f
        inline Vector3f&
        operator*=(const Vector3f& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
        
        // Vector3f * number
        template <typename T>
        inline Vector3f
        operator*(const T a) const { return Vector3f(x * a, y * a, z * a); }

        // number * Vector3f
        template<typename T>
        friend inline
        Vector3f operator*(const T a, Vector3f& v);

        #pragma  endregion

        #pragma region Division

        // Vector3f / Vector3f (要素毎)
        inline Vector3f
        operator/(const Vector3f& v) const { return Vector3f(x / v.x, y / v.y, z / v.z); }

        // Vector3f /= Vector3f
        inline Vector3f&
        operator/=(const Vector3f& v)
        {
            x /= v.x; y /= v.y; z /= v.z;
            return *this;
        }

        #pragma endregion
        
        static inline Vector3f
        Zero() { return Vector3f(0, 0, 0); }

        static inline Vector3f
        Right() { return Vector3f(1.0f, 0.0f, 0.0f); }

        static inline Vector3f
        Up() { return Vector3f(0.0f, 1.0f, 0.0f); }

        static inline Vector3f
        Forward() { return Vector3f(0.0f, 0.0f, -1.0f); }

    private:
        
    };

    // number * Vector3f
    template<typename T>
    static inline Vector3f
    operator*(T a, Vector3f& v) { return v * a };

    // 内積
    static inline float
    Dot(const Vector3f& v1, const Vector3f& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

    // 外積
    static inline Vector3f
    Cross(const Vector3f& v1, const Vector3f& v2)
    {
        Vector3f v;
        v.x = v1.y * v2.z - v1.z * v2.y;
        v.y = v1.z * v2.x - v1.x * v2.z;
        v.z = v1.x * v2.y - v1.y * v2.x;
        return v;
    }

    static inline std::ostream&
    operator<<(std::ostream& os, const Vector3f& v)
    {
        os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
        return os;
    }
}
