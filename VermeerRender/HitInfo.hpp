#pragma once

#include "ray.hpp"
#include "vector3f.hpp"
#include "geometricObject.hpp"

namespace VermeerRender
{
    class HitInfo
    {
    public:
        HitInfo() :
			point(Vector3f::Zero()), 
			normal(Vector3f::Zero()),
			uv(Vector3f(0, 0, 0)),
			ray(Ray()),
			length(INFINITY),
			hitObjPtr(nullptr)
			{};

        Vector3f point;
        Vector3f normal;
		Vector3f uv;			// Z座標が0でなければUVが存在する
		Ray ray;				// 入射したレイ
		float length;
		GeometricObject* hitObjPtr;
    };
}