#pragma once

#include "scene.hpp"
#include "vector3f.hpp"

namespace VermeerRender
{
	class Integrator
	{
	public:
		// Normal path tracing
		static Vector3f
		PathTracing(const Scene& scene, Ray* rayPtr);

		// Path tracing with "next event estimation"
		static Vector3f
		PathTracingNEE(const Scene& scene, Ray* rayPtr);
	};

}