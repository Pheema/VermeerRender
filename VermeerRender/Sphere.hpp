#pragma once

#include "geometricObject.hpp"
#include "ray.hpp"
#include "vector3f.hpp"
#include "hitInfo.hpp"
#include <algorithm>
#include <cmath>

namespace VermeerRender
{
    class Sphere : public GeometricObject
    {
    public:
        Vector3f o;
        float r;
        
        Sphere() {}
        Sphere(const Vector3f& o, float r) : o(o), r(r) {}
    
        virtual bool
        Intersect(const Ray& ray, HitInfo* const hitInfo) override
        {
            float a = Dot(ray.dir, ray.dir);
            float b_2 = Dot(ray.dir, ray.o - o);
            float c = Dot(ray.o - o, ray.o - o) - r * r;
            float D_4 = b_2 * b_2 - a * c;
            if (D_4 < 0) return false;

            float l1 = (-b_2 - sqrt(D_4)) / a;
            float l2 = (-b_2 + sqrt(D_4)) / a;

            if (l1 < 0 && l2 < 0) return false;
            float l = std::min(std::max(l1, 0.0f), std::max(l2, 0.0f));
            
            hitInfo->length = l;
            hitInfo->point = ray.o + ray.dir * l;

            // 球の外側からの衝突
            if (abs(l - l1) < FLT_EPSILON)
            {
                hitInfo->normal = (hitInfo->point - o).Normalized();
            }
            else
            {
                hitInfo->normal = -(hitInfo->point - o).Normalized();
            }

			hitInfo->hitObjPtr = this;

            return true;
        }

		virtual Vector3f
		SampleSurface(const HitInfo& hitInfo) override
		{
			static XorShift128 xor;
			std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);
			
			Vector3f v = (hitInfo.point - o).Normalized();
			Vector3f u = Cross(Vector3f::Up(), v).Normalized();
			Vector3f w = Cross(u, v);

			float l = (hitInfo.point - o).Length();

			float phi = 2.0f * M_PI * uniDist(xor);
			float theta = acos(1.0f - (1.0f - r / l) * uniDist(xor));

			Vector3f sampleDir =
				u * sin(theta) * cos(phi) +
				v * cos(theta) +
				w * sin(theta) * sin(phi);

			return o + r * sampleDir;
		}

		virtual float
		SamplingArea(const HitInfo& hitInfo)
		{
			float l = (hitInfo.point - o).Length();
			return 2.0f * M_PI * r * r * (1.0f - r / l);
		};
    };
}