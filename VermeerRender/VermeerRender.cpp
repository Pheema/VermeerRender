#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <omp.h>
#include "accel.hpp"
#include "camera.hpp"
#include "mesh.h"
#include "emission.hpp"
#include "lambert.hpp"
#include "reflection.hpp"
#include "scene.hpp"
#include "sceneMaker.h"
#include "sphere.hpp"
#include "texture2D.hpp"
#include "triangle.hpp"
#include "vermeerRender.h"
#include "integrator.hpp"

namespace VermeerRender
{
	void
	VermeerRender::Render()
	{
		std::cout << "This is the raytracer for RayTracing Camp 4!?." << std::endl;

		Scene scene;
		//SceneMaker::Teapot(&scene);
		//SceneMaker::Cube(&scene);
		SceneMaker::PolyWave(&scene);

		Accel accel;
		accel.Build(scene);
		scene.accelPtr = &accel;

		const auto startTime = std::chrono::system_clock::now();

		#pragma omp parallel for schedule(dynamic, 1)
		for (int j = 0; j < m_renderTexture.Height(); j++)
		{
			for (int i = 0; i < m_renderTexture.Width(); i++)
			{
				Color3f pixelColorSum = Color3f::Zero();
				const int spp = 64;
				for (int smp = 0; smp < spp; ++smp)
				{
					Ray ray = scene.GetCamera().PixelToRay(i, j, m_renderTexture.Width(), m_renderTexture.Height());

					pixelColorSum += Integrator::PathTracing(scene, &ray);
					// pixelColorSum += Integrator::PathTracingNEE(scene, &ray);
				}
				
				#pragma omp critical
				m_renderTexture.SetPixel(i, j, pixelColorSum / spp);
			}

			std::stringstream stream;
			stream << "[" << j << "/" << m_renderTexture.Height() << "]\n";
			std::cout << stream.str();
		}

		std::stringstream filepath;
		filepath << "./Rendered/rendered_final.png";
		m_renderTexture.SaveImage(filepath.str().c_str());
		
		const auto endTime = std::chrono::system_clock::now();
		const auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime);
		std::cout << "Duration: " << duration.count() << std::endl;
		exit(0);
	}
}