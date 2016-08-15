#pragma once

namespace VermeerRender
{
	class Ray;
	class HitInfo;
	class Material;

    // 幾何オブジェクトのアブストラクトクラス
    class GeometricObject
    {
    public:
        void
        SetMaterial(Material& material)
        {
            m_materialPtr = &material;
        }

		Material&
		GetMaterial()
		{
			return *m_materialPtr;
		}

        virtual bool
		Intersect(const Ray& ray, HitInfo* const hitinfo) = 0;

		virtual Vector3f
		SampleSurface() = 0;

		virtual float
		Area() = 0;

    protected:
        Material* m_materialPtr = nullptr;
    };
}