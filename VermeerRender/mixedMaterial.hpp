#pragma once

#include <functional>
#include "Material.hpp"
#include "texture2D.hpp"
#include "XorShift128.hpp"

namespace VermeerRender
{
	class MixedMaterial : public Material
	{
	public:
		MixedMaterial(const Material& mat1, const Material& mat2, float ratio = 0.0f) :
			m_mat1Ptr(&mat1), m_mat2Ptr(&mat2), m_ratio(ratio) {}

		MixedMaterial(const Material& mat1, const Material& mat2, const Texture2D& ratioTex) :
			m_mat1Ptr(&mat1), m_mat2Ptr(&mat2), m_ratioTex(&ratioTex) {}

		MixedMaterial(const Material& mat1, const Material& mat2, std::function<float(const HitInfo&)>& ratioFunc) :
			m_mat1Ptr(&mat1), m_mat2Ptr(&mat2), m_ratioFunc(ratioFunc) {}

		virtual Color3f
		Radiance(Ray* const rayPtr, const HitInfo& hitInfo) const override
		{
			static XorShift128 xor;
			std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

			float ratio;
			if (m_ratioTex)
			{
				Vector3f ratioColor = m_ratioTex->GetPixelUV(hitInfo.uv.x, hitInfo.uv.y);
				ratio = (ratioColor.x + ratioColor.y + ratioColor.z) / 3.0f;
			}
			else if (m_ratioFunc)
			{
				ratio = m_ratioFunc(hitInfo);
			}
			else
			{
				ratio = m_ratio;
			}

			if (ratio < uniDist(xor))
			{
				// Mat1
				return m_mat1Ptr->Radiance(rayPtr, hitInfo);
			}
			else
			{
				// Mat2
				return m_mat2Ptr->Radiance(rayPtr, hitInfo);
			}

		}

		virtual Color3f
		Brdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) const override
		{
			std::cerr << "This function is not implemented." << std::endl;
			abort();
			return Vector3f::Zero();
		}

		virtual float
		Pdf(const Vector3f& inDir, const Vector3f& outDir, const HitInfo& hitInfo) const override
		{
			std::cerr << "This function is not implemented." << std::endl;
			abort();
			return 1.0f;
		};

	private:
		const Material* m_mat1Ptr;
		const Material* m_mat2Ptr;
		float m_ratio;

		const Texture2D* m_ratioTex = nullptr;
		std::function<float(const HitInfo&)> m_ratioFunc = nullptr;
	};
}