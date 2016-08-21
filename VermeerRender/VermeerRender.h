#pragma once

#include "texture2D.hpp"

namespace VermeerRender
{
	class VermeerRender
	{
	public:
		VermeerRender() {}

		void Render();
		void SaveImage(const char* filepath)
		{
			m_renderTexture.SaveImage(filepath);
		}
		// Texture2D m_renderTexture{ 640, 360 };
		Texture2D m_renderTexture{ 1920, 1080 };
	};
}