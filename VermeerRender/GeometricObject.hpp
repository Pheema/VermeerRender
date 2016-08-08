#pragma once

#include "HitInfo.hpp"
#include "Ray.hpp"

namespace VermeerRender
{
    // 幾何オブジェクト用のアブストラクトクラス
    class GeometricObject
    {
        virtual bool
        Intersect(const Ray& ray, HitInfo* const hitinfo) = 0;
    };
}