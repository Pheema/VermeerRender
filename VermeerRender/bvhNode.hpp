#pragma once

#include <algorithm>
#include <vector>

#include "bounds.hpp"
#include "geometricObject.hpp"
#include "hitInfo.hpp"
#include "ray.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
	class GeometricObject;
	class HitInfo;

	class BVHNode
	{
	public:
		Bounds bounds;

		BVHNode() {}

		BVHNode(const Vector3f& vMin, const Vector3f& vMax) :
			bounds{vMin, vMax} {};

		bool
		Intersect(const Ray& ray, HitInfo* const hitInfo)
		{
			// TODO: ray.dir�����ɕ��s����0div
			Vector3f t0 = (bounds.vMin - ray.o) / ray.dir;
			Vector3f t1 = (bounds.vMax - ray.o) / ray.dir;

			for (int axis = 0; axis < 3; ++axis)
			{
				int axis2 = (axis + 1) % 3;
				float tMinMax = std::max(
					std::min(t0[axis], t1[axis]), std::min(t0[axis2], t1[axis2])
				);
				float tMaxMin = std::min(
					std::max(t0[axis], t1[axis]), std::max(t0[axis2], t1[axis2])
				);
				if (tMaxMin < tMinMax) return false;
			}

			float l = INFINITY;

			for (int axis = 0; axis < 3; ++axis)
			{
				l = std::min(l, t0[axis]);
				l = std::min(l, t1[axis]);
			}

			// ���C��BVH����O�̃��b�V���ɂ������Ă���ꍇ��
			// �q��T�����Ȃ�
			if (l > hitInfo->length) return false;

			// BVHNode�͎��ۂɐڐG���镨�̂ł͂Ȃ��̂�
			// hitInfo�̍X�V�͍s��Ȃ�

			return true;
		}
		
		// TODO: public�ϐ��̐���
	public:
		bool isLeaf = false;
		std::vector<GeometricObject*> m_childObjPtrs;
		int m_childNodeIndicies[2]{ -1, -1 };
	};
}