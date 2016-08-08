#include <iostream>
#include "Vector3f.hpp"
using namespace VermeerRender;

int main()
{
    std::cout << "This is the raytracer for RayTracing Camp 4!?." << std::endl;
    Vector3f v1(1, 0, 0);
    Vector3f v2(0, 1, 0);

    std::cout << Cross(v1, v2) << std::endl;

    return 0;
}