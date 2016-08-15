#include <chrono>
#include <iostream>
#include <string>
#include <sstream>
#include <omp.h>
#include "camera.hpp"
#include "emission.hpp"
#include "lambert.hpp"
#include "reflection.hpp"
#include "scene.hpp"
#include "sphere.hpp"
#include "texture2D.hpp"
#include "vermeerRender.h"
#include "integrator.hpp"

namespace VermeerRender
{
	void
	VermeerRender::Render()
	{
		std::cout << "This is the raytracer for RayTracing Camp 4!?." << std::endl;

		const float largeR = 10.0f;
		Camera mainCamera(Vector3f(0, 0.0f, 10.0f), Vector3f::Forward());

		Sphere ceil(Vector3f::Up() * (largeR + 1.0f) , largeR);
		Sphere floor(-Vector3f::Up() * (largeR + 1.0f), largeR);
		Sphere leftWall(-Vector3f::Right() * (largeR + 1.0f), largeR);
		Sphere rightWall(Vector3f::Right() * (largeR + 1.0f), largeR);
		Sphere backWall(Vector3f::Forward() * (largeR + 1.0f), largeR);

		Sphere sphereLight(Vector3f(0.0f, 1.0f, 0.0f), 0.6f);
		
		// ---- Material ----
		Lambert lambertWhite(Color3f(0.9f, 0.9f, 0.9f));
		Lambert lambertRed(Color3f(0.9f, 0.0f, 0.0f));
		Lambert lambertGreen(Color3f(0.0f, 0.9f, 0.0f));
		Reflection reflectionMat(Color3f(0.9f, 0.1f, 0.1f));
		Emission emissionMat(Color3f::One());


		ceil.SetMaterial(lambertWhite);
		floor.SetMaterial(lambertWhite);
		backWall.SetMaterial(lambertWhite);
		leftWall.SetMaterial(lambertGreen);
		rightWall.SetMaterial(lambertRed);


		sphereLight.SetMaterial(reflectionMat);
		Texture2D bgTex("./Assets/bgTex.png");

		Scene scene;
		// scene.AddGeoObject(ceil);
		scene.AddGeoObject(floor);
		/*scene.AddGeoObject(backWall);
		scene.AddGeoObject(leftWall);
		scene.AddGeoObject(rightWall);*/

		scene.AddGeoObject(sphereLight);
		// scene.SetBGColor(Color3f(0.2f, 0.2f, 0.2f));
		scene.SetBGTexture(bgTex);

		const auto startTime = std::chrono::system_clock::now();

		#pragma omp parallel for schedule(dynamic, 1)
		for (int j = 0; j < m_renderTexture.Height(); j++)
		{
			for (int i = 0; i < m_renderTexture.Width(); i++)
			{
				Color3f pixelColorSum = Color3f::Zero();
				const int spp = 256;
				for (int smp = 0; smp < spp; ++smp)
				{
					Ray ray = mainCamera.PixelToRay(i, j, m_renderTexture.Width(), m_renderTexture.Height());
					pixelColorSum += Integrator::PathTracing(scene, &ray);
				}
				m_renderTexture.SetPixel(i, j, pixelColorSum / spp);
			}
			std::cout << "[" << j << "/" << m_renderTexture.Height() << "]\n";
		}

		std::stringstream filepath;
		filepath << "./Rendered/rendered_final.png";
		m_renderTexture.SaveImage(filepath.str().c_str());
		exit(0);
	}
}