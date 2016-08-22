#pragma once

#include "accel.hpp"
#include "camera.hpp"
#include "geometricObject.hpp"
#include "ggx.hpp"
#include "mesh.h"
#include "scene.hpp"
#include "sphere.hpp"
#include "texture2D.hpp"
#include "triangle.hpp"
#include "lambert.hpp"

#include <vector>

namespace VermeerRender
{
	class SceneMaker
	{
	public:
		static void
		Cube(Scene *scene)
		{
			Mesh* cubeMesh = new Mesh("Assets/cube.obj");

			float floorSize = 10.0f;
			Vector3f yOffset = 0.0f * Vector3f::Up();
			Vertex* v0 = new Vertex(yOffset + floorSize * Vector3f(1.0f, 0.0f, 1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Vertex* v1 = new Vertex(yOffset + floorSize * Vector3f(1.0f, 0.0f, -1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Vertex* v2 = new Vertex(yOffset + floorSize * Vector3f(-1.0f, 0.0f, -1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Vertex* v3 = new Vertex(yOffset + floorSize * Vector3f(-1.0f, 0.0f, 1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Triangle* floor1 = new Triangle(*v0, *v1, *v2);
			Triangle* floor2 = new Triangle(*v2, *v3, *v0);

			Sphere* sphereLight = new Sphere(Vector3f(1, 2, 2), 0.2);

			Texture2D* cubeTex = new Texture2D("Assets/UV.png");
			Texture2D* bgTex = new Texture2D("Assets/bgTex_small.png");

			Lambert* cubeMat = new Lambert(*cubeTex);
			Lambert* floorMat = new Lambert(Color3f(0.9f, 0.9f, 0.9f));
			GGX* ggxMat = new GGX(Color3f(0.5f, 0.5f, 0.5f), 0.01f);
			Emission* emissionMat = new Emission(50.0f * Color3f::One());

			cubeMesh->SetMaterial(*cubeMat);
			// cubeMesh->SetMaterial(*ggxMat);
			floor1->SetMaterial(*floorMat);
			floor2->SetMaterial(*floorMat);
			sphereLight->SetMaterial(*emissionMat);
			
			Camera* camera = new Camera(Vector3f(-4.0f, 2.0f, 8.0f), Vector3f::Forward());
			camera->LookAt(0.5f * Vector3f::Up());
			camera->SetLens(150e-3f);
			camera->FocusTo(0.5f * Vector3f::Up());
			camera->SetFNumber(4.5f);
			scene->SetCamera(*camera);

			scene->AddGeoObject(*cubeMesh);
			scene->AddGeoObject(*floor1);
			scene->AddGeoObject(*floor2);
			// scene->AddGeoObject(*sphereLight);
			scene->SetBGTexture(*bgTex);
		}

		static void
		Teapot(Scene *scene)
		{
			float floorSize = 10.0f;
			Vector3f yOffset = 0.0f * Vector3f::Up();

			Vertex* v0 = new Vertex(yOffset + floorSize * Vector3f(1.0f, 0.0f, 1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Vertex* v1 = new Vertex(yOffset + floorSize * Vector3f(1.0f, 0.0f, -1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Vertex* v2 = new Vertex(yOffset + floorSize * Vector3f(-1.0f, 0.0f, -1.0f), Vector3f(0, 1, 0), Vector3f::Zero());
			Vertex* v3 = new Vertex(yOffset + floorSize * Vector3f(-1.0f, 0.0f, 1.0f), Vector3f(0, 1, 0), Vector3f::Zero());

			Triangle* floor1 = new Triangle(*v0, *v1, *v2);
			Triangle* floor2 = new Triangle(*v2, *v3, *v0);

			// ---- Models ----
			Sphere* sphereRed = new Sphere(Vector3f(-1.5f, 0.5, 0), 0.5);
			Sphere* sphereGreen = new Sphere(Vector3f(1.5f, 0.5, 0), 0.5);
			Mesh* teapot = new Mesh("Assets/teapot.obj");


			Lambert* lambertWhite = new Lambert(Color3f(0.9f, 0.9f, 0.9f));
			Lambert* lambertRed = new Lambert(Color3f(0.9f, 0, 0));
			Lambert* lambertGreen = new Lambert(Color3f(0, 0.9f, 0));
			GGX* ggxMat = new GGX(Color3f(0.5f, 1.0f, 0.5f), 0.2f);

			Texture2D* bgTex = new Texture2D("./Assets/bgTex.png");

			// ---- apply material ----
			floor1->SetMaterial(*lambertWhite);
			floor2->SetMaterial(*lambertWhite);
			sphereRed->SetMaterial(*lambertRed);
			sphereGreen->SetMaterial(*ggxMat);

			teapot->SetMaterial(*lambertWhite);

			// ---- camera settings ----
			Camera* camera = new Camera(Vector3f(-4.0f, 2.0f, 8.0f), Vector3f::Forward());
			camera->LookAt(0.5f * Vector3f::Up());
			camera->SetLens(18e-3f);
			camera->FocusTo(Vector3f(-1.5f, 0.5, 0));
			camera->SetFNumber(1.8f);
			scene->SetCamera(*camera);

			scene->AddGeoObject(*floor1);
			scene->AddGeoObject(*floor2);
			scene->AddGeoObject(*teapot);
			scene->AddGeoObject(*sphereRed);
			scene->AddGeoObject(*sphereGreen);

			scene->SetBGTexture(*bgTex);
		}

		static void
		PolyWave(Scene *scene)
		{
			// ---- Models ----
			Sphere* sphereRed = new Sphere(Vector3f(-1.5f, 0.5, 0), 0.5);
			Sphere* sphereGreen = new Sphere(Vector3f(1.5f, 0.5, 0), 0.5);
			Mesh* polyWave = new Mesh("Assets/polyWave.obj");

			Lambert* lambertWhite = new Lambert(Color3f(0.9f, 0.9f, 0.9f));
			Lambert* lambertRed = new Lambert(Color3f(0.9f, 0, 0));
			Lambert* lambertGreen = new Lambert(Color3f(0, 0.9f, 0));
			GGX* ggxMat = new GGX(Color3f(0.5f, 1.0f, 0.5f), 0.2f);

			Texture2D* bgTex = new Texture2D("./Assets/bgTex.png");

			// ---- apply material ----
			sphereRed->SetMaterial(*lambertRed);
			sphereGreen->SetMaterial(*ggxMat);

			polyWave->SetMaterial(*lambertWhite);

			// ---- camera settings ----
			Camera* camera = new Camera(Vector3f(0.0f, 1.0f, 10.0f), Vector3f::Forward());
			camera->LookAt(Vector3f::Zero());
			camera->SetLens(55e-3f);
			camera->FocusTo(Vector3f::Zero());
			camera->SetFNumber(10000.8f);
			scene->SetCamera(*camera);

			scene->AddGeoObject(*polyWave);
			scene->AddGeoObject(*sphereRed);
			scene->AddGeoObject(*sphereGreen);

			scene->SetBGTexture(*bgTex);
		}

	private:
	};

}