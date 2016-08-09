#include <iostream>
#include "Camera.hpp"
using namespace VermeerRender;

int main()
{
    std::cout << "This is the raytracer for RayTracing Camp 4!?." << std::endl;
    Camera mainCamera(Vector3f::Zero(), Vector3f::Forward());

    std::cout << mainCamera.Right() << std::endl;

    return 0;
}