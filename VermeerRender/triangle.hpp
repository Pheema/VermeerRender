#pragma once

#include "geometricObject.hpp"
#include "hitInfo.hpp"
#include "vector3f.hpp"
#include "xorShift128.hpp"
#include <array>
#include <random>

namespace VermeerRender
{
	class Vertex
	{
	public:
		Vector3f pos;
		Vector3f normal;
		Vector3f uv;

		Vertex() :
			pos(Vector3f(0, 0, 0)), normal(Vector3f(0, 1, 0)), uv(Vector3f(0, 0, 0)) {};

		Vertex(const Vector3f& pos, const Vector3f& normal, const Vector3f& uv) :
			pos(pos), normal(normal), uv(uv) {}
	};

	class Triangle : public GeometricObject
	{
	public:
		std::array<const Vertex*, 3> vertexPtrs;
		
		Triangle() {}

		Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
		{
			vertexPtrs[0] = &v0;
			vertexPtrs[1] = &v1;
			vertexPtrs[2] = &v2;
		}

		Triangle(const std::array<Vertex, 3>& vertices)
		{
			vertexPtrs[0] = &vertices[0];
			vertexPtrs[1] = &vertices[1];
			vertexPtrs[2] = &vertices[2];
		}

		virtual bool
		Intersect(const Ray& ray, HitInfo* const hitInfo) override
		{
			Vector3f e1 = vertexPtrs[1]->pos - vertexPtrs[0]->pos;
			Vector3f e2 = vertexPtrs[2]->pos - vertexPtrs[0]->pos;

			const Vector3f crossEdges = Cross(e1, e2);
			const float invDet = 1.0 / Dot(-ray.dir, crossEdges);

			Vector3f vec = ray.o - vertexPtrs[0]->pos;

			float weightE1 = Dot(vec, Cross(ray.dir, e2)) * invDet;
			float weightE2 = Dot(vec, Cross(e1, ray.dir)) * invDet;

			if (weightE1 < 0 || weightE1 >= 1) return false;
			if (weightE2 < 0 || weightE2 >= 1) return false;
			if (weightE1 + weightE2 >= 1.0f) return false;

			float l = Dot(vec, crossEdges) * invDet;
			if (l < 0) return false;

			hitInfo->length = l;
			hitInfo->point = ray.o + ray.dir * l;
#if 1
			// Smooth shading
			hitInfo->normal =
				weightE1 * (vertexPtrs[1]->normal - vertexPtrs[0]->normal) +
				weightE2 * (vertexPtrs[2]->normal - vertexPtrs[0]->normal) +
				vertexPtrs[0]->normal;
#else
			// Flat shading
			hitInfo->normal = crossEdges.Normalized();
#endif
			hitInfo->hitObjPtr = this;
			return true;
		}

		virtual void
		CalcBound() override
		{
			Bounds b;
			for (auto& vertexPtr : vertexPtrs)
			{
				b.Merge(vertexPtr->pos);
			}
			bounds = b;
		}

		virtual Vector3f
		SampleSurface(const HitInfo& hitInfo) override
		{
			static XorShift128 xor;
			std::uniform_real_distribution<float> uniDist(0.0f, 1.0f);

			Vector3f e1 = vertexPtrs[1]->pos - vertexPtrs[0]->pos;
			Vector3f e2 = vertexPtrs[2]->pos - vertexPtrs[0]->pos;

			float sqrtu1 = sqrt(uniDist(xor));
			Vector3f v = vertexPtrs[0]->pos + 
				(1.0f - sqrtu1) * e1 + sqrtu1 * uniDist(xor) * e2;

			return v;
		}

		virtual float
		SamplingArea(const HitInfo& hitInfo)
		{
			Vector3f e1 = vertexPtrs[1]->pos - vertexPtrs[0]->pos;
			Vector3f e2 = vertexPtrs[2]->pos - vertexPtrs[0]->pos;

			return 0.5f * Cross(e1, e2).Length();
		}
	private:


	};
}