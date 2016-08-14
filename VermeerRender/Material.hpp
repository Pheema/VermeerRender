#pragma once

#include "HitInfo.hpp"
#include "Ray.hpp"
#include "Vector3f.hpp"

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