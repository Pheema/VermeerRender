#pragma once

#include "hitInfo.hpp"
#include "material.hpp"
#include "vector3f.hpp"
#include "xorShift128.hpp"
#include <random>
#include <cmath>

namespace VermeerRender
{
    class Lambert : public Material
    {
    public:
        Lambert(const Color3f& color) : m_matColor(color) {}

        virtual Color3f
        Radiance(Ray* const rayPtr, const HitInfo& hitInfo) override
        {
			static XorShift128 xor;
            std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);
            
            Vector3f v = hitInfo.normal;
            Vector3f u = Cross(Vector3f::Up(), v).Normalized();
            Vector3f w = Cross(u, v);

			// lambert BRDFに対するImportance sampling
            float phi = 2.0f * M_PI * uniDist(xor);
            float theta = acos(sqrt(uniDist(xor)));

            Vector3f reflectionRayDir =
                u * sin(theta) * cos(phi) +
                v * cos(theta) +
                w * sin(theta) * sin(phi);

            rayPtr->o = hitInfo.point;
            rayPtr->dir = reflectionRayDir;
            rayPtr->bounce++;
			rayPtr->recurrenceProb *= m_matColor.Max();
			rayPtr->rayType = RayTypes::DIFFUSE;

            return m_matColor;
        }

		virtual Color3f
		Brdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			return m_matColor * M_1_PI;
		}

		virtual float
		Pdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			return abs(Dot(outDir, hitInfo.normal)) * M_1_PI;
		};

    private:
        Color3f m_matColor = Color3f::One();
    };
}