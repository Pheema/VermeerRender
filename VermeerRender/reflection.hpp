#pragma once

#include "hitInfo.hpp"
#include "material.hpp"
#include "ray.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
	class Reflection : public Material
	{
	public:
		Reflection(const Color3f& color) : m_matColor(color) {}

		virtual Color3f
		Radiance(Ray* const rayPtr, const HitInfo& hitInfo) override
		{
			rayPtr->o = hitInfo.point;
			rayPtr->dir = Reflect(rayPtr->dir, hitInfo.normal);
			rayPtr->bounce++;
			rayPtr->recurrenceProb *= m_matColor.Max();

			return m_matColor;
		}

	private:
		Color3f m_matColor = Color3f::One();
	};
}