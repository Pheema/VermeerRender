#pragma once

#include "Vector3f.hpp"

namespace VermeerRender
{
    class HitInfo
    {
    public:
        HitInfo() {};
        Vector3f point;
        Vector3f normal;
        float length;
    };
}