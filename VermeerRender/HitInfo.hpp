#pragma once

#include "ray.hpp"
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
		Ray ray;				// 入射したレイ
        float length = INFINITY;
        GeometricObject* hitObjPtr = nullptr;
    };
}