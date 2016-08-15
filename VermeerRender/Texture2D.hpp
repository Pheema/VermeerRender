#pragma once

#include <algorithm>
#include <vector>
#include "lodepng.h"
#include "vector3f.hpp"


namespace
{
    unsigned char
    ToneMap(float val)
    {
        val = std::min(std::max(val, 0.0f), 1.0f);  // Clamp
        val = pow(val, 1 / 2.2f);

        return static_cast<unsigned char>(255 * val);
    }

	float
	InvToneMap(unsigned char c)
	{
		float val = c / 255.0f;
		val = pow(val, 2.2f);
		return val;
	}
}

namespace VermeerRender
{
    class Texture2D
    {
    public:
		explicit Texture2D(const char* path)
		{
			LoadImage(path);
		}

        Texture2D(unsigned width, unsigned height, const Color3f& color = Color3f::Zero())
            : m_width(width), m_height(height)
        {
            Clear(color);
        }

        void
        Clear(const Color3f& color = Color3f::Zero())
        {
			m_pixels.resize(m_width * m_height, color);
        }

        const Color3f&
        GetPixel(unsigned i, unsigned j) const
        {
            return m_pixels[i + (m_height - j) * m_width];
        }

		const Color3f&
		GetPixel(const Vector3f& dir) const
		{
			float u = 0.5f + 0.5f * M_1_PI * atan2(dir.z, dir.x);
			float v = 0.5f + 0.5f * M_1_PI * atan2(dir.y, sqrt(dir.x * dir.x + dir.z * dir.z));
			
			unsigned i = static_cast<unsigned>(u * m_width);
			unsigned j = static_cast<unsigned>(v * m_height);

			return GetPixel(i, j);
		}

        unsigned Width() const { return m_width; }
        unsigned Height() const { return m_height; }

        void
        SetPixel(unsigned i, unsigned j, const Color3f& color)
        {
			m_pixels[i + j * m_width] = color;
        }

        void
        SaveImage(const char* path) const
        {
            std::vector<unsigned char> pixels24;
            pixels24.resize(m_width * m_height * 4);
            for (size_t i = 0; i < m_pixels.size(); ++i)
            {
                pixels24[4 * i    ] = ToneMap(m_pixels[i].x);		// R
                pixels24[4 * i + 1] = ToneMap(m_pixels[i].y);		// G
                pixels24[4 * i + 2] = ToneMap(m_pixels[i].z);		// B
                pixels24[4 * i + 3] = 255;							// A
            }
            lodepng::encode(path, pixels24, m_width, m_height);
        }

		void
		LoadImage(const char* path)
		{
			std::vector<unsigned char> pixels24;
			lodepng::decode(pixels24, m_width, m_height, path);

			m_pixels.resize(pixels24.size() / 4);
			for (size_t i = 0; i < pixels24.size() / 4; ++i)
			{
				m_pixels[i].x = InvToneMap(pixels24[4 * i    ]);	// R
				m_pixels[i].y = InvToneMap(pixels24[4 * i + 1]);	// G
				m_pixels[i].z = InvToneMap(pixels24[4 * i + 2]);	// B
			}
		}

    private:
        unsigned m_width, m_height;
        std::vector<Color3f> m_pixels;
    };
}