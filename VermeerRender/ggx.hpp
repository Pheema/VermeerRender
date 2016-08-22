#pragma once

#include <algorithm>
#include <cmath>
#include <random>
#include "constant.hpp"
#include "material.hpp"
#include "vector3f.hpp"
#include "xorShift128.hpp"

namespace VermeerRender
{
	class GGX : public Material
	{
	public:
		GGX(const Color3f& color, const float roughness) :
			m_color(color), m_roughness(roughness) {}

		virtual Color3f
		Radiance(Ray* const rayPtr, const HitInfo& hitInfo) override
		{
			// Paper
			// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf

			static XorShift128 xor;
			std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

			// Macrosurface normalを計算
			Vector3f n = Dot(hitInfo.ray.dir, hitInfo.normal) < 0 ? hitInfo.normal : -hitInfo.normal;

			Vector3f v = n;
			Vector3f u = Cross(Vector3f::Up() + Vector3f(kEpsilon, kEpsilon, kEpsilon), v).Normalized();
			Vector3f w = Cross(u, v);

			// Microsurface normalを計算
			float eta = uniDist(xor);
			float thetaM = atan(m_roughness * sqrt(eta / (1.0f - eta)));
			float phiM = 2.0f * M_PI * uniDist(xor);
			Vector3f m = 
				u * sin(thetaM) * cos(phiM) +
				v * cos(thetaM) +
				w * sin(thetaM) * sin(phiM);

			// 入射光に対してG1を計算
			auto g1 = [&](const Vector3f& vec) -> float
			{
				float chi = Dot(vec, m) / Dot(vec, n) >= 0 ? 1.0f : 0.0f;
				float sqTanThetaV = 1.0f / (Dot(vec, n) * Dot(vec, n)) - 1.0f;
				float val = 2.0f / (1.0f + sqrt(1.0f + m_roughness * m_roughness * sqTanThetaV));

				return chi * val;
			};
			float g1_im = g1(-hitInfo.ray.dir);

			// 反射ベクトルを計算
			Vector3f o = Reflect(hitInfo.ray.dir, m);

			// 反射光に対してG1を計算
			float g1_om = g1(o);

			// 分布D(m)を計算
			float chi = Dot(m, n) >= 0 ? 1.0f : 0.0f;
			
			float cosThetaM4 = cos(thetaM);
			cosThetaM4 *= cosThetaM4;
			cosThetaM4 *= cosThetaM4;

			float tanThetaM2 = tan(thetaM);
			tanThetaM2 *= tanThetaM2;

			float dmNum = m_roughness * m_roughness;
			float val = m_roughness * m_roughness + tanThetaM2;
			float dmDen = M_PI * cosThetaM4 * val * val;
			float dm = chi * dmNum / dmDen;


			rayPtr->o = hitInfo.point + kEpsilon * n;
			rayPtr->dir = o;
			rayPtr->bounce++;
			rayPtr->recurrenceProb *= m_color.Max();
			rayPtr->rayType = RayTypes::GLOSSY;

			return m_color * (g1_im * g1_om * Dot(-hitInfo.ray.dir, m)) / (Dot(-hitInfo.ray.dir, n) * Dot(m, n));
		}

		virtual Color3f
		Brdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			// TODO
			std::cerr << "This function is not implemented." << std::endl;
			abort();
			return Vector3f::Zero();
		}

		virtual float
		Pdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) override
		{
			// TODO
			std::cerr << "This function is not implemented." << std::endl;
			abort();
			return 1.0f;
		}

	private:
		Color3f m_color;
		float m_roughness;
	};
}