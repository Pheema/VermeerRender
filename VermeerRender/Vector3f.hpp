#pragma once

#include <iostream>

namespace VermeerRender
{
	class Vector3f
	{
	public:
		float x, y, z;
		Vector3f() {}
		explicit Vector3f(float f) { this->x = f; this->y = f; this->z = f; }
		Vector3f(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }

		// Vector3f + Vector3f
		inline Vector3f
		operator+(Vector3f& v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
		
		// Vector3f - Vector3f
		inline Vector3f
		operator-(Vector3f& v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }

		// Vector3f * Vector3f (�v�f��)
		inline Vector3f
		operator*(Vector3f& v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }

		// Vector3f / Vector3f (�v�f��)
		inline Vector3f
		operator/(Vector3f& v) const { return Vector3f(x / v.x, y / v.y, z / v.z); }

		// Vector3f * number
		template <typename T>
		inline Vector3f
		operator*(T a) const { return Vector3f(x * a, y * a, z * a); }

		// number * Vector3f
		template<typename T>
		friend inline
		Vector3f operator*(T a, Vector3f& v);

	private:
		
	};

	// number * Vector3f
	template<typename T>
	Vector3f
	operator*(T a, Vector3f& v) { return v * a };

	// ����
	float
	Dot(const Vector3f& v1, const Vector3f& v2) { return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z; }

	// �O��
	Vector3f
	Cross(const Vector3f& v1, const Vector3f& v2)
	{
		Vector3f v;
		v.x = v1.y * v2.z - v1.z * v2.y;
		v.y = v1.z * v2.x - v1.x * v2.z;
		v.z = v1.x * v2.y - v1.y * v2.x;
		return v;
	}

	std::ostream&
	operator<<(std::ostream& os, const Vector3f& v)
	{
		os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
		return os;
	}
}
