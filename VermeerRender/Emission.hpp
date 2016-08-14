#pragma once

#include "HitInfo.hpp"
#include "Material.hpp"
#include "Vector3f.hpp"
#include "XorShift128.hpp"

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
	private:
		Color3f m_emissionColor = Color3f::One();
	};

}