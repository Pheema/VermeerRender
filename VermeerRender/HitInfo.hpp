#pragma once

#include "vector3f.hpp"
#include "geometricObject.hpp"

namespace VermeerRender
{
    class HitInfo
    {
    public:
        HitInfo() {};

        Vector3f point;
        Vector3f normal;
        float length = INFINITY;
        GeometricObject* hitObjPtr = nullptr;
    };
}