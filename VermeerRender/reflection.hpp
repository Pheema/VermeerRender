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
			rayPtr->rayType = RayTypes::GLOSSY;

			return m_matColor;
		}

		virtual Color3f
		Brdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			// ���S���ʔ��˂�BRDF�̓f���^�֐��ƂȂ邽��
			// �قڂ��ׂĂ̏ꍇ��0
			return Color3f::Zero();
		}

		virtual float
		Pdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			// ���S���ʔ��˂�BRDF�̓f���^�֐��ƂȂ邽��
			// �قڂ��ׂĂ̏ꍇ��0
			return 0.0f;
		};

	private:
		Color3f m_matColor = Color3f::One();
	};
}