#pragma once

#include "hitInfo.hpp"
#include "ray.hpp"
#include "texture2D.hpp"
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
        Radiance(Ray* const rayPtr, const HitInfo& hitInfo) const = 0;

		virtual Color3f
		Brdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) const = 0;

		virtual float
		Pdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) const = 0;
    };
}