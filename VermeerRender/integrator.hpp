#pragma once

#include "Scene.hpp"
#include "Vector3f.hpp"

namespace VermeerRender
{
	class Integrator
	{
	public:
		static Vector3f
		PathTracing(const Scene& scene, Ray* rayPtr);
	};

}