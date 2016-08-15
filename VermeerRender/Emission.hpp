#pragma once

#include "hitInfo.hpp"
#include "material.hpp"
#include "vector3f.hpp"
#include "xorShift128.hpp"

namespace VermeerRender
{
	class Emission : public Material
	{
	public:
		Emission(const Color3f& color) : m_emissionColor(color) {};

		virtual Color3f
		Radiance(Ray* const rayPtr, const HitInfo& hitInfo)
		{
			return m_emissionColor;
		}

		virtual Color3f
		Brdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			// 光源のBrdf()は読まれない前提
			std::cerr << "Error: EmissionのBrdf()が呼ばれました。" << std::endl;
			abort();
			return Color3f::One();
		}

		virtual float
		Pdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			// 光源のPdf()は読まれない前提
			std::cerr << "Error: EmissionのPdf()が呼ばれました。" << std::endl;
			abort();
			return 1.0f;
		}
	private:
		Color3f m_emissionColor;
	};

}