#include "emission.hpp"
#include "hitInfo.hpp"
#include "integrator.hpp"
#include "scene.hpp"
#include "ray.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
	Vector3f
	Integrator::PathTracing(const Scene& scene, Ray* rayPtr)
	{
		Color3f pixelColor = Color3f::Zero();
		Color3f weight = Color3f::One();
		for (;;)
		{
			static XorShift128 xor;
			std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

			HitInfo h;
			if (scene.Intersect(*rayPtr, &h))
			{
				// 光源に衝突した場合はbreak
				const std::type_info& hitMatType = typeid(h.hitObjPtr->GetMaterial());
				if (hitMatType == typeid(Emission))
				{
					pixelColor += weight * (h.hitObjPtr->GetMaterial()).Radiance(rayPtr, h);
					break;
				}

				// ---- BRDF sampling ----
				weight *= (h.hitObjPtr->GetMaterial()).Radiance(rayPtr, h);

				// ---- ロシアンルーレット ----
				if (uniDist(xor) < rayPtr->recurrenceProb)
				{
					weight /= rayPtr->recurrenceProb;
					if (rayPtr->bounce > 16)
					{
						rayPtr->recurrenceProb *= 0.001f;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				pixelColor += weight * scene.GetBGColor(rayPtr->dir);
				break;
			}
		}
		return pixelColor;
	}

	Vector3f
	Integrator::PathTracingNEE(const Scene& scene, Ray* rayPtr)
	{
		Color3f pixelColor = Color3f::Zero();
		Color3f weight = Color3f::One();
		std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

		// レイのトレース
		for (;;)
		{
			static XorShift128 xor;
			
			HitInfo h;
			if (scene.Intersect(*rayPtr, &h))
			{
				// 光源に衝突した場合はbreak
				const std::type_info& hitMatType = typeid(h.hitObjPtr->GetMaterial());
				if (hitMatType == typeid(Emission))
				{
					if (h.ray.rayType == RayTypes::CAMERA)
					{
						pixelColor += weight * h.hitObjPtr->GetMaterial().Radiance(rayPtr, h);
					}
					break;
				}

				// ---- light sampling ----
				for (const auto& objPtr : scene.GetGeoObjectPtrs())
				{
					const std::type_info& matType = typeid(objPtr->GetMaterial());
					if (matType == typeid(Emission))
					{
						// 光源上の点をランダムにサンプリング
						Vector3f samplePoint = objPtr->SampleSurface();
						Vector3f outRayDir = (samplePoint - h.point).Normalized();

						Ray shadowRay(h.point, outRayDir, RayTypes::SHADOW);

						HitInfo shadowRayHit;
						if (scene.Intersect(shadowRay, &shadowRayHit) &&
							(shadowRayHit.point - samplePoint).Length() < kEpsilon)
						{
							// シャドウレイが光源にヒットした場合
							// 寄与を加える
							pixelColor += weight * shadowRayHit.hitObjPtr->Area() *
								(shadowRayHit.hitObjPtr->GetMaterial()).Radiance(&shadowRay, shadowRayHit) *
								(h.hitObjPtr->GetMaterial()).Brdf(h.ray.dir, outRayDir, h) *
								abs(Dot(shadowRayHit.ray.dir, h.normal)) * abs(Dot(-shadowRayHit.ray.dir, shadowRayHit.normal)) /
								(samplePoint - h.point).SqLength();
						}
					}
				}

				// ---- BRDF sampling ----
				weight *= (h.hitObjPtr->GetMaterial()).Radiance(rayPtr, h);

				if (uniDist(xor) < rayPtr->recurrenceProb)
				{
					weight /= rayPtr->recurrenceProb;
					if (rayPtr->bounce > 16)
					{
						rayPtr->recurrenceProb *= 0.001f;
					}
				}
				else
				{
					break;
				}
			}
			else
			{
				pixelColor += weight * scene.GetBGColor(rayPtr->dir);
				break;
			}
		}
		return pixelColor;
	}
}