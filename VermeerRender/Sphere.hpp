#pragma once

#include "GeometricObject.hpp"
#include "Ray.hpp"
#include "Vector3f.hpp"
#include <algorithm>
#include <cmath>

namespace VermeerRender
{
    class Sphere : GeometricObject
    {
    public:
        Vector3f o;
        float r;

    protected:
        virtual bool
        Intersect(const Ray& ray, HitInfo* const hitinfo) final
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

            hitinfo->length = l;
            hitinfo->point = ray.o + ray.dir * l;

            // 球の外側からの衝突
            if (abs(l - l1) < FLT_EPSILON)
            {
                hitinfo->normal = (hitinfo->point - o).Normalized();
            }
            else
            {
                hitinfo->normal = -(hitinfo->point - o).Normalized();
            }

            return true;
        }
    };
}