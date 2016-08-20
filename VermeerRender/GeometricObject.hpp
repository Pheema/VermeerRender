#pragma once

#include "bounds.hpp"

namespace VermeerRender
{
	class Ray;
	class HitInfo;
	class Material;

    // 幾何オブジェクトのアブストラクトクラス
    class GeometricObject
    {
    public:
        virtual void
        SetMaterial(Material& material)
        {
            m_materialPtr = &material;
        }

		Material&
		GetMaterial()
		{
			return *m_materialPtr;
		}

		const Bounds&
		GetBounds() const
		{
			return bounds;
		}

        virtual bool
		Intersect(const Ray& ray, HitInfo* const hitinfo) = 0;

		virtual void
		CalcBound() = 0;

		virtual Vector3f
		SampleSurface(const HitInfo& hitInfo) = 0;

		virtual float
		SamplingArea(const HitInfo& hitInfo) = 0;

    protected:
		Bounds bounds;
        Material* m_materialPtr = nullptr;

    };
}