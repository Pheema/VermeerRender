#pragma once

#include "hitInfo.hpp"
#include "ray.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
    // class Ray;
    //class HitInfo;
    // マテリアルのアブストラクトクラス
    class Material
    {
    public:
        // 色と反射ベクトルを返す
        virtual Color3f
        Radiance(Ray* const rayPtr, const HitInfo& hitInfo) = 0;
    };
}