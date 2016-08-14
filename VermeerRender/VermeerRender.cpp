#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <omp.h>
#include "Camera.hpp"
#include "Emission.hpp"
#include "Lambert.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Texture2D.hpp"
#include "VermeerRender.h"
#include "integrator.hpp"

namespace VermeerRender
{
	void
	VermeerRender::Render()
	{
		std::cout << "This is the raytracer for RayTracing Camp 4!?." << std::endl;
		Camera mainCamera(Vector3f(0, 1.0f, 10.0f), Vector3f::Forward());
		Sphere sphere(Vector3f::Up(), 1.0f);
		Sphere sphereLight(Vector3f(1.0f, 3.0f, 3.0f), 1.0f);
		Sphere floor(-Vector3f::Up() * 100.0f, 100.0f);
		Lambert lambertMat(Color3f(0.9f, 0.5f, 0.5f));
		Emission emissionMat(Color3f::One());
		sphere.SetMaterial(lambertMat);
		floor.SetMaterial(lambertMat);
		sphereLight.SetMaterial(emissionMat);
		// Texture2D m_renderTexture(256, 256);

		Scene scene;
		scene.AddGeoObject(sphere);
		scene.AddGeoObject(floor);
		scene.AddGeoObject(sphereLight);
		scene.SetBGColor(Color3f(0.2f, 0.2f, 0.2f));


		const auto startTime = std::chrono::system_clock::now();

		#pragma omp parallel for schedule(dynamic, 1)
		for (int j = 0; j < m_renderTexture.Height(); j++)
		{
			for (int i = 0; i < m_renderTexture.Width(); i++)
			{
				Color3f pixelColorSum = Color3f::Zero();
				for (int smp = 0; smp < 1000; ++smp)
				{
					Ray ray = mainCamera.PixelToRay(i, j, m_renderTexture.Width(), m_renderTexture.Height());
					pixelColorSum += Integrator::PathTracing(scene, &ray);
				}
				m_renderTexture.SetPixel(i, j, pixelColorSum / 1000.0f);
			}
			std::cout << "[" << j << "/" << m_renderTexture.Height() << "]\n";
		}

		std::stringstream filepath;
		filepath << "./Rendered/rendered_final.png";
		m_renderTexture.SaveImage(filepath.str().c_str());
		exit(0);
	}
}