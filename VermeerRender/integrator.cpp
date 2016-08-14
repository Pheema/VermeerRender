#include "Emission.hpp"
#include "HitInfo.hpp"
#include "integrator.hpp"
#include "Scene.hpp"
#include "Ray.hpp"
#include "Vector3f.hpp"

namespace VermeerRender
{
	Vector3f
	Integrator::PathTracing(const Scene& scene, Ray* rayPtr)
	{
		Color3f pixelColor = Color3f::One();
		// レイのトレース
		for (;;)
		{
			static XorShift128 xor;
			std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

			HitInfo h;
			if (scene.Intersect(*rayPtr, &h))
			{
				pixelColor *= (h.hitObjPtr->GetMaterial()).Radiance(rayPtr, h);
				if (uniDist(xor) < rayPtr->recurrenceProb)
				{
					//std::cout << rayPtr->recurrenceProb << std::endl;
					pixelColor /= rayPtr->recurrenceProb;
				}
				else
				{
					pixelColor = Color3f::Zero();
					break;
				}

				if (rayPtr->bounce > 16)
				{
					rayPtr->recurrenceProb *= 0.001f;
				}
				
				const std::type_info& hitMatType = typeid(h.hitObjPtr->GetMaterial());
				if (hitMatType == typeid(Emission))
				{
					break;
				}
			}
			else
			{
				pixelColor *= scene.GetBGColor();
				break;
			}
		}
		return pixelColor;
	}
}