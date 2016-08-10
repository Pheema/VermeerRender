#pragma once

#include "HitInfo.hpp"
#include "Material.hpp"
#include "Ray.hpp"

namespace VermeerRender
{
    // 幾何オブジェクトのアブストラクトクラス
    class GeometricObject
    {
    public:
        void
        SetMaterial(Material& material)
        {
            m_materialPtr = &material;
        }

        virtual bool Intersect(const Ray& ray, HitInfo* const hitinfo) = 0;

    protected:
        Material* m_materialPtr = nullptr;
    };
}