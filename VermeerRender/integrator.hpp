#pragma once

#include "scene.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
	class Integrator
	{
	public:
		static Vector3f
		PathTracing(const Scene& scene, Ray* rayPtr);
	};

}