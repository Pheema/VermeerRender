#pragma once

#include <algorithm>
#include "vector3f.hpp"

 namespace VermeerRender
{
	class Bounds
	{
	public:
		Vector3f vMin, vMax;

		Bounds() :
			vMin(Vector3f::One() * INFINITY),
			vMax(-Vector3f::One() * INFINITY) {};

		Bounds(const Vector3f& vMin, const Vector3f& vMax) :
			vMin(vMin), vMax(vMax) {};

		Vector3f
		GetCenter() const
		{
			return 0.5f * (vMin + vMax);
		}

		int
		GetWidestAxis() const
		{
			float w = 0.0f;
			int result = 0;
			for (int axis = 0; axis < 3; ++axis)
			{
				float width = vMax[axis] - vMin[axis];
				if (width > w)
				{
					w = width;
					result = axis;
				}
			}
			return result;
		}

		void
		Merge(const Bounds& bounds)
		{
			// TODO: 冗長なのでvector3fへインデックスで
			// アクセスできるようにしたのち修正する
			vMin.x = std::min(vMin.x, bounds.vMin.x);
			vMin.y = std::min(vMin.y, bounds.vMin.y);
			vMin.z = std::min(vMin.z, bounds.vMin.z);
			
			vMax.x = std::max(vMax.x, bounds.vMax.x);
			vMax.y = std::max(vMax.y, bounds.vMax.y);
			vMax.z = std::max(vMax.z, bounds.vMax.z);
		}

		void
		Merge(const Vector3f& v)
		{
			vMin.x = std::min(vMin.x, v.x);
			vMin.y = std::min(vMin.y, v.y);
			vMin.z = std::min(vMin.z, v.z);

			vMax.x = std::max(vMax.x, v.x);
			vMax.y = std::max(vMax.y, v.y);
			vMax.z = std::max(vMax.z, v.z);
		}
	};
}