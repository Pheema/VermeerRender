#pragma once

#include "HitInfo.hpp"
#include "Material.hpp"
#include "Vector3f.hpp"
#include "XorShift128.hpp"
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

            return m_matColor;
        }

    private:
        Color3f m_matColor = Color3f::Zero();
    };
}