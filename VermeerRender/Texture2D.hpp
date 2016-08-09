#pragma once

#include "lodepng.h"
#include "Vector3f.hpp"
#include <algorithm>
#include <vector>

namespace
{
    unsigned char
    ToneMap(float val)
    {
        val = std::min(std::max(val, 0.0f), 1.0f);  // Clamp
        val = pow(val, 1 / 2.2f);

        return static_cast<unsigned char>(255 * val);
    }
}

namespace VermeerRender
{
    using Color3f = Vector3f;

    class Texture2D
    {
    public:
        Texture2D() {}

        Texture2D(unsigned width, unsigned height, const Color3f& color = Color3f::Zero())
            : m_width(width), m_height(height)
        {
            Clear(color);
        }

        void
        Clear(const Color3f& color = Color3f::Zero())
        {
            pixels.resize(m_width * m_height, color);
        }

        Color3f
        GetPixel(unsigned i, unsigned j)
        {
            return pixels[i + j * m_width];
        }

        void
        SetPixel(unsigned i, unsigned j, const Color3f& color)
        {
            pixels[i + j * m_width] = color;
        }

        void
        SaveImage(const char* path) const
        {
            std::vector<unsigned char> pixels24;
            pixels24.resize(m_width * m_height * 4);
            for (size_t i = 0; i < pixels.size(); ++i)
            {
                pixels24[4 * i    ] = ToneMap(pixels[i].x); // R
                pixels24[4 * i + 1] = ToneMap(pixels[i].y); // G
                pixels24[4 * i + 2] = ToneMap(pixels[i].z); // B
                pixels24[4 * i + 3] = 255;                  // A
            }
            lodepng::encode(path, pixels24, m_width, m_height);
        }

    private:
        unsigned m_width, m_height;
        std::vector<Color3f> pixels;
    };
}