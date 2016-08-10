#include <iostream>
#include "Camera.hpp"
#include "Lambert.hpp"
#include "Scene.hpp"
#include "Sphere.hpp"
#include "Texture2D.hpp"
using namespace VermeerRender;

int main()
{
    std::cout << "This is the raytracer for RayTracing Camp 4!?." << std::endl;
    Camera mainCamera(Vector3f(0, 0, 10.0f), Vector3f::Forward());
    Sphere sphere(Vector3f::Zero(), 1.0f);
    Lambert lambertMat(Color3f(1.0f, 0.0f, 0.0f));
    sphere.SetMaterial(lambertMat);

    Texture2D renderTexture(256, 256);

    Scene scene;
    scene.AddGeoObject(sphere);

    for (int i = 0; i < renderTexture.Width(); i++)
    {
        for (int j = 0; j < renderTexture.Height(); j++)
        {
            Ray camRay = mainCamera.PixelToRay(i, j, renderTexture.Width(), renderTexture.Height());
            
            HitInfo h;
            if (scene.Intersect(camRay, &h))
            {
                renderTexture.SetPixel(i, j, 0.5f * (Vector3f::One() + h.normal));
            }
        }
    }

    renderTexture.SaveImage("./Rendered/rendered.png");

    std::cout << mainCamera.Right() << std::endl;

    return 0;
}