#pragma once

#include "Vector3f.hpp"

namespace VermeerRender
{
    enum class RayTypes
    {
        CAMERA,
        SHADOW,
        DIFFUSE,
        TRANSLUCENT,
        GLOSSY
    };

    class Ray
    {
    public:
        Vector3f o;				// Ray origin
        Vector3f dir;			// Ray direction

        float ior;				// レイが飛んでいるマテリアルの屈折率
        unsigned bounce;	// レイが反射した回数

        Ray() {};
        Ray(const Vector3f& o, const Vector3f& d, RayTypes rayType, float ior = 1.0f, unsigned bounce = 0)
            : o(o), dir(dir.Normalized()), ior(ior), bounce(bounce) {};
    };
}